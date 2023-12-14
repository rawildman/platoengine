#ifndef PLATO_FUNCTIONAL_AFFIRMUTILITIES
#define PLATO_FUNCTIONAL_AFFIRMUTILITIES

#include <string>

#include "Exception.hpp"
#include "Plato_InputBlocks.hpp"

namespace Plato::Functional::Affirmations
{
/// @return an optional error message if @a aParameter does not contain a value.
template <typename T>
[[nodiscard]] std::optional<std::string> error_message_for_empty_parameter(const std::string_view aPrependString,
                                                                           const boost::optional<T>& aParameter,
                                                                           const std::string_view aEntryName);

/// @brief Checks if the objective or constraint given by @a aParameter should be included in the optimization problem.
/// @tparam Must have a public field `active` that is a `boost` or `std::optional`.
template <typename Parameter>
bool is_active(const Parameter& aParameter);

template <typename T>
std::optional<std::string> error_message_for_empty_parameter(const std::string_view aPrependString,
                                                             const boost::optional<T>& aParameter,
                                                             const std::string_view aEntryName)
{
    if (!aParameter)
    {
        return std::string(aPrependString) + " missing required entry \"" + std::string{aEntryName} + "\"";
    }
    else
    {
        return std::nullopt;
    }
}

template <typename Parameter>
bool is_active(const Parameter& aParameter)
{
    return !aParameter.active.has_value() || aParameter.active.value();
}

void print_messages(const std::vector<std::string>& aMessages);

}  // namespace Plato::Functional::Affirmations

#endif