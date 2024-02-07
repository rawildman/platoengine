#ifndef PLATO_FUNCTIONAL_GEOMETRY_LIBRARY_GEOMETRYVALIDATION
#define PLATO_FUNCTIONAL_GEOMETRY_LIBRARY_GEOMETRYVALIDATION

#include <optional>

#include "core/ValidationUtilities.hpp"
#include "input_parser/InputBlocks.hpp"

namespace plato::functional::geometry::library
{
namespace detail
{
[[nodiscard]] std::optional<std::string> validate_only_one_geometry(const input_parser::ParsedInput& aInput);

template <typename Geometry>
[[nodiscard]] std::optional<std::string> validate_mesh_name(const Geometry& aInput)
{
    return core::error_message_for_empty_parameter(input_parser::block_name<Geometry>(), aInput.mesh_name, "mesh_name");
}

}  // namespace detail

[[nodiscard]] std::vector<std::string> validate_geometry(const input_parser::ParsedInput& aInput,
                                                         std::vector<std::string>&& aCurrentMessageList);

}  // namespace plato::functional::geometry::library

#endif
