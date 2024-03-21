#include "plato/geometry/library/GeometryRegistrationUtilities.hpp"

#include "plato/core/InputVariantUtilities.hpp"

namespace plato::geometry::library::detail
{
std::vector<library::GeometryInput> geometry_blocks(const input_parser::ParsedInput& aInput)
{
    return core::all_input_blocks_in_variant<library::GeometryInput>(aInput);
}

std::optional<library::GeometryInput> first_geometry_block(const input_parser::ParsedInput& aInput)
{
    return core::first_input_block_in_variant<library::GeometryInput>(aInput);
}

std::string block_name(const library::ValidatedGeometryInput& aInput) { return core::block_name(aInput); }

}  // namespace plato::geometry::library::detail
