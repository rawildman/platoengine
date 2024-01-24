#include "ObjectiveFactory.hpp"

#include <boost/mpi/communicator.hpp>

#include "CriterionFactory.hpp"
#include "DynamicVectorSerialization.hpp"
#include "RankSplitVector.hpp"
#include "ValidatedInputTypeWrapper.hpp"
#include "ValidationUtilities.hpp"

namespace Plato::Functional::ObjectiveFactory
{
namespace detail
{
namespace
{
template <typename AggregateType, typename... Args>
AggregateType make_aggregate_impl(const std::vector<Core::ValidatedInputTypeWrapper<Plato::objective>>& tObjectives, Args&&... aArgs)
{
    using ObjectiveAndWeight = std::pair<ObjectiveFunction, double>;
    std::vector<ObjectiveAndWeight> tFunctionsAndWeights;
    for (const auto& tObjective : tObjectives)
    {
        if (Plato::Functional::Validation::is_active(tObjective.rawInput()))
        {
            const double tWeight = tObjective.rawInput().aggregation_weight.value();
            tFunctionsAndWeights.emplace_back(CriterionFactory::make_criterion_function(tObjective), tWeight);
        }
    }
    return AggregateType{std::move(tFunctionsAndWeights), std::forward<Args>(aArgs)...};
}
}  // namespace

AggregateObjective make_aggregate(const ValidatedObjectives& aInput)
{
    return make_aggregate_impl<AggregateObjective>(aInput.rawInput());
}

ParallelAggregateObjective make_parallel_aggregate(const ValidatedObjectives& aInput)
{
    const auto tCommunicator = boost::mpi::communicator{};
    const auto tObjectives = Utilities::rank_split_vector(aInput.rawInput(), tCommunicator);
    return make_aggregate_impl<ParallelAggregateObjective>(tObjectives, tCommunicator);
}

}  // namespace detail

ObjectiveFunction make_aggregate_objective_function(const ValidatedObjectives& aInput)
{
    return make_aggregate_function(detail::make_parallel_aggregate(aInput));
}

}  // namespace Plato::Functional::ObjectiveFactory
