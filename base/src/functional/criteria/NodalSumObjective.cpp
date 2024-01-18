#include "NodalSumObjective.hpp"

#include <numeric>
#include <vector>

#include "CriterionRegistration.hpp"
#include "Plato_InputEnumTypes.hpp"
#include "STKUtilities.hpp"

namespace Plato::Functional
{
namespace
{
[[maybe_unused]] static auto kNodalSumRegistration = CriterionFactory::CriterionRegistration{
    Plato::kCodeOptionsTable.toString(Plato::CodeOptions::kNodalSum).value(),
    [](const Plato::Functional::CriterionFactory::CriterionInput&) { return make_nodal_sum_function(); }};
}

double NodalSumObjective::f(const MeshProxy& aMeshProxy) const
{
    auto tBulk = read_mesh_bulk_data(aMeshProxy.mFileName.string());
    const std::vector<double> tCoordinates = nodal_coordinates(tBulk);
    return std::accumulate(tCoordinates.begin(), tCoordinates.end(), 0.0);
}

Core::DynamicVector<double> NodalSumObjective::df(const MeshProxy& aMeshProxy) const
{
    auto tBulk = read_mesh_bulk_data(aMeshProxy.mFileName.string());
    std::vector<double> tCoordinates = nodal_coordinates(tBulk);
    std::fill(tCoordinates.begin(), tCoordinates.end(), 1.0);
    return Core::DynamicVector<double>(std::move(tCoordinates));
}

auto make_nodal_sum_function() -> Function<double, Core::DynamicVector<double>, const MeshProxy&>
{
    return make_function([](const MeshProxy& mesh) { return NodalSumObjective{}.f(mesh); },
                         [](const MeshProxy& mesh) { return NodalSumObjective{}.df(mesh); });
}
}  // namespace Plato::Functional
