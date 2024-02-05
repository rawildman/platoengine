#include "DensityTopology.hpp"

#include "Exception.hpp"
#include "FilterInterface.hpp"
#include "FilterJacobian.hpp"
#include "GeometryRegistration.hpp"
#include "GeometryValidation.hpp"
#include "STKUtilities.hpp"

namespace plato::functional::geometry::extension
{
namespace
{
constexpr double kInitialDensity = 0.5;
constexpr double kDensityLowerBound = 0.0;
constexpr double kDensityUpperBound = 1.0;

std::function<void(const Plato::Functional::Core::DynamicVector<double>&)> make_topology_output(
    const std::filesystem::path& aInputMeshName, const std::filesystem::path& aOutputMeshName)
{
    return [aInputMeshName, aOutputMeshName](const Plato::Functional::Core::DynamicVector<double>& aSolution)
    { return DensityTopology::output(aInputMeshName, aSolution, aOutputMeshName); };
}

/// Static registration for library
[[maybe_unused]] static auto kDensityTopologyRegistration = plato::functional::geometry::library::GeometryRegistration{
    Plato::block_name<Plato::density_topology>(), [](const library::ValidatedGeometryInput& aGeometryInput)
    {
        const auto& tInput = library::geometry_raw_input<Plato::density_topology>(aGeometryInput);
        return library::FactoryTypes{
            make_topology_geometry(DensityTopology{tInput}),
            DensityTopology::initialGuess(tInput.mesh_name.value().mName),
            DensityTopology::bounds(tInput.mesh_name.value().mName),
            make_topology_output(tInput.mesh_name.value().mName, tInput.output_name.value().mName)};
    }};

/// Static registration for input validation functions
[[maybe_unused]] static auto kDensityTopologyValidationRegistration =
    Plato::Functional::Validation::Registration<Plato::density_topology>{
        [](const Plato::density_topology& aInput) { return library::detail::validate_mesh_name(aInput); },
        [](const Plato::density_topology& aInput) { return detail::validate_output_name(aInput); }};
}  // namespace

DensityTopology::DensityTopology(const Plato::density_topology& aInput)
    : mFileName(aInput.mesh_name.value().mName),
      mNumDesignParameters(Plato::Functional::read_mesh_node_size(mFileName)),
      mFilter(plato::functional::filter::library::make_filter_function(aInput))
{
}

Plato::Functional::MeshProxy DensityTopology::generateMesh(
    const Plato::Functional::Core::DynamicVector<double>& aDesignParameters) const
{
    return mFilter.f(Plato::Functional::MeshProxy{mFileName, aDesignParameters.stdVector()});
}

Plato::Functional::JacobianMultiplier DensityTopology::jacobian(
    const Plato::Functional::Core::DynamicVector<double>& aDesignParameters) const
{
    return Plato::Functional::JacobianMultiplier{
        /*.mNumColumns=*/mNumDesignParameters,
        /*.mJacobianTimesVectorFunction=*/
        [tMeshProxy = Plato::Functional::MeshProxy{mFileName, aDesignParameters.stdVector()},
         this](const Plato::Functional::Core::DynamicVector<double>& x) { return x * mFilter.df(tMeshProxy); }};
}

Plato::Functional::Core::DynamicVector<double> DensityTopology::initialGuess(const std::filesystem::path& aMeshFileName)
{
    const unsigned int tNumNodes = Plato::Functional::read_mesh_node_size(aMeshFileName);
    return Plato::Functional::Core::DynamicVector<double>(tNumNodes, kInitialDensity);
}

std::pair<std::vector<double>, std::vector<double>> DensityTopology::bounds(const std::filesystem::path& aMeshFileName)
{
    const unsigned int tNumNodes = Plato::Functional::read_mesh_node_size(aMeshFileName);
    return {std::vector<double>(tNumNodes, kDensityLowerBound), std::vector<double>(tNumNodes, kDensityUpperBound)};
}

void DensityTopology::output(const std::filesystem::path& aInputMeshName,
                             const Plato::Functional::Core::DynamicVector<double>& aSolution,
                             const std::filesystem::path& aOutputMeshName)
{
    Plato::Functional::write_mesh_density(aInputMeshName, aSolution.stdVector(), aOutputMeshName);
}

auto make_topology_geometry(const DensityTopology& aDensityTopology)
    -> Plato::Functional::Function<Plato::Functional::MeshProxy,
                                   Plato::Functional::JacobianMultiplier,
                                   const Plato::Functional::Core::DynamicVector<double>&>
{
    return Plato::Functional::make_function(
        [tDensityTopology = aDensityTopology](const Plato::Functional::Core::DynamicVector<double>& x)
        { return tDensityTopology.generateMesh(x); },
        [tDensityTopology = aDensityTopology](const Plato::Functional::Core::DynamicVector<double>& x)
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

}  // namespace plato::functional::geometry::extension
