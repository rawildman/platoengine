#include "ConstraintValidation.hpp"

#include "CriterionValidation.hpp"

namespace Plato::Functional::Criteria
{
[[maybe_unused]] static auto kConstraintValidationRegistration =
    Plato::Functional::Validation::Registration<Plato::constraint>{
        [](const Plato::constraint& aInput)
        { return detail::validate_app(aInput); },
        [](const Plato::constraint& aInput)
        { return detail::validate_custom_app(aInput); },
        [](const Plato::constraint& aInput)
        { return detail::validate_equal_to(aInput); }};

std::vector<std::string> validate_constraints(const std::vector<Plato::constraint>& aInput,
                                              std::vector<std::string>&& aCurrentMessageList)
{
    return detail::validate_criteria(aInput, std::move(aCurrentMessageList));
}

namespace detail
{

std::optional<std::string> validate_equal_to(const Plato::constraint& aInput)
{
    return Plato::Functional::Validation::error_message_for_empty_parameter(prepend_string<Plato::constraint>(aInput),
                                                                            aInput.equal_to, "equal_to");
}

}  // namespace detail

}  // namespace Plato::Functional::Criteria