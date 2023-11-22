#ifndef PLATO_FUNCTIONAL_FACTORYREGISTRATION
#define PLATO_FUNCTIONAL_FACTORYREGISTRATION

#include <ROL_StdVector.hpp>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#include "Function.hpp"
#include "Plato_FileList.hpp"

namespace Plato::Functional
{
template <typename Return, typename Input>
using FactoryFunction = std::function<Return(const Input&)>;

/// @brief Object used for static registration of Criterion creation functions.
///
/// The purpose of this struct is to enable static registration of the functions
/// used in CriterionFactory to create a Criterion. It must be used for registering new
/// types. Its usage is:
/// @code
/// namespace{
/// [[maybe_unused]] static auto kNewCriterionRegistration = Registration{
///   "filter-type", // Must match the name used in the input block
///    [](){ return make_new_filter(); // Must be a function that creates the desired filter
/// };
/// }
/// @endcode
template <typename FactoryReturn, typename FactoryInput>
struct Registration
{
    Registration(std::string aName, FactoryFunction<FactoryReturn, FactoryInput> aFunction);
};

/// @brief Checks if the function labeled with name @a aFunctionName is registered with the
///  factory associated with template types @a FactoryReturn and @a FactoryInput.
template <typename FactoryReturn, typename FactoryInput>
bool is_function_registered(const std::string_view aFunctionName);

namespace detail
{
/// @return Map holding registered functions used to create CriterionFunction objects in the factory.
template <typename FactoryReturn, typename FactoryInput>
[[nodiscard]] auto registered_functions()
    -> std::unordered_map<std::string, FactoryFunction<FactoryReturn, FactoryInput>>&
{
    static auto tFunctions = std::unordered_map<std::string, FactoryFunction<FactoryReturn, FactoryInput>>{};
    return tFunctions;
}

}  // namespace detail

template <typename FactoryReturn, typename FactoryInput>
Registration<FactoryReturn, FactoryInput>::Registration(std::string aName,
                                                        FactoryFunction<FactoryReturn, FactoryInput> aFunction)
{
    detail::registered_functions<FactoryReturn, FactoryInput>().try_emplace(std::move(aName), std::move(aFunction));
}

template <typename FactoryReturn, typename FactoryInput>
bool is_function_registered(const std::string_view aFunctionName)
{
    return detail::registered_functions<FactoryReturn, FactoryInput>().count(std::string{aFunctionName}) == 1;
}

}  // namespace Plato::Functional

#endif
