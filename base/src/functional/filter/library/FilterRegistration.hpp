#ifndef PLATO_FUNCTIONAL_FILTERREGISTRATION
#define PLATO_FUNCTIONAL_FILTERREGISTRATION

#include <filesystem>
#include <memory>

#include "FactoryRegistration.hpp"
#include "Function.hpp"

namespace Plato
{
struct density_topology;
}

namespace Plato::Functional
{
struct MeshProxy;
}  // namespace Plato::Functional

namespace plato::functional::filter::library
{
class FilterInterface;
struct FilterJacobian;

using FilterFunction =
    Plato::Functional::Function<Plato::Functional::MeshProxy, FilterJacobian, const Plato::Functional::MeshProxy&>;
using FilterInput = Plato::density_topology;
using FilterRegistration = Plato::Functional::Registration<FilterFunction, FilterInput>;

[[nodiscard]] auto make_filter_function_from_interface(std::unique_ptr<FilterInterface> aFilter) -> FilterFunction;

/// @brief Loads a filter from a shared library.
/// @param aInput The input parameters defining the filter's properties.
/// @param aSharedLibraryPath The path at which the shared library is located.
[[nodiscard]] std::unique_ptr<FilterInterface> load_filter(const Plato::density_topology& aInput,
                                                           const std::filesystem::path& aSharedLibraryPath);

[[nodiscard]] bool is_filter_function_registered(std::string_view aFunctionName);
}  // namespace plato::functional::filter::library

#endif
