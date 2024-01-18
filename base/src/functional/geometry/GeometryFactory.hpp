#ifndef PLATO_GEOMETRY_MANAGER
#define PLATO_GEOMETRY_MANAGER

#include <ROL_StdVector.hpp>

#include "Function.hpp"
#include "GeometryRegistration.hpp"
#include "JacobianMultiplier.hpp"
#include "MeshProxy.hpp"
#include "ValidatedInputTypeWrapper.hpp"

namespace Plato
{
struct PlatoInput;
}

namespace Plato::Functional::GeometryFactory
{
using DesignParameters = ROL::StdVector<double>;
using GeometryFunction = Function<MeshProxy, JacobianMultiplier, const ROL::StdVector<double>&>;

/// @brief Factory function for creating all geometry data, including a GeometryFunction, an initial guess,
///  the bound constraints, and an output function.
///
/// A GeometryFunction is the main geometry representation. It transforms design variables to
/// a mesh or density field.
/// The initial guess is generated from the specific geometry representation, such as
/// a uniform density field.
/// The bound constraints are generated from the specific geometry representation, such as
/// all 0 lower bounds and all 1 upper bounds for density topology optimization.
[[nodiscard]] FactoryTypes make_geometry_data(const ValidatedGeometryInput& aGeometryInput);

}  // namespace Plato::Functional::GeometryFactory

#endif
