#include "ObjectiveFactory.hpp"

#include <string>
#include <unordered_map>

#include "AffirmUtilities.hpp"
#include "CriterionFactory.hpp"
#include "Exception.hpp"
#include "MeshProxy.hpp"
#include "NodalSumObjective.hpp"
#include "SharedLibCriterion.hpp"

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
        if (Plato::Functional::Affirmations::is_active(tObjective))
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

void affirm_valid_input(const std::vector<Plato::objective>& aInput)
{
    unsigned int tNumActiveObjectives = 0;
    for (const auto& tObjective : aInput)
    {
        if (Plato::Functional::Affirmations::is_active(tObjective))
        {
            ++tNumActiveObjectives;
            Plato::Functional::Affirmations::affirm_parameter_exists(tObjective.aggregation_weight,
                                                                     "aggregation_weight");
            Plato::Functional::Affirmations::affirm_parameter_exists(tObjective.app, "app");
            if (tObjective.app.value() == Plato::CodeOptions::kCustomApp)
            {
                Plato::Functional::Affirmations::affirm_parameter_exists(tObjective.shared_library_path,
                                                                         "shared_library_path");
            }
        }
    }
    if (tNumActiveObjectives == 0)
    {
        throw Plato::Functional::Exception("Error: No active objectives found!");
    }
}

}  // namespace Plato::Functional::ObjectiveFactory
