#ifndef PLATO_FUNCTIONAL_VALIDATION
#define PLATO_FUNCTIONAL_VALIDATION

#include <optional>
#include <string>

#include "FactoryRegistration.hpp"
#include "Plato_InputBlocks.hpp"

namespace Plato::Functional::Validation
{

template <typename Input>
using ValidationFunction = std::function<std::optional<std::string>(const Input&)>;

template <typename ValidationInput>
struct Registration
{
    Registration(ValidationFunction<ValidationInput> aFunction);
    Registration(std::initializer_list<ValidationFunction<ValidationInput>> aFunctions);
};

namespace detail
{

template <typename ValidationInput>
[[nodiscard]] auto registered_functions() -> std::vector<ValidationFunction<ValidationInput>>&
{
    static auto tFunctions = std::vector<ValidationFunction<ValidationInput>>{};
    return tFunctions;
}

bool is_within_bounds(const double aValue, const std::optional<double> aLowerBound, const std::optional<double> aUpperBound);
bool is_within_lower_bounds(const double aValue, const std::optional<double> aLowerBound);
bool is_within_upper_bounds(const double aValue, const std::optional<double> aUpperBound);

}  // namespace detail

template <typename ValidationInput>
Registration<ValidationInput>::Registration(ValidationFunction<ValidationInput> aFunction)
{
    detail::registered_functions<ValidationInput>().push_back(std::move(aFunction));
}

template <typename ValidationInput>
Registration<ValidationInput>::Registration(std::initializer_list<ValidationFunction<ValidationInput>> aFunctions)
{
    std::move(aFunctions.begin(), aFunctions.end(),
              std::back_inserter(detail::registered_functions<ValidationInput>()));
}

template <typename ValidationInput>
[[nodiscard]] std::vector<std::string> validate(const ValidationInput& aInput,
                                                std::vector<std::string>&& aCurrentMessageList)
{
    auto tTests = detail::registered_functions<ValidationInput>();
    for (auto& iTest : tTests)
    {
        std::optional<std::string> tMessage = iTest(aInput);
        if (tMessage.has_value())
        {
            aCurrentMessageList.emplace_back(std::move(tMessage).value());
        }
    }
    return aCurrentMessageList;
}

void print_messages(const std::vector<std::string>& aMessages);

/// @return an optional error message if @a aParameter does not contain a value.
template <typename T>
[[nodiscard]] std::optional<std::string> error_message_for_empty_parameter(const std::string_view aPrependString,
                                                                           const boost::optional<T>& aParameter,
                                                                           const std::string_view aEntryName);

/// @return an optional error message if @a aParameter does not fall between @a aLowerBound and @a aUpperBound
template <typename T>
[[nodiscard]] std::optional<std::string> error_message_for_parameter_out_of_bounds(const std::string_view aPrependString,
                                                                                   const boost::optional<T>& aParameter,
                                                                                   const std::string_view aEntryName,
                                                                                   const std::optional<double> aLowerBound,
                                                                                   const std::optional<double> aUpperBound);

/// @brief Checks if the objective or constraint given by @a aParameter should be included in the optimization problem.
/// @tparam Must have a public field `active` that is a `boost` or `std::optional`.
template <typename Parameter>
[[nodiscard]] bool is_active(const Parameter& aParameter);

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
        const std::string tLowerBoundString = aLowerBound ? "[" + std::to_string(aLowerBound.value()) + ", " : "(-inf, ";
        const std::string tUpperBoundString = aUpperBound ? std::to_string(aUpperBound.value()) + "]." : "inf).";
        return std::string(aPrependString) + " entry \"" + std::string{aEntryName} + "\" has value " + std::to_string(aParameter.value()) +
               " and is outside the bounds " + tLowerBoundString + tUpperBoundString;
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

#endif