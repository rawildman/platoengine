#include "OptimizationProblem.hpp"

#include <ROL_Algorithm.hpp>
#include <fstream>
#include <string_view>

#include "AffirmUtilities.hpp"
#include "ConstraintValidation.hpp"
#include "Exception.hpp"
#include "GeometryFactory.hpp"
#include "InputParser.hpp"
#include "MeshProxy.hpp"
#include "ObjectiveFactory.hpp"
#include "ObjectiveValidation.hpp"
#include "PlatoProblem.hpp"

namespace Plato::Functional
{
namespace
{
constexpr std::string_view kROLGradientCheckFileName = "ROL_Gradient_Check.txt";
constexpr std::string_view kROLSensitivityCheckFileName = "ROL_Sensitivity_Check.txt";
constexpr std::string_view kROLOptimizerFileName = "ROL_Optimizer.txt";
constexpr std::string_view kROLConstraintCheckFileName = "ROL_Constraint_Check.txt";

///@brief Helper function that creates a perturbation of the initial value for use in diagnostic checks
ROL::StdVector<double> generatePerturbation(const int aDimension)
{
    ROL::StdVector<double> tPerturbation(aDimension);
    tPerturbation.randomize();
    return tPerturbation;
}

}  // namespace

OptimizationProblem::OptimizationProblem(const std::string_view aInputFile)
    : mProblem(make_plato_problem(parse_and_validate(aInputFile))),
      mROLProblem(make_rol_problem(mProblem).release()),
      mROLSolver(make_rol_solver(mProblem.mROLOptions, mROLProblem))
{
}

void OptimizationProblem::gradientCheck() const
{
    std::ofstream tOutFile(std::string{kROLGradientCheckFileName});
    constexpr bool tPrintOutput = true;

    mROLProblem->getObjective()->checkGradient(*mProblem.mGeometry.mInitialGuess, generatePerturbation(dimension()),
                                               tPrintOutput, tOutFile);
}

void OptimizationProblem::constraintCheck() const
{
    // This mutates the initial guess, so it shouldn't be used as is before
    // an optimization run. Calling clone on each vector would be a solution, but doing so
    // on the result of getResidualVector seg faults
    constexpr double tPerturbationScale = 0.1;
    auto tPrimal = mROLProblem->getPrimalOptimizationVector();
    tPrimal->randomize(0, tPerturbationScale);
    auto tv = mROLProblem->getPrimalOptimizationVector();
    tv->randomize(0, tPerturbationScale);
    auto tResidual = mROLProblem->getResidualVector();
    tResidual->randomize(-tPerturbationScale, tPerturbationScale);
    auto tMultiplier = mROLProblem->getMultiplierVector();
    tMultiplier->randomize(-tPerturbationScale, tPerturbationScale);

    auto tConstraint = mROLProblem->getConstraint();
    if (tConstraint)
    {
        constexpr bool tPrintOutput = true;
        std::ofstream tOutput(std::string{kROLConstraintCheckFileName});
        tConstraint->checkApplyJacobian(*tPrimal, *tv, *tResidual, tPrintOutput, tOutput);
        tConstraint->checkAdjointConsistencyJacobian(*tMultiplier, *tv, *tPrimal, tPrintOutput, tOutput);
        mROLProblem->checkLinearity(tPrintOutput, tOutput);
    }
    else
    {
        std::cout << "No constraints, skipping constraint check.\n";
    }
}

void OptimizationProblem::sensitivityCheck() const
{
    std::ofstream tOutFile(std::string{kROLSensitivityCheckFileName});
    constexpr bool tPrintOutput = true;

    auto tSensitivityObjective = make_rol_sensitivity_objective(mProblem);
    tSensitivityObjective->checkGradient(*mProblem.mGeometry.mInitialGuess, generatePerturbation(dimension()),
                                         tPrintOutput, tOutFile);
}

void OptimizationProblem::optimize()
{
    std::ofstream tOutFile(std::string{kROLOptimizerFileName});
    mROLSolver.solve(tOutFile);

    auto tSolution = dynamic_cast<ROL::StdVector<double>&>(*mROLProblem->getPrimalOptimizationVector());
    mProblem.mGeometry.mOutput(tSolution);
}

int OptimizationProblem::dimension() const { return mProblem.mGeometry.mInitialGuess->dimension(); }

ValidatedInput parse_and_validate(const std::string_view aInputFile)
{
    const auto tInput = parse_input_from_file(aInputFile);
    std::vector<std::string> tMessages;
    tMessages = Criteria::validate_objectives(tInput.mObjectives, std::move(tMessages));
    tMessages = Criteria::validate_constraints(tInput.mConstraints, std::move(tMessages));
    if (tMessages.size() != 0)
    {
        Affirmations::print_messages(tMessages);
        throw Exception("Could not validate input.");
    }
    return ValidatedInput{tInput};
}

}  // namespace Plato::Functional
