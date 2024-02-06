#ifndef PLATO_FUNCTIONAL_GEOMETRYVALIDATION
#define PLATO_FUNCTIONAL_GEOMETRYVALIDATION

#include <optional>

#include "InputBlocks.hpp"
#include "ValidationUtilities.hpp"

namespace plato::functional::geometry::library
{

namespace detail
{

[[nodiscard]] std::optional<std::string> validate_only_one_geometry(const Plato::PlatoInput& aInput);

template <typename Geometry>
[[nodiscard]] std::optional<std::string> validate_mesh_name(const Geometry& aInput)
{
    return core::error_message_for_empty_parameter(Plato::block_name<Geometry>(),
                                                                            aInput.mesh_name, "mesh_name");
}

}  // namespace detail

[[nodiscard]] std::vector<std::string> validate_geometry(const Plato::PlatoInput& aInput,
                                                         std::vector<std::string>&& aCurrentMessageList);

}  // namespace plato::functional::geometry::library

#endif
