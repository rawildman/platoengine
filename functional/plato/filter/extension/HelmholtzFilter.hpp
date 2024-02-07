#ifndef PLATO_FUNCTIONAL_FILTER_EXTENSION_HELMHOLTZFILTER
#define PLATO_FUNCTIONAL_FILTER_EXTENSION_HELMHOLTZFILTER

#include <optional>
#include <string>

namespace plato::functional::input_parser
{
struct density_topology;
}
namespace plato::functional::filter::extension
{
[[nodiscard]] std::optional<std::string> validate_helmholtz_filter_radius(const input_parser::density_topology& aInput);
[[nodiscard]] std::optional<std::string> validate_helmholtz_filter_boundary_sticking_penalty(
    const input_parser::density_topology& aInput);

}  // namespace plato::functional::filter::extension

#endif
