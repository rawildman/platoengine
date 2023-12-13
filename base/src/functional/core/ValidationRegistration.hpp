#ifndef PLATO_FUNCTIONAL_VALIDATION
#define PLATO_FUNCTIONAL_VALIDATION

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

}  // namespace Plato::Functional::Validation

#endif