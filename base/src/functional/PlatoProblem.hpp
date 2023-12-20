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

namespace Plato::Functional
{

///@brief The struct that contains the functions used to create a ROL objective, as well as the ROL parameters to create
/// a ROL problem
struct PlatoProblem
{
    GeometryFactory::FactoryTypes mGeometry;
    ObjectiveFactory::ObjectiveFunction mObjective;
    std::vector<ConstraintFactory::Constraint<const MeshProxy&>> mConstraints;
    Teuchos::ParameterList mROLOptions;
};

///@brief Convert validated parsed input into a populated PlatoProblem struct
///
///@param aData
///@return PlatoProblem
[[nodiscard]] PlatoProblem make_plato_problem(const Validation::ValidatedInput& aData);

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

///@brief Parse input from file @a aInputFile and then validate the input
[[nodiscard]] Validation::ValidatedInput parse_and_validate_from_file(const std::filesystem::path& aFileName);

///@brief Parse input from input string @a aInput and then validate the input
[[nodiscard]] Validation::ValidatedInput parse_and_validate(const std::string_view aInput);

}  // namespace Plato::Functional

#endif