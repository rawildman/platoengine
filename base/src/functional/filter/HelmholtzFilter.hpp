#ifndef PLATO_FUNCTIONAL_HELMHOLTZFILTER
#define PLATO_FUNCTIONAL_HELMHOLTZFILTER

#include <optional>
#include <string>

namespace Plato
{
struct density_topology;
}
namespace Plato::Functional
{
[[nodiscard]] std::optional<std::string> validate_helmholtz_filter_radius(const Plato::density_topology& aInput);
[[nodiscard]] std::optional<std::string> validate_helmholtz_filter_boundary_sticking_penalty(
    const Plato::density_topology& aInput);

}  // namespace Plato::Functional

#endif
