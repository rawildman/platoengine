#include "NodalSumObjective.hpp"

#include <numeric>
#include <vector>

#include "STKUtilities.hpp"

namespace Plato::Functional
{
double NodalSumObjective::f(const MeshProxy& aMeshProxy) const
{
    auto tBulk = read_mesh_bulk_data(aMeshProxy.mFileName.string());
    const std::vector<double> tCoordinates = nodal_coordinates(tBulk);
    return std::accumulate(tCoordinates.begin(), tCoordinates.end(), 0.0);
}

ROL::StdVector<double> NodalSumObjective::df(const MeshProxy& aMeshProxy) const
{
    auto tBulk = read_mesh_bulk_data(aMeshProxy.mFileName.string());
    std::vector<double> tCoordinates = nodal_coordinates(tBulk);
    std::fill(tCoordinates.begin(), tCoordinates.end(), 1.0);
    return ROL::StdVector<double>(ROL::makePtr<std::vector<double>>(std::move(tCoordinates)));
}

auto make_nodal_sum_function() -> Function<double, ROL::StdVector<double>, const MeshProxy&>
{
    return make_function([](const MeshProxy& mesh) { return NodalSumObjective{}.f(mesh); },
                         [](const MeshProxy& mesh) { return NodalSumObjective{}.df(mesh); });
}
}  // namespace Plato::Functional
