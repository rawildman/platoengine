#include "GeometryFactory.hpp"

#include "BrickShapeGeometry.hpp"
#include "DensityTopology.hpp"
#include "Exception.hpp"
#include "GeometryRegistration.hpp"
#include "GeometryRegistrationUtilities.hpp"
#include "InputBlocks.hpp"

namespace Plato::Functional::GeometryFactory
{

FactoryTypes make_geometry_data(const ValidatedGeometryInput& aGeometryInput)
{
    if (const auto tIter = detail::registered_functions<FactoryTypes, ValidatedGeometryInput>().find(
            Detail::block_name(aGeometryInput));
        tIter != detail::registered_functions<FactoryTypes, ValidatedGeometryInput>().end())
    {
        return tIter->second(aGeometryInput);
    }
    else
    {
        throw Plato::Functional::Exception{"Unknown geometry"};
    }
}
}  // namespace Plato::Functional::GeometryFactory
