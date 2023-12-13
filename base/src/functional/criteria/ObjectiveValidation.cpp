#include "ObjectiveValidation.hpp"

#include "AffirmUtilities.hpp"
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
                                             const std::vector<std::string>& aCurrentMessageList)
{
    std::vector<std::string> tMessageList = aCurrentMessageList;

    tMessageList = Plato::Functional::Validation::validate<std::vector<Plato::objective>>(aInput, tMessageList);
    for (const auto& iObjectiveInput : aInput)
    {
        tMessageList = Plato::Functional::Validation::validate<Plato::objective>(iObjectiveInput, tMessageList);
    }
    return tMessageList;
}

namespace detail
{

namespace
{
std::string prepend_string(const Plato::objective& aInput)
{
    if (aInput.name.has_value())
    {
        return "Objective " + aInput.name.value();
    }
    else
    {
        return "Objective";
    }
}
}  // namespace

std::optional<std::string> validate_app(const Plato::objective& aInput)
{
    return Plato::Functional::Affirmations::error_message_for_empty_parameter(prepend_string(aInput), aInput.app,
                                                                              "app");
}

std::optional<std::string> validate_custom_app(const Plato::objective& aInput)
{
    if (aInput.app.has_value() && aInput.app.value() == Plato::CodeOptions::kCustomApp)
    {
        return Plato::Functional::Affirmations::error_message_for_empty_parameter(
            prepend_string(aInput), aInput.shared_library_path, "shared_library_path");
    }
    else
    {
        return std::nullopt;
    }
}

std::optional<std::string> validate_aggregation_weight(const Plato::objective& aInput)
{
    return Plato::Functional::Affirmations::error_message_for_empty_parameter(
        prepend_string(aInput), aInput.aggregation_weight, "aggregation_weight");
}

std::optional<std::string> validate_at_least_one_objective(const std::vector<Plato::objective>& aInput)
{
    const bool tAnyActiveObjectives = std::any_of(aInput.begin(), aInput.end(),
                                                  [](const Plato::objective& aObjective)
                                                  { return Plato::Functional::Affirmations::is_active(aObjective); });

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