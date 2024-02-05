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
    if (const auto tIter = Plato::Functional::detail::registered_functions<FactoryTypes, ValidatedGeometryInput>().find(
            detail::block_name(aGeometryInput));
        tIter != Plato::Functional::detail::registered_functions<FactoryTypes, ValidatedGeometryInput>().end())
    {
        return tIter->second(aGeometryInput);
    }
    else
    {
        throw Plato::Functional::Exception{"Unknown geometry"};
    }
}
}  // namespace plato::functional::geometry::library
