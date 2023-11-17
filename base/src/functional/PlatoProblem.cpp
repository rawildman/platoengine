#include "PlatoProblem.hpp"

#include "Compose.hpp"
#include "MeshProxy.hpp"
#include "NodalSumObjective.hpp"

namespace Plato::Functional
{

PlatoProblem make_plato_problem(const Plato::PlatoInput& aData)
{
    return PlatoProblem{GeometryFactory::make_geometry_function(aData),
                        ObjectiveFactory::make_aggregate_objective_function(aData.mObjectives),
                        ConstraintFactory::make_constraints(aData.mConstraints),
                        rol_parameter_list(aData.mOptimizationParameters),
                        ROL::Ptr<ROL::StdVector<double>>(GeometryFactory::make_initial_guess(aData).release()),
                        GeometryFactory::make_bound_constraint(aData),
                        GeometryFactory::make_output_function(aData)};
}

std::unique_ptr<ROLObjectiveFunction> make_rol_objective(const PlatoProblem& aProblem)
{
    return std::make_unique<ROLObjectiveFunction>(compose(aProblem.mObjective, aProblem.mGeometryFunction));
}

std::vector<std::unique_ptr<ROLConstraintFunction>> make_rol_constraints(const PlatoProblem& aProblem)
{
    std::vector<std::unique_ptr<ROLConstraintFunction>> tROLConstraints;
    for (auto tConstraint : aProblem.mConstraints)
    {
        Plato::Functional::ConstraintFactory::Constraint<const ROL::StdVector<double>&> tConObj{
            tConstraint.mName, compose(tConstraint.mConstraintFunction, aProblem.mGeometryFunction),
            tConstraint.mConstraintTarget, tConstraint.mLinear};
        tROLConstraints.push_back(std::make_unique<ROLConstraintFunction>(std::move(tConObj)));
    }
    return tROLConstraints;
}

std::unique_ptr<ROLObjectiveFunction> make_rol_sensitivity_objective(const PlatoProblem& aProblem)
{
    auto tSimpleObjectiveFunction = make_nodal_sum_function();
    return std::make_unique<ROLObjectiveFunction>(compose(tSimpleObjectiveFunction, aProblem.mGeometryFunction));
}

std::unique_ptr<ROL::Problem<double>> make_rol_problem(const PlatoProblem& aProblem)
{
    auto tROLProblem = std::make_unique<ROL::Problem<double>>(
        ROL::Ptr<ROL::Objective<double>>(make_rol_objective(aProblem).release()), aProblem.mInitialGuess);
    tROLProblem->addBoundConstraint(ROL::makePtr<ROL::StdBoundConstraint<double>>(aProblem.mBounds));
    for (auto& tConstraint : make_rol_constraints(aProblem))
    {
        const std::string& tName = tConstraint->name();
        if (tConstraint->linear())
        {
            tROLProblem->addLinearConstraint(tName, Teuchos::rcp(tConstraint.release()),
                                             Teuchos::rcp(ConstraintFactory::make_dual_vector().release()));
        }
        else
        {
            tROLProblem->addConstraint(tName, Teuchos::rcp(tConstraint.release()),
                                       Teuchos::rcp(ConstraintFactory::make_dual_vector().release()));
        }
    }
    ///@todo Determine how ROL lumps constraints - should this only be false if they are all linear constraints?
    constexpr bool tLumpConstraints =
        false;  //( mAlgorithmType == Plato::optimizer::algorithm_t::ROL_LINEAR_CONSTRAINT ? false : true );
    tROLProblem->finalize(tLumpConstraints);  //, tPrintToStream, mOutputFile);
    return tROLProblem;
}

ROL::Solver<double> make_rol_solver(Teuchos::ParameterList& aROLOptions, ROL::Ptr<ROL::Problem<double>> aROLProblem)
{
    return ROL::Solver<double>{std::move(aROLProblem), aROLOptions};
}

}  // namespace Plato::Functional
