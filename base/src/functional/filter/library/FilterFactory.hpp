#ifndef PLATO_FUNCTIONAL_FILTERFACTORY
#define PLATO_FUNCTIONAL_FILTERFACTORY

#include "Function.hpp"

namespace Plato
{
struct density_topology;

namespace Functional
{
struct MeshProxy;
}  // namespace Functional
}  // namespace Plato

namespace plato::functional::filter::library
{
struct FilterJacobian;

using FilterFunction =
    Plato::Functional::Function<Plato::Functional::MeshProxy, FilterJacobian, const Plato::Functional::MeshProxy&>;

/// @brief Creates a Function used for filtering a nodal density field for density TO.
/// @param aInput The input parameters parsed from an input deck.
[[nodiscard]] FilterFunction make_filter_function(const Plato::density_topology& aInput);

}  // namespace plato::functional::filter::library

#endif
