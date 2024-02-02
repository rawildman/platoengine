#include "GeometryRegistration.hpp"
#include "GeometryRegistrationUtilities.hpp"

namespace Plato::Functional::GeometryFactory
{
bool is_geometry_function_registered(const std::string_view aFunctionName)
{
    return is_function_registered<FactoryTypes, ValidatedGeometryInput>(aFunctionName);
}

GeometryFactory::GeometryInput first_geometry_input(const Plato::PlatoInput& aInput)
{
    const std::optional<GeometryFactory::GeometryInput> tGeometryInput = Detail::first_geometry_block(aInput);
    if (!tGeometryInput)
    {
        throw Exception("No geometry block was defined.");
    }
    return tGeometryInput.value();
}

}  // namespace Plato::Functional::GeometryFactory