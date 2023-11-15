#ifndef PLATO_FUNCTIONAL_GEOMETRYREGISTRATION
#define PLATO_FUNCTIONAL_GEOMETRYREGISTRATION

#include <ROL_StdVector.hpp>
#include <memory>
#include <string_view>
#include <variant>

#include "FactoryRegistration.hpp"
#include "Function.hpp"

namespace Plato
{
struct density_topology;
struct brick_shape_geometry;

namespace Functional
{
struct JacobianMultiplier;
struct MeshProxy;
}  // namespace Functional
}  // namespace Plato

namespace Plato::Functional::GeometryFactory
{

using GeometryInput = std::variant<Plato::brick_shape_geometry, Plato::density_topology>;
using GeometryFunction = Function<MeshProxy, JacobianMultiplier, const ROL::StdVector<double>&>;
using GeometryRegistration = Registration<GeometryFunction, GeometryInput>;

bool is_geometry_function_registered(const std::string_view aFunctionName);

}  // namespace Plato::Functional::GeometryFactory

#endif