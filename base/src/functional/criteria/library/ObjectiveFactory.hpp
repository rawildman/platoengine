#ifndef PLATO_FUNCTIONAL_CRITERION_LIBRARY_OBJECTIVEFACTORY
#define PLATO_FUNCTIONAL_CRITERION_LIBRARY_OBJECTIVEFACTORY

#include "DynamicVector.hpp"
#include "Function.hpp"
#include "InputBlocks.hpp"
#include "ParallelAggregate.hpp"
#include "ValidatedInputTypeWrapper.hpp"

namespace plato::functional::core
{
struct MeshProxy;
}

namespace plato::functional::criteria::library
{
using ValidatedObjectives =
    core::ValidatedInputTypeWrapper<std::vector<core::ValidatedInputTypeWrapper<Plato::objective>>>;

using ObjectiveFunction = core::Function<double, linear_algebra::DynamicVector<double>, const core::MeshProxy&>;
using AggregateObjective = core::Aggregate<double, linear_algebra::DynamicVector<double>, const core::MeshProxy&>;
using ParallelAggregateObjective =
    core::ParallelAggregate<double, linear_algebra::DynamicVector<double>, const core::MeshProxy&>;

namespace detail
{
[[nodiscard]] AggregateObjective make_aggregate(const ValidatedObjectives& aInput);
[[nodiscard]] ParallelAggregateObjective make_parallel_aggregate(const ValidatedObjectives& aInput);
}  // namespace detail

[[nodiscard]] ObjectiveFunction make_aggregate_objective_function(const ValidatedObjectives& aInput);

}  // namespace plato::functional::criteria::library

#endif
