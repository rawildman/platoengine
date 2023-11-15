#include "GeometryRegistration.hpp"

namespace Plato::Functional::GeometryFactory
{
bool is_geometry_function_registered(const std::string_view aFunctionName)
{
    return is_function_registered<GeometryFunction, GeometryInput>(aFunctionName);
}
}  // namespace Plato::Functional::GeometryFactory