#ifndef PLATO_FUNCTIONAL_GEOMETRY_LIBRARY_GEOMETRYFACTORY
#define PLATO_FUNCTIONAL_GEOMETRY_LIBRARY_GEOMETRYFACTORY

#include "DynamicVector.hpp"
#include "Function.hpp"
#include "GeometryRegistration.hpp"
#include "JacobianMultiplier.hpp"
#include "MeshProxy.hpp"
#include "ValidatedInputTypeWrapper.hpp"

namespace input_parser
{
struct PlatoInput;
}

namespace plato::functional::geometry::library
{
using DesignParameters = linear_algebra::DynamicVector<double>;
using GeometryFunction =
    core::Function<core::MeshProxy, linear_algebra::JacobianMultiplier, const linear_algebra::DynamicVector<double>&>;

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

}  // namespace plato::functional::geometry::library

#endif
