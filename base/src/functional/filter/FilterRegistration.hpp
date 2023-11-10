#ifndef PLATO_FUNCTIONAL_FILTERREGISTRATION
#define PLATO_FUNCTIONAL_FILTERREGISTRATION

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#include "Function.hpp"

namespace Plato
{
struct density_topology;
}

namespace Plato::Functional
{
struct FilterJacobian;
struct MeshProxy;
class FilterInterface;
}  // namespace Plato::Functional

namespace Plato::Functional::FilterFactory
{
using FilterFunction = Function<MeshProxy, FilterJacobian, const MeshProxy&>;
using FactoryFunction = std::function<FilterFunction(const Plato::density_topology&)>;

/// @brief Object used for static registration of Filter creation functions.
///
/// The purpose of this struct is to enable static registration of the functions
/// used in FilterFactory to create a Filter. It must be used for registering new
/// types. Its usage is:
/// @code
/// namespace{
/// [[maybe_unused]] static auto kNewFilterRegistration = Registration{
///   "filter-type", // Must match the name used in the input block
///    [](){ return make_new_filter(); // Must be a function that creates the desired filter
/// };
/// }
/// @endcode
struct Registration
{
    template <typename F>
    Registration(std::string aName, F aFunction);
};

///
[[nodiscard]] auto make_filter_function_from_interface(std::unique_ptr<FilterInterface> aFilter) -> FilterFunction;

/// @brief Loads a filter from a shared library.
/// @param aInput The input parameters defining the filter's properties.
/// @param aSharedLibraryPath The path at which the shared library is located.
[[nodiscard]] std::unique_ptr<FilterInterface> load_filter(const Plato::density_topology& aInput,
                                                           const std::filesystem::path& aSharedLibraryPath);

namespace detail
{
/// @return Map holding registered functions used to create FilterFunction objects in the factory.
[[nodiscard]] auto registered_functions() -> std::unordered_map<std::string, FactoryFunction>&;
}  // namespace detail

template <typename F>
Registration::Registration(std::string aName, F aFunction)
{
    detail::registered_functions().try_emplace(std::move(aName), std::move(aFunction));
}
}  // namespace Plato::Functional::FilterFactory

#endif
