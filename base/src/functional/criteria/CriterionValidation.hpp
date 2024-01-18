#ifndef PLATO_FUNCTIONAL_CRITERIAVALIDATION
#define PLATO_FUNCTIONAL_CRITERIAVALIDATION

#include "Plato_InputBlocks.hpp"
#include "ValidationRegistration.hpp"
#include "ValidationUtilities.hpp"

namespace Plato::Functional::Criteria
{
namespace detail
{
template <typename Criteria>
[[nodiscard]] std::string criterion_name(const Criteria& aInput)
{
    std::string tName = aInput.name.value_or("unnamed");
    return Plato::block_name<Criteria>() + " " + tName;
}

template <typename Criteria>
[[nodiscard]] std::optional<std::string> validate_app(const Criteria& aInput)
{
    return Plato::Functional::Validation::error_message_for_empty_parameter(criterion_name(aInput), aInput.app, "app");
}

template <typename Criteria>
[[nodiscard]] std::optional<std::string> validate_custom_app(const Criteria& aInput)
{
    if (aInput.app.has_value() && aInput.app.value() == Plato::CodeOptions::kCustomApp)
    {
        return Plato::Functional::Validation::error_message_for_empty_parameter(
            criterion_name(aInput), aInput.shared_library_path, "shared_library_path");
    }
    else
    {
        return std::nullopt;
    }
}

template <typename Criteria>
[[nodiscard]] std::optional<std::string> validate_number_of_processors(const Criteria& aInput)
{
    namespace pfv = Plato::Functional::Validation;
    namespace pfc = Plato::Functional::Core;
    if (aInput.number_of_processors.has_value())
    {
        return pfv::error_message_for_parameter_out_of_bounds(criterion_name(aInput), aInput.number_of_processors,
                                                              "num_processors", pfc::lower_bounded(pfc::Inclusive{1u}));
    }
    else
    {
        return std::nullopt;
    }
}

template <typename Criteria>
[[nodiscard]] std::vector<std::string> validate_criteria(const std::vector<Criteria>& aInput,
                                                         std::vector<std::string>&& aCurrentMessageList)
{
    aCurrentMessageList = Plato::Functional::Validation::validate(aInput, std::move(aCurrentMessageList));
    for (const auto& iCriterionInput : aInput)
    {
        aCurrentMessageList = Plato::Functional::Validation::validate(iCriterionInput, std::move(aCurrentMessageList));
    }
    return aCurrentMessageList;
}

}  // namespace detail

}  // namespace Plato::Functional::Criteria

#endif