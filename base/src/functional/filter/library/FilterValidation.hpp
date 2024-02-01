#ifndef PLATO_FUNCTIONAL_FILTERVALIDATION
#define PLATO_FUNCTIONAL_FILTERVALIDATION

#include <optional>
#include <string>

namespace Plato
{
struct density_topology;
}

namespace Plato::Functional::Filter
{
[[nodiscard]] std::optional<std::string> validate_filter_type(const Plato::density_topology& aInput);
}  // namespace Plato::Functional::Filter

#endif
