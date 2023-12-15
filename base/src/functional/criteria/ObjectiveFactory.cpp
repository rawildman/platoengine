#include "ObjectiveFactory.hpp"

#include <string>
#include <unordered_map>

#include "CriterionFactory.hpp"
#include "Exception.hpp"
#include "MeshProxy.hpp"
#include "NodalSumObjective.hpp"
#include "SharedLibCriterion.hpp"
#include "ValidationRegistration.hpp"

namespace Plato::Functional::ObjectiveFactory
{
namespace detail
{
AggregateObjective make_aggregate(const std::vector<Plato::objective>& aInput)
{
    using ObjectiveAndWeight = std::pair<ObjectiveFunction, double>;
    std::vector<ObjectiveAndWeight> tFunctionsAndWeights;
    for (const auto& tObjective : aInput)
    {
        if (Plato::Functional::Validation::is_active(tObjective))
        {
            const double tWeight = tObjective.aggregation_weight.value();
            tFunctionsAndWeights.emplace_back(CriterionFactory::make_criterion_function(tObjective), tWeight);
        }
    }
    return AggregateObjective{std::move(tFunctionsAndWeights)};
}
}  // namespace detail

ObjectiveFunction make_aggregate_objective_function(const std::vector<Plato::objective>& aInput)
{
    return make_aggregate_function(detail::make_aggregate(aInput));
}

}  // namespace Plato::Functional::ObjectiveFactory
