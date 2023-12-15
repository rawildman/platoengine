#ifndef PLATO_FUNCTIONAL_GEOMETRYVALIDATION
#define PLATO_FUNCTIONAL_GEOMETRYVALIDATION

#include <optional>

#include "Plato_InputBlocks.hpp"
#include "ValidationRegistration.hpp"

namespace Plato::Functional::Geometry
{

namespace detail
{

[[nodiscard]] std::optional<std::string> validate_only_one_geometry(const Plato::PlatoInput& aInput);

}  // namespace detail

[[nodiscard]] std::vector<std::string> validate_geometry(const Plato::PlatoInput& aInput,
                                                         std::vector<std::string>&& aCurrentMessageList);

}  // namespace Plato::Functional::Geometry

#endif