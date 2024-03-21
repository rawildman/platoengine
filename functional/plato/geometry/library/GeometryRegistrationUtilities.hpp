#ifndef PLATO_GEOMETRY_LIBRARY_GEOMETRYREGISTRATIONUTILITIES
#define PLATO_GEOMETRY_LIBRARY_GEOMETRYREGISTRATIONUTILITIES

#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

#include "plato/geometry/library/GeometryRegistration.hpp"
#include "plato/input_parser/InputBlocks.hpp"
#include "plato/utilities/Exception.hpp"

namespace plato::geometry::library::detail
{
[[nodiscard]] std::vector<library::GeometryInput> geometry_blocks(const input_parser::ParsedInput& aInput);

/// @return An optional GeometryInput variant, which is the first non-empty geometry input block found in @a aInput.
///  If no geometry block was found, an empty optional is returned.
[[nodiscard]] std::optional<library::GeometryInput> first_geometry_block(const input_parser::ParsedInput& aInput);

/// @return The name of the geometry input held by the variant @a aInput
[[nodiscard]] std::string block_name(const library::ValidatedGeometryInput& aInput);

}  // namespace plato::geometry::library::detail

#endif
