#include "ObjectiveValidation.hpp"

#include "CriterionValidation.hpp"
namespace Plato::Functional::Criteria
{

[[maybe_unused]] static auto kObjectiveValidationRegistration =
    Plato::Functional::Validation::Registration<Plato::objective>{
        [](const Plato::objective& aInput) { return detail::validate_app(aInput); },
        [](const Plato::objective& aInput) { return detail::validate_custom_app(aInput); },
        [](const Plato::objective& aInput) { return detail::validate_aggregation_weight(aInput); }};

[[maybe_unused]] static auto kListObjectivesValidationRegistration =
    Plato::Functional::Validation::Registration<std::vector<Plato::objective>>{
        [](const std::vector<Plato::objective>& aInput) { return detail::validate_at_least_one_objective(aInput); }};

std::vector<std::string> validate_objectives(const std::vector<Plato::objective>& aInput,
                                             std::vector<std::string>&& aCurrentMessageList)
{
    return detail::validate_criteria(aInput, std::move(aCurrentMessageList));
}

namespace detail
{

std::optional<std::string> validate_aggregation_weight(const Plato::objective& aInput)
{
    return Plato::Functional::Validation::error_message_for_empty_parameter(
        prepend_string(aInput), aInput.aggregation_weight, "aggregation_weight");
}

std::optional<std::string> validate_at_least_one_objective(const std::vector<Plato::objective>& aInput)
{
    const bool tAnyActiveObjectives = std::any_of(aInput.begin(), aInput.end(),
                                                  [](const Plato::objective& aObjective)
                                                  { return Plato::Functional::Validation::is_active(aObjective); });

    if (tAnyActiveObjectives)
    {
        return std::nullopt;
    }
    else
    {
        return "No active objectives found.";
    }
}
}  // namespace detail
}  // namespace Plato::Functional::Criteria