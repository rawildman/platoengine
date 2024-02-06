#include "GeometryRegistration.hpp"

#include "GeometryRegistrationUtilities.hpp"

namespace plato::functional::geometry::library
{
bool is_geometry_function_registered(const std::string_view aFunctionName)
{
    return core::is_factory_function_registered<FactoryTypes, ValidatedGeometryInput>(aFunctionName);
}

library::GeometryInput first_geometry_input(const input_parser::ParsedInput& aInput)
{
    const std::optional<library::GeometryInput> tGeometryInput = detail::first_geometry_block(aInput);
    if (!tGeometryInput)
    {
        throw plato::functional::utilities::Exception("No geometry block was defined.");
    }
    return tGeometryInput.value();
}

}  // namespace plato::functional::geometry::library