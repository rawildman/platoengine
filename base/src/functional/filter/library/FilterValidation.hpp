#ifndef PLATO_FUNCTIONAL_FILTER_LIBRARY_FILTERVALIDATION
#define PLATO_FUNCTIONAL_FILTER_LIBRARY_FILTERVALIDATION

#include <optional>
#include <string>

namespace plato::functional::input_parser
{
struct density_topology;
}

namespace plato::functional::filter::library
{
[[nodiscard]] std::optional<std::string> validate_filter_type(const input_parser::density_topology& aInput);
}  // namespace plato::functional::filter::library

#endif
