#include "DensityTopology.hpp"

#include "Exception.hpp"
#include "FilterInterface.hpp"
#include "FilterJacobian.hpp"
#include "GeometryRegistration.hpp"
#include "GeometryValidation.hpp"
#include "STKUtilities.hpp"

namespace Plato::Functional
{

[[maybe_unused]] static auto kDensityTopologyValidationRegistration = Validation::Registration<Plato::density_topology>{
    [](const Plato::density_topology& aInput)
    { return Geometry::detail::validate_mesh_name(aInput); },
    [](const Plato::density_topology& aInput)
    { return Validation::DensityTopology::detail::validate_output_name(aInput); },
    [](const Plato::density_topology& aInput)
    { return Validation::DensityTopology::detail::validate_filter(aInput); }};

namespace
{
constexpr double kInitialDensity = 0.5;
constexpr double kDensityLowerBound = 0.0;
constexpr double kDensityUpperBound = 1.0;

std::function<void(const ROL::StdVector<double>&)> make_topology_output(const std::filesystem::path& aInputMeshName,
                                                                        const std::filesystem::path& aOutputMeshName)
{
    return [aInputMeshName, aOutputMeshName](const ROL::StdVector<double>& aSolution)
    { return DensityTopology::output(aInputMeshName, aSolution, aOutputMeshName); };
}

/// Static registration for GeometryFactory
[[maybe_unused]] static auto kDensityTopologyRegistration = Plato::Functional::GeometryFactory::GeometryRegistration{
    Plato::block_name<Plato::density_topology>(), [](const GeometryFactory::GeometryInput& aGeometryInput)
    {
        const Plato::density_topology& tInput = std::get<Plato::density_topology>(aGeometryInput);
        return GeometryFactory::FactoryTypes{
            make_topology_geometry(DensityTopology{tInput}),
            DensityTopology::initialGuess(tInput.mesh_name.value().mName),
            DensityTopology::bounds(tInput.mesh_name.value().mName),
            make_topology_output(tInput.mesh_name.value().mName, tInput.output_name.value().mName)};
    }};
}  // namespace

DensityTopology::DensityTopology(const density_topology& aInput)
    : mFileName(aInput.mesh_name.value().mName),
      mNumDesignParameters(read_mesh_node_size(mFileName)),
      mFilter(FilterFactory::make_filter_function(aInput))
{
}

MeshProxy DensityTopology::generateMesh(const ROL::StdVector<double>& aDesignParameters) const
{
    return mFilter.f(MeshProxy{mFileName, *aDesignParameters.getVector()});
}

JacobianMultiplier DensityTopology::jacobian(const ROL::StdVector<double>& aDesignParameters) const
{
    return JacobianMultiplier{/*.mNumColumns=*/mNumDesignParameters,
                              /*.mJacobianTimesVectorFunction=*/
                              [tMeshProxy = MeshProxy{mFileName, *aDesignParameters.getVector()},
                               this](const ROL::StdVector<double>& x)
                              { return x * mFilter.df(tMeshProxy); }};
}

std::unique_ptr<ROL::StdVector<double>> DensityTopology::initialGuess(const std::filesystem::path& aMeshFileName)
{
    const unsigned int tNumNodes = read_mesh_node_size(aMeshFileName);
    return std::make_unique<ROL::StdVector<double>>(tNumNodes, kInitialDensity);
}

std::pair<std::vector<double>, std::vector<double>> DensityTopology::bounds(const std::filesystem::path& aMeshFileName)
{
    const unsigned int tNumNodes = read_mesh_node_size(aMeshFileName);
    return {std::vector<double>(tNumNodes, kDensityLowerBound), std::vector<double>(tNumNodes, kDensityUpperBound)};
}

void DensityTopology::output(const std::filesystem::path& aInputMeshName,
                             const ROL::StdVector<double>& aSolution,
                             const std::filesystem::path& aOutputMeshName)
{
    write_mesh_density(aInputMeshName, *aSolution.getVector(), aOutputMeshName);
}

auto make_topology_geometry(const DensityTopology& aDensityTopology)
    -> Function<MeshProxy, JacobianMultiplier, const ROL::StdVector<double>&>
{
    return make_function([tDensityTopology = aDensityTopology](const ROL::StdVector<double>& x)
                         { return tDensityTopology.generateMesh(x); },
                         [tDensityTopology = aDensityTopology](const ROL::StdVector<double>& x)
                         { return tDensityTopology.jacobian(x); });
}

namespace Validation::DensityTopology::detail
{

std::optional<std::string> validate_output_name(const Plato::density_topology& aInput)
{
    return Plato::Functional::Validation::error_message_for_empty_parameter(
        Plato::block_name<Plato::density_topology>(), aInput.output_name, "output_name");
}

std::optional<std::string> validate_filter(const Plato::density_topology& aInput)
{
    if (!aInput.filter_type)
    {
        return Plato::Functional::Validation::error_message_for_empty_parameter(
            Plato::block_name<Plato::density_topology>(), aInput.filter_type, "filter_type");
    }
    else
    {
        // return check_filter_values<aInput.filter_type.value()>(aInput);
        if (aInput.filter_type == Plato::FilterTypes::kIdentity)
            return check_filter_values<Plato::FilterTypes::kIdentity>(aInput);
        else if (aInput.filter_type == Plato::FilterTypes::kHelmholtz)
            return check_filter_values<Plato::FilterTypes::kHelmholtz>(aInput);
        else
            return "Unknown filter type";
    }
}

}  // namespace Validation::DensityTopology::detail

}  // namespace Plato::Functional
