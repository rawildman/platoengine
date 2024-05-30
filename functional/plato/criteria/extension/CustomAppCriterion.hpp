#ifndef PLATO_CRITERIA_EXTENSION_CUSTOMAPPCRITERION
#define PLATO_CRITERIA_EXTENSION_CUSTOMAPPCRITERION

#include <string_view>

#include "plato/core/ValidationRegistration.hpp"
#include "plato/core/ValidationUtilities.hpp"
#include "plato/criteria/library/CriterionValidation.hpp"

namespace plato::criteria::extension::detail
{
constexpr std::string_view custom_app_name() { return std::string_view{"custom_app"}; }

template <typename Criteria>
[[nodiscard]] std::optional<std::string> validate_custom_app(const Criteria& aInput)
{
    if (aInput.app.has_value() && aInput.app.value().mToken == custom_app_name())
    {
        return core::error_message_for_empty_parameter(plato::criteria::library::detail::criterion_name(aInput),
                                                       aInput.shared_library_path, "shared_library_path");
    }
    else
    {
        return std::nullopt;
    }
}
}  // namespace plato::criteria::extension::detail

#endif
