#include "geometry/library/GeometryFactory.hpp"

#include "utilities/Exception.hpp"
#include "geometry/library/GeometryRegistration.hpp"
#include "geometry/library/GeometryRegistrationUtilities.hpp"
#include "input_parser/InputBlocks.hpp"

namespace plato::functional::geometry::library
{
FactoryTypes make_geometry_data(const ValidatedGeometryInput& aGeometryInput)
{
    const std::optional<FactoryTypes> tGeometry =
        core::create_object_from_factory<FactoryTypes, ValidatedGeometryInput>(detail::block_name(aGeometryInput),
                                                                               aGeometryInput);

    if (tGeometry)
    {
        return std::move(tGeometry).value();
    }
    else
    {
        throw utilities::Exception{"Unknown geometry"};
    }
}
}  // namespace plato::functional::geometry::library
