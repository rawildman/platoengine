#ifndef PLATO_FUNCTIONAL_PLATOPROBLEM
#define PLATO_FUNCTIONAL_PLATOPROBLEM

#include <memory>
#include <vector>

#include "ConstraintFactory.hpp"
#include "GeometryFactory.hpp"
#include "ObjectiveFactory.hpp"
#include "OptimizerFactory.hpp"
#include "ROLConstraintFunction.hpp"
#include "ROLObjectiveFunction.hpp"
#include "ValidatedInput.hpp"

namespace plato::functional::main::library
{
///@brief The struct that contains the functions used to create a ROL objective, as well as the ROL parameters to create
/// a ROL problem
struct PlatoProblem
{
    plato::functional::geometry::library::FactoryTypes mGeometry;
    plato::functional::criteria::library::ObjectiveFunction mObjective;
    std::vector<plato::functional::criteria::library::Constraint<const Plato::Functional::MeshProxy&>> mConstraints;
    Teuchos::ParameterList mROLOptions;
};

///@brief Convert validated parsed input into a populated PlatoProblem struct
[[nodiscard]] PlatoProblem make_plato_problem(const ValidatedInput& aData);

///@brief Create a ROL objective object from a PlatoProblem by composing the mGeometryFunction with the mObjective
[[nodiscard]] std::unique_ptr<plato::functional::rol_integration::ROLObjectiveFunction> make_rol_objective(
    const PlatoProblem& aProblem);

///@brief Create a ROL constraint from a PlatoProblem by looping through the constraints
[[nodiscard]] std::vector<std::unique_ptr<plato::functional::rol_integration::ROLConstraintFunction>>
make_rol_constraints(const PlatoProblem& aProblem);

///@brief Create a specialized ROL objective that helps facilitate testing of the parameter sensitivities
[[nodiscard]] std::unique_ptr<plato::functional::rol_integration::ROLObjectiveFunction> make_rol_sensitivity_objective(
    const PlatoProblem& aProblem);

///@brief Create the fully posed ROL problem from the PlatoProblem struct
/// Apply bound constraints, constraints, and finalize the ROL problem
[[nodiscard]] std::unique_ptr<ROL::Problem<double>> make_rol_problem(const PlatoProblem& aProblem);

}  // namespace plato::functional::main::library

#endif