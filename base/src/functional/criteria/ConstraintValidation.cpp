#include "ConstraintValidation.hpp"

#include "AffirmUtilities.hpp"
#include "CriterionValidation.hpp"
namespace Plato::Functional::Criteria
{
[[maybe_unused]] static auto kConstraintValidationRegistration =
    Plato::Functional::Validation::Registration<Plato::constraint>{
        [](const Plato::constraint& aInput) { return detail::validate_app(aInput); },
        [](const Plato::constraint& aInput) { return detail::validate_custom_app(aInput); },
        [](const Plato::constraint& aInput) { return detail::validate_only_one_type(aInput); }};

std::vector<std::string> validate_constraints(const std::vector<Plato::constraint>& aInput,
                                              const std::vector<std::string>& aCurrentMessageList)
{
    return detail::validate_criteria(aInput, aCurrentMessageList);
}

namespace detail
{

std::optional<std::string> validate_only_one_type(const Plato::constraint& aInput)
{
    int tTally = (aInput.equal_to.has_value() ? 1 : 0);
    tTally += (aInput.less_than.has_value() ? 1 : 0);
    tTally += (aInput.greater_than.has_value() ? 1 : 0);
    if (tTally != 1)
    {
        return prepend_string<Plato::constraint>(aInput) +
               R"( must have exactly one "equal_to", "less_than", or "greater_than" type specification.)" +
               " There were " + std::to_string(tTally) + " defined.";
    }
    else
    {
        return std::nullopt;
    }
}

}  // namespace detail

}  // namespace Plato::Functional::Criteria