#ifndef PLATO_FUNCTIONAL_CRITERION_LIBRARY_OBJECTIVEFACTORY
#define PLATO_FUNCTIONAL_CRITERION_LIBRARY_OBJECTIVEFACTORY

#include "core/Function.hpp"
#include "core/ParallelAggregate.hpp"
#include "core/ValidatedInputTypeWrapper.hpp"
#include "input_parser/InputBlocks.hpp"
#include "linear_algebra/DynamicVector.hpp"

namespace plato::functional::core
{
struct MeshProxy;
}

namespace plato::functional::criteria::library
{
using ValidatedObjectives =
    core::ValidatedInputTypeWrapper<std::vector<core::ValidatedInputTypeWrapper<input_parser::objective>>>;

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
