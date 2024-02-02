#include "DensityTopology.hpp"

#include "Exception.hpp"
#include "FilterInterface.hpp"
#include "FilterJacobian.hpp"
#include "GeometryRegistration.hpp"
#include "GeometryValidation.hpp"
#include "STKUtilities.hpp"

namespace Plato::Functional
{
namespace
{
constexpr double kInitialDensity = 0.5;
constexpr double kDensityLowerBound = 0.0;
constexpr double kDensityUpperBound = 1.0;

std::function<void(const Core::DynamicVector<double>&)> make_topology_output(
    const std::filesystem::path& aInputMeshName, const std::filesystem::path& aOutputMeshName)
{
    return [aInputMeshName, aOutputMeshName](const Core::DynamicVector<double>& aSolution)
    { return DensityTopology::output(aInputMeshName, aSolution, aOutputMeshName); };
}

/// Static registration for GeometryFactory
[[maybe_unused]] static auto kDensityTopologyRegistration = Plato::Functional::GeometryFactory::GeometryRegistration{
    Plato::block_name<Plato::density_topology>(), [](const GeometryFactory::ValidatedGeometryInput& aGeometryInput)
    {
        const auto& tInput = GeometryFactory::geometry_raw_input<Plato::density_topology>(aGeometryInput);
        return GeometryFactory::FactoryTypes{
            make_topology_geometry(DensityTopology{tInput}),
            DensityTopology::initialGuess(tInput.mesh_name.value().mName),
            DensityTopology::bounds(tInput.mesh_name.value().mName),
            make_topology_output(tInput.mesh_name.value().mName, tInput.output_name.value().mName)};
    }};

/// Static registration for input validation functions
[[maybe_unused]] static auto kDensityTopologyValidationRegistration = Validation::Registration<Plato::density_topology>{
    [](const Plato::density_topology& aInput) { return Geometry::detail::validate_mesh_name(aInput); },
    [](const Plato::density_topology& aInput) { return detail::validate_output_name(aInput); }};
}  // namespace

DensityTopology::DensityTopology(const density_topology& aInput)
    : mFileName(aInput.mesh_name.value().mName),
      mNumDesignParameters(read_mesh_node_size(mFileName)),
      mFilter(FilterFactory::make_filter_function(aInput))
{
}

MeshProxy DensityTopology::generateMesh(const Core::DynamicVector<double>& aDesignParameters) const
{
    return mFilter.f(MeshProxy{mFileName, aDesignParameters.stdVector()});
}

JacobianMultiplier DensityTopology::jacobian(const Core::DynamicVector<double>& aDesignParameters) const
{
    return JacobianMultiplier{/*.mNumColumns=*/mNumDesignParameters,
                              /*.mJacobianTimesVectorFunction=*/
                              [tMeshProxy = MeshProxy{mFileName, aDesignParameters.stdVector()},
                               this](const Core::DynamicVector<double>& x) { return x * mFilter.df(tMeshProxy); }};
}

Core::DynamicVector<double> DensityTopology::initialGuess(const std::filesystem::path& aMeshFileName)
{
    const unsigned int tNumNodes = read_mesh_node_size(aMeshFileName);
    return Core::DynamicVector<double>(tNumNodes, kInitialDensity);
}

std::pair<std::vector<double>, std::vector<double>> DensityTopology::bounds(const std::filesystem::path& aMeshFileName)
{
    const unsigned int tNumNodes = read_mesh_node_size(aMeshFileName);
    return {std::vector<double>(tNumNodes, kDensityLowerBound), std::vector<double>(tNumNodes, kDensityUpperBound)};
}

void DensityTopology::output(const std::filesystem::path& aInputMeshName,
                             const Core::DynamicVector<double>& aSolution,
                             const std::filesystem::path& aOutputMeshName)
{
    write_mesh_density(aInputMeshName, aSolution.stdVector(), aOutputMeshName);
}

auto make_topology_geometry(const DensityTopology& aDensityTopology)
    -> Function<MeshProxy, JacobianMultiplier, const Core::DynamicVector<double>&>
{
    return make_function([tDensityTopology = aDensityTopology](const Core::DynamicVector<double>& x)
                         { return tDensityTopology.generateMesh(x); },
                         [tDensityTopology = aDensityTopology](const Core::DynamicVector<double>& x)
                         { return tDensityTopology.jacobian(x); });
}

namespace detail
{
std::optional<std::string> validate_output_name(const Plato::density_topology& aInput)
{
    return Plato::Functional::Validation::error_message_for_empty_parameter(
        Plato::block_name<Plato::density_topology>(), aInput.output_name, "output_name");
}

}  // namespace detail

}  // namespace Plato::Functional
