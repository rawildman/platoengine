#ifndef PLATO_FUNCTIONAL_PLATOPROBLEM
#define PLATO_FUNCTIONAL_PLATOPROBLEM

#include <ROL_Solver.hpp>
#include <ROL_StdBoundConstraint.hpp>
#include <memory>
#include <vector>

#include "ConstraintFactory.hpp"
#include "GeometryFactory.hpp"
#include "ObjectiveFactory.hpp"
#include "OptimizerFactory.hpp"
#include "ROLConstraintFunction.hpp"
#include "ROLObjectiveFunction.hpp"

namespace Plato::Functional
{

///@brief The struct that contains the functions used to create a ROL objective, as well as the ROL parameters to create
///a ROL problem
struct PlatoProblem
{
    GeometryFactory::GeometryFunction mGeometryFunction;
    ObjectiveFactory::ObjectiveFunction mObjective;
    std::vector<ConstraintFactory::Constraint<const MeshProxy&>> mConstraints;
    Teuchos::ParameterList mROLOptions;
    ROL::Ptr<ROL::StdVector<double>> mInitialGuess;
    ROL::StdBoundConstraint<double> mBounds;
    std::function<void(const ROL::StdVector<double>&)> mOutput;
};

///@brief Convert validated parsed input into a populated PlatoProblem struct
///
///@param aData
///@return PlatoProblem
[[nodiscard]] PlatoProblem make_plato_problem(const Plato::PlatoInput& aData);

///@brief Create a ROL objective object from a PlatoProblem by composing the mGeometryFunction with the mObjective
///
///@param aProblem
///@return std::unique_ptr<ROLObjectiveFunction>
[[nodiscard]] std::unique_ptr<ROLObjectiveFunction> make_rol_objective(const PlatoProblem& aProblem);

///@brief Create a ROL constraint from a PlatoProblem by looping through the constraints
///
///@param aProblem
///@return std::vector<std::unique_ptr<ROLConstraintFunction> >
[[nodiscard]] std::vector<std::unique_ptr<ROLConstraintFunction>> make_rol_constraints(const PlatoProblem& aProblem);

///@brief Create a specialized ROL objective that helps facilitate testing of the parameter sensitivities
///
///@param aProblem
///@return std::unique_ptr<ROLObjectiveFunction>
[[nodiscard]] std::unique_ptr<ROLObjectiveFunction> make_rol_sensitivity_objective(const PlatoProblem& aProblem);

///@brief Create the fully posed ROL problem from the PlatoProblem struct
/// Apply bound constraints, constraints, and finalize the ROL problem
///@param aProblem
///@return std::unique_ptr<ROL::Problem<double>>
[[nodiscard]] std::unique_ptr<ROL::Problem<double>> make_rol_problem(const PlatoProblem& aProblem);

///@brief Create a ROL solver based on the ROL problem and the ROL options specified in the solver parameter list
///
///@param aROLOptions
///@param aProblem
///@return ROL::Solver<double>
[[nodiscard]] ROL::Solver<double> make_rol_solver(Teuchos::ParameterList& aROLOptions,
                                                  ROL::Ptr<ROL::Problem<double>> aProblem);
}  // namespace Plato::Functional

#endif