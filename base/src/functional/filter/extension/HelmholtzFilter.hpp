#ifndef PLATO_FUNCTIONAL_HELMHOLTZFILTER
#define PLATO_FUNCTIONAL_HELMHOLTZFILTER

#include <optional>
#include <string>

namespace Plato
{
struct density_topology;
}
namespace plato::functional::filter::extension
{
[[nodiscard]] std::optional<std::string> validate_helmholtz_filter_radius(const Plato::density_topology& aInput);
[[nodiscard]] std::optional<std::string> validate_helmholtz_filter_boundary_sticking_penalty(
    const Plato::density_topology& aInput);

}  // namespace plato::functional::filter::extension

#endif
