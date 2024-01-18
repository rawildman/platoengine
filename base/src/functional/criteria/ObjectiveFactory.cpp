#include "ObjectiveFactory.hpp"

#include "CriterionFactory.hpp"
#include "ValidatedInputTypeWrapper.hpp"
#include "ValidationUtilities.hpp"

namespace Plato::Functional::ObjectiveFactory
{
namespace detail
{
AggregateObjective make_aggregate(const ValidatedObjectives& aInput)
{
    using ObjectiveAndWeight = std::pair<ObjectiveFunction, double>;
    std::vector<ObjectiveAndWeight> tFunctionsAndWeights;
    for (const auto& tObjective : aInput.rawInput())
    {
        if (Plato::Functional::Validation::is_active(tObjective.rawInput()))
        {
            const double tWeight = tObjective.rawInput().aggregation_weight.value();
            tFunctionsAndWeights.emplace_back(CriterionFactory::make_criterion_function(tObjective), tWeight);
        }
    }
    return AggregateObjective{std::move(tFunctionsAndWeights)};
}
}  // namespace detail

ObjectiveFunction make_aggregate_objective_function(const ValidatedObjectives& aInput)
{
    return make_aggregate_function(detail::make_aggregate(aInput));
}

}  // namespace Plato::Functional::ObjectiveFactory
