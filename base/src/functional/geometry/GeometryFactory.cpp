#include "GeometryFactory.hpp"

#include "BrickShapeGeometry.hpp"
#include "DensityTopology.hpp"
#include "Exception.hpp"
#include "GeometryRegistration.hpp"
#include "Plato_InputBlocks.hpp"
#include "detail/GeometryRegistrationUtilities.hpp"

namespace Plato::Functional::GeometryFactory
{
FactoryTypes make_geometry_data(const Plato::PlatoInput& aInput)
{
    const GeometryInput tGeometryInput = Detail::geometry_input(aInput);

    if (const auto tIter =
            detail::registered_functions<FactoryTypes, GeometryInput>().find(Detail::block_name(tGeometryInput));
        tIter != detail::registered_functions<FactoryTypes, GeometryInput>().end())
    {
        return tIter->second(tGeometryInput);
    }
    else
    {
        throw Plato::Functional::Exception{"Unknown geometry"};
    }
}
}  // namespace Plato::Functional::GeometryFactory
