#ifndef PLATO_FUNCTIONAL_VALIDATIONREGISTRATION
#define PLATO_FUNCTIONAL_VALIDATIONREGISTRATION

#include <optional>
#include <string>

#include "FactoryRegistration.hpp"
#include "Plato_InputBlocks.hpp"

namespace Plato::Functional::Validation
{
template <typename Input>
using ValidationFunction = std::function<std::optional<std::string>(const Input&)>;

/// @brief Object used for static registration of validation functions that validate
///  parsed input data.
///
/// The purpose of this struct is to enable static registration of the functions
/// used in to validate parsed user input of type @a ValidationInput. The ctor can
/// be used with a list of functions, each of which will be added to the full set
/// of validation functions for the templated type.
///
/// To register functions, client code should instantiate a static object in a cpp file.
/// For example, registering a new validation function for density topology is:
/// @code
/// namespace{
/// [[maybe_unused]] static auto kNewValidationRegistration =
///   Plato::Functonal::Validation::Registration<Plato::density_topology>{
///    [](const Plato::density_topology& aInput){ return validate_foo_parameter(aInput); }
/// };
/// }
/// @endcode
///
/// @tparam ValidationInput The type of the input data needed by the validation function as an argument.
template <typename ValidationInput>
struct Registration
{
    Registration(ValidationFunction<ValidationInput> aFunction);
    Registration(std::initializer_list<ValidationFunction<ValidationInput>> aFunctions);
};

/// @brief Validates @a aInput, appending any error messages to @a aCurrentMessageList and returning
///  the result.
template <typename ValidationInput>
[[nodiscard]] std::vector<std::string> validate(const ValidationInput& aInput,
                                                std::vector<std::string>&& aCurrentMessageList);

namespace detail
{
template <typename ValidationInput>
[[nodiscard]] auto registered_functions() -> std::vector<ValidationFunction<ValidationInput>>&
{
    static auto tFunctions = std::vector<ValidationFunction<ValidationInput>>{};
    return tFunctions;
}

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
    const auto tTests = detail::registered_functions<ValidationInput>();
    for (const auto& iTest : tTests)
    {
        std::optional<std::string> tMessage = iTest(aInput);
        if (tMessage.has_value())
        {
            aCurrentMessageList.emplace_back(std::move(tMessage).value());
        }
    }
    return aCurrentMessageList;
}

}  // namespace Plato::Functional::Validation

#endif
