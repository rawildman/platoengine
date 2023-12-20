#include "GeometryFactory.hpp"

#include "BrickShapeGeometry.hpp"
#include "DensityTopology.hpp"
#include "Exception.hpp"
#include "GeometryRegistration.hpp"
#include "Plato_InputBlocks.hpp"
#include "detail/GeometryRegistrationUtilities.hpp"

namespace Plato::Functional::GeometryFactory
{

FactoryTypes make_geometry_data(const ValidGeometryInput& aGeometryInput)
{
    if (const auto tIter =
            detail::registered_functions<FactoryTypes, GeometryInput>().find(Detail::block_name(aGeometryInput.value()));
        tIter != detail::registered_functions<FactoryTypes, GeometryInput>().end())
    {
        return tIter->second(aGeometryInput.value());
    }
    else
    {
        throw Plato::Functional::Exception{"Unknown geometry"};
    }
}
}  // namespace Plato::Functional::GeometryFactory
