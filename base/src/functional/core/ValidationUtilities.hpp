#include <optional>
#include <string>
#include <vector>

#include "Plato_InputBlocks.hpp"

namespace Plato::Functional::Validation
{
/// @brief Creates a string by concatenating each entry of @a aMessages, with a newline between each.
[[nodiscard]] std::string all_messages(const std::vector<std::string>& aMessages);

/// @return an optional error message if @a aParameter does not contain a value.
template <typename T>
[[nodiscard]] std::optional<std::string> error_message_for_empty_parameter(const std::string_view aPrependString,
                                                                           const boost::optional<T>& aParameter,
                                                                           const std::string_view aEntryName);

/// @return an optional error message if @a aParameter does not fall between @a aLowerBound and @a aUpperBound
template <typename T>
[[nodiscard]] std::optional<std::string> error_message_for_parameter_out_of_bounds(
    const std::string_view aPrependString,
    const boost::optional<T>& aParameter,
    const std::string_view aEntryName,
    const std::optional<double> aLowerBound,
    const std::optional<double> aUpperBound);

/// @brief Checks if the objective or constraint given by @a aParameter should be included in the optimization problem.
/// @tparam Must have a public field `active` that is a `boost` or `std::optional`.
template <typename Parameter>
[[nodiscard]] bool is_active(const Parameter& aParameter);

namespace detail
{
[[nodiscard]] bool is_within_bounds(const double aValue,
                                    const std::optional<double> aLowerBound,
                                    const std::optional<double> aUpperBound);
[[nodiscard]] bool is_within_lower_bounds(const double aValue, const std::optional<double> aLowerBound);
[[nodiscard]] bool is_within_upper_bounds(const double aValue, const std::optional<double> aUpperBound);
}  // namespace detail

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

template <typename T>
std::optional<std::string> error_message_for_parameter_out_of_bounds(const std::string_view aPrependString,
                                                                     const boost::optional<T>& aParameter,
                                                                     const std::string_view aEntryName,
                                                                     const std::optional<double> aLowerBound,
                                                                     const std::optional<double> aUpperBound)
{
    if (aParameter && !detail::is_within_bounds(aParameter.value(), aLowerBound, aUpperBound))
    {
        const std::string tLowerBoundString =
            aLowerBound ? "[" + std::to_string(aLowerBound.value()) + ", " : "(-inf, ";
        const std::string tUpperBoundString = aUpperBound ? std::to_string(aUpperBound.value()) + "]." : "inf).";
        return std::string(aPrependString) + " entry \"" + std::string{aEntryName} + "\" has value " +
               std::to_string(aParameter.value()) + " and is outside the bounds " + tLowerBoundString +
               tUpperBoundString;
    }
    else
    {
        return error_message_for_empty_parameter(aPrependString, aParameter, aEntryName);
    }
}

template <typename Parameter>
bool is_active(const Parameter& aParameter)
{
    return !aParameter.active.has_value() || aParameter.active.value();
}

}  // namespace Plato::Functional::Validation
