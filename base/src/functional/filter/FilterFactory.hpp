#ifndef PLATO_FUNCTIONAL_FILTERFACTORY
#define PLATO_FUNCTIONAL_FILTERFACTORY

#include <filesystem>
#include <unordered_map>

#include "FilterJacobian.hpp"
#include "Function.hpp"
#include "MeshProxy.hpp"

namespace Plato
{
struct density_topology;
}  // namespace Plato

namespace Plato::Functional::FilterFactory
{
using FilterFunction = Function<MeshProxy, FilterJacobian, const MeshProxy&>;

/// @brief Creates a Function used for filtering a nodal density field for density TO.
/// @param aInput The input parameters parsed from an input deck.
[[nodiscard]] FilterFunction make_filter_function(const Plato::density_topology& aInput);

}  // namespace Plato::Functional::FilterFactory

#endif
