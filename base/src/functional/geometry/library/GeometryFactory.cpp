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
    if (const auto tIter = core::detail::registered_factory_functions<FactoryTypes, ValidatedGeometryInput>().find(
            detail::block_name(aGeometryInput));
        tIter != core::detail::registered_factory_functions<FactoryTypes, ValidatedGeometryInput>().end())
    {
        return tIter->second(aGeometryInput);
    }
    else
    {
        throw utilities::Exception{"Unknown geometry"};
    }
}
}  // namespace plato::functional::geometry::library
