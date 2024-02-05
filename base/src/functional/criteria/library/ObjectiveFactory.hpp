#ifndef PLATO_FUNCTIONAL_OBJECTIVEFACTORY
#define PLATO_FUNCTIONAL_OBJECTIVEFACTORY

#include "DynamicVector.hpp"
#include "Function.hpp"
#include "InputBlocks.hpp"
#include "ParallelAggregate.hpp"
#include "ValidatedInputTypeWrapper.hpp"

namespace Plato::Functional
{
struct MeshProxy;
}

namespace plato::functional::criteria::library
{
using ValidatedObjectives = Plato::Functional::Core::ValidatedInputTypeWrapper<
    std::vector<Plato::Functional::Core::ValidatedInputTypeWrapper<Plato::objective>>>;

using ObjectiveFunction = Plato::Functional::
    Function<double, linear_algebra::DynamicVector<double>, const Plato::Functional::MeshProxy&>;
using AggregateObjective = Plato::Functional::
    Aggregate<double, linear_algebra::DynamicVector<double>, const Plato::Functional::MeshProxy&>;
using ParallelAggregateObjective = Plato::Functional::
    ParallelAggregate<double, linear_algebra::DynamicVector<double>, const Plato::Functional::MeshProxy&>;

namespace detail
{
[[nodiscard]] AggregateObjective make_aggregate(const ValidatedObjectives& aInput);
[[nodiscard]] ParallelAggregateObjective make_parallel_aggregate(const ValidatedObjectives& aInput);
}  // namespace detail

[[nodiscard]] ObjectiveFunction make_aggregate_objective_function(const ValidatedObjectives& aInput);

}  // namespace plato::functional::criteria::library

#endif
