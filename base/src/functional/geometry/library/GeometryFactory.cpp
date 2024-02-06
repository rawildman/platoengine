#include "GeometryFactory.hpp"

#include "BrickShapeGeometry.hpp"
#include "DensityTopology.hpp"
#include "Exception.hpp"
#include "GeometryRegistration.hpp"
#include "GeometryRegistrationUtilities.hpp"
#include "InputBlocks.hpp"

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
