#ifndef PLATO_FUNCTIONAL_FILTERFACTORY
#define PLATO_FUNCTIONAL_FILTERFACTORY

#include <filesystem>

#include "FilterJacobian.hpp"
#include "Function.hpp"
#include "MeshProxy.hpp"

namespace Plato
{
struct PlatoInput;
struct density_topology;
namespace Functional
{
class FilterInterface;
struct FilterParameters;
}  // namespace Functional
}  // namespace Plato

namespace Plato::Functional::FilterFactory
{
using FilterFunction = Function<MeshProxy, FilterJacobian, const MeshProxy&>;

/// @brief Creates a Function used for filtering a nodal density field for density TO.
/// @param aInput The input parameters parsed from an input deck.
[[nodiscard]] FilterFunction make_filter_function(const Plato::density_topology& aInput);

namespace detail
{
/// @brief Loads a filter from a shared library.
/// @param aInput The input parameters defining the filter's properties.
/// @param aSharedLibraryPath The path at which the shared library is located.
[[nodiscard]] std::unique_ptr<FilterInterface> load_filter(const FilterParameters& aInput,
                                                           const std::filesystem::path& aSharedLibraryPath);
}  // namespace detail
}  // namespace Plato::Functional::FilterFactory

#endif
