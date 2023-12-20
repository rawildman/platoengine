#include "PlatoProblem.hpp"

#include <ROL_Bounds.hpp>

#include "Compose.hpp"
#include "InputParser.hpp"
#include "MeshProxy.hpp"
#include "NodalSumObjective.hpp"
#include "ROLHelpers.hpp"

namespace Plato::Functional
{
namespace
{
[[nodiscard]] ROL::Ptr<ROL::Bounds<double>> make_rol_bound_constraint(
    const std::pair<std::vector<double>, std::vector<double>>& aBounds)
{
    auto tLowerBounds = ROL::makePtr<std::vector<double>>(aBounds.first);
    auto tUpperBounds = ROL::makePtr<std::vector<double>>(aBounds.second);
    auto tLowerBoundsVector = ROL::makePtr<ROL::StdVector<double>>(std::move(tLowerBounds));
    auto tUpperBoundsVector = ROL::makePtr<ROL::StdVector<double>>(std::move(tUpperBounds));
    return ROL::makePtr<ROL::Bounds<double>>(std::move(tLowerBoundsVector), std::move(tUpperBoundsVector));
}
}  // namespace

PlatoProblem make_plato_problem(const Validation::ValidatedInput& aData)
{
    return PlatoProblem{GeometryFactory::make_geometry_data(aData.geometry()),
                        ObjectiveFactory::make_aggregate_objective_function(aData.objectives()),
                        ConstraintFactory::make_constraints(aData.constraints()),
                        rol_parameter_list(aData.optimizationParameters())};
}

std::unique_ptr<ROLObjectiveFunction> make_rol_objective(const PlatoProblem& aProblem)
{
    return std::make_unique<ROLObjectiveFunction>(compose(aProblem.mObjective, aProblem.mGeometry.mCompute));
}

std::vector<std::unique_ptr<ROLConstraintFunction>> make_rol_constraints(const PlatoProblem& aProblem)
{
    std::vector<std::unique_ptr<ROLConstraintFunction>> tROLConstraints;
    std::transform(aProblem.mConstraints.cbegin(), aProblem.mConstraints.cend(), std::back_inserter(tROLConstraints),
                   [&aProblem](const ConstraintFactory::Constraint<const MeshProxy&>& aConstraintData)
                   {
                       ConstraintFactory::Constraint<const ROL::StdVector<double>&> tConstraint{
                           aConstraintData.mName,
                           compose(aConstraintData.mConstraintFunction, aProblem.mGeometry.mCompute),
                           aConstraintData.mConstraintTarget, aConstraintData.mLinear};
                       return std::make_unique<ROLConstraintFunction>(std::move(tConstraint));
                   });
    return tROLConstraints;
}

std::unique_ptr<ROLObjectiveFunction> make_rol_sensitivity_objective(const PlatoProblem& aProblem)
{
    auto tSimpleObjectiveFunction = make_nodal_sum_function();
    return std::make_unique<ROLObjectiveFunction>(compose(tSimpleObjectiveFunction, aProblem.mGeometry.mCompute));
}

std::unique_ptr<ROL::Problem<double>> make_rol_problem(const PlatoProblem& aProblem)
{
    auto tROLProblem =
        std::make_unique<ROL::Problem<double>>(ROL::Ptr<ROL::Objective<double>>(make_rol_objective(aProblem).release()),
                                               copy_vector(*aProblem.mGeometry.mInitialGuess));
    tROLProblem->addBoundConstraint(make_rol_bound_constraint(aProblem.mGeometry.mBounds));
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
        false;                                //( mAlgorithmType == Plato::optimizer::algorithm_t::ROL_LINEAR_CONSTRAINT ? false : true );
    tROLProblem->finalize(tLumpConstraints);  //, tPrintToStream, mOutputFile);
    return tROLProblem;
}

Validation::ValidatedInput parse_and_validate_from_file(const std::filesystem::path& aFileName)
{
    return Validation::make_validated_input(parse_input_from_file(aFileName));
}

Validation::ValidatedInput parse_and_validate(const std::string_view aInput)
{
    return Validation::make_validated_input(parse_input(aInput));
}

}  // namespace Plato::Functional
