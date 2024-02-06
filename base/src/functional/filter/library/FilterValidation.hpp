#ifndef PLATO_FUNCTIONAL_FILTER_LIBRARY_FILTERVALIDATION
#define PLATO_FUNCTIONAL_FILTER_LIBRARY_FILTERVALIDATION

#include <optional>
#include <string>

namespace Plato
{
struct density_topology;
}

namespace plato::functional::filter::library
{
[[nodiscard]] std::optional<std::string> validate_filter_type(const Plato::density_topology& aInput);
}  // namespace plato::functional::filter::library

#endif
