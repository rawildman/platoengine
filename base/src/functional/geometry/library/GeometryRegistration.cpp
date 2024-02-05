#include "GeometryRegistration.hpp"

#include "GeometryRegistrationUtilities.hpp"

namespace plato::functional::geometry::library
{
bool is_geometry_function_registered(const std::string_view aFunctionName)
{
    return Plato::Functional::is_function_registered<FactoryTypes, ValidatedGeometryInput>(aFunctionName);
}

library::GeometryInput first_geometry_input(const Plato::PlatoInput& aInput)
{
    const std::optional<library::GeometryInput> tGeometryInput = detail::first_geometry_block(aInput);
    if (!tGeometryInput)
    {
        throw Plato::Functional::Exception("No geometry block was defined.");
    }
    return tGeometryInput.value();
}

}  // namespace plato::functional::geometry::library