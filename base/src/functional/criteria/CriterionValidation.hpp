#ifndef PLATO_FUNCTIONAL_CRITERIAVALIDATION
#define PLATO_FUNCTIONAL_CRITERIAVALIDATION

#include "Plato_InputBlocks.hpp"
#include "ValidationRegistration.hpp"

namespace Plato::Functional::Criteria
{

namespace detail
{

template <typename Criteria>
[[nodiscard]] std::string prepend_string(const Criteria& aInput)
{
    std::string tName = aInput.name.value_or("unnamed");
    return Plato::block_name<Criteria>() + " " + tName;
}

template <typename Criteria>
[[nodiscard]] std::optional<std::string> validate_app(const Criteria& aInput)
{
    return Plato::Functional::Validation::error_message_for_empty_parameter(prepend_string<Criteria>(aInput),
                                                                            aInput.app, "app");
}

template <typename Criteria>
[[nodiscard]] std::optional<std::string> validate_custom_app(const Criteria& aInput)
{
    if (aInput.app.has_value() && aInput.app.value() == Plato::CodeOptions::kCustomApp)
    {
        return Plato::Functional::Validation::error_message_for_empty_parameter(
            prepend_string<Criteria>(aInput), aInput.shared_library_path, "shared_library_path");
    }
    else
    {
        return std::nullopt;
    }
}

template <typename Criteria>
std::vector<std::string> validate_criteria(const std::vector<Criteria>& aInput,
                                           std::vector<std::string>&& aCurrentMessageList)
{
    aCurrentMessageList =
        Plato::Functional::Validation::validate<std::vector<Criteria>>(aInput, std::move(aCurrentMessageList));
    for (const auto& iCriterionInput : aInput)
    {
        aCurrentMessageList =
            Plato::Functional::Validation::validate<Criteria>(iCriterionInput, std::move(aCurrentMessageList));
    }
    return aCurrentMessageList;
}

}  // namespace detail

}  // namespace Plato::Functional::Criteria

#endif