#include "DensityTopology.hpp"

#include "FilterInterface.hpp"
#include "Plato_InputBlocks.hpp"
#include "STKUtilities.hpp"

namespace Plato::Functional
{
namespace
{
constexpr double kInitialDensity = 0.5;
constexpr double kDensityLowerBound = 0.0;
constexpr double kDensityUpperBound = 1.0;
}  // namespace

DensityTopology::DensityTopology(const density_topology& aInput)
    : mFileName(aInput.mesh_name->mName),
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
                               this](const ROL::StdVector<double>& x) { return x * mFilter.df(tMeshProxy); }};
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

}  // namespace Plato::Functional
