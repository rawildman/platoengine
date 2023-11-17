#ifndef PLATO_GEOMETRY_MANAGER
#define PLATO_GEOMETRY_MANAGER

#include <ROL_StdBoundConstraint.hpp>
#include <ROL_StdVector.hpp>

#include "Function.hpp"
#include "JacobianMultiplier.hpp"
#include "MeshProxy.hpp"

namespace Plato
{
struct PlatoInput;
}

namespace Plato::Functional::GeometryFactory
{
using DesignParameters = ROL::StdVector<double>;
using GeometryFunction = Function<MeshProxy, JacobianMultiplier, const ROL::StdVector<double>&>;

/// @brief Factory function for creating a GeometryFunction.
///
/// A GeometryFunction is the main geometry representation. It transforms design variables to
/// a mesh or density field.
[[nodiscard]] GeometryFunction make_geometry_function(const Plato::PlatoInput& aInput);

/// @brief Generates an initial guess based on the input settings in @a aInput
///
/// The initial guess is generated from the specific geometry representation, such as
/// a uniform density field.
[[nodiscard]] std::unique_ptr<ROL::StdVector<double>> make_initial_guess(const Plato::PlatoInput& aInput);

/// @brief Generates a ROL bound constraint based on the input settings in @a aInput.
///
/// The bound constraints are generated from the specific geometry representation, such as
/// all 0 lower bounds and all 1 upper bounds for density topology optimization.
[[nodiscard]] ROL::StdBoundConstraint<double> make_bound_constraint(const Plato::PlatoInput& aInput);

/// @brief Generates a function that performs some output task based on the input settings in @a aInput.
[[nodiscard]] std::function<void(const ROL::StdVector<double>&)> make_output_function(const Plato::PlatoInput& aInput);
}  // namespace Plato::Functional::GeometryFactory

#endif
