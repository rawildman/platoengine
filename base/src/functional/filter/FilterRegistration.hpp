#ifndef PLATO_FUNCTIONAL_FILTERREGISTRATION
#define PLATO_FUNCTIONAL_FILTERREGISTRATION

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#include "Function.hpp"
#include "FactoryRegistration.hpp"

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
using FilterInput = Plato::density_topology;
using FilterRegistration = Registration<FilterFunction, FilterInput>;

[[nodiscard]] auto make_filter_function_from_interface(std::unique_ptr<FilterInterface> aFilter) -> FilterFunction;

/// @brief Loads a filter from a shared library.
/// @param aInput The input parameters defining the filter's properties.
/// @param aSharedLibraryPath The path at which the shared library is located.
[[nodiscard]] std::unique_ptr<FilterInterface> load_filter(const Plato::density_topology& aInput,
                                                           const std::filesystem::path& aSharedLibraryPath);

bool is_filter_function_registered(const std::string_view aFunctionName);
}  // namespace Plato::Functional::FilterFactory

#endif
