#ifndef PLATO_PROCESSMANAGER_LIBRARY_PROCESSMANAGERDATA
#define PLATO_PROCESSMANAGER_LIBRARY_PROCESSMANAGERDATA

#include <memory>
#include <vector>

#include "plato/criteria/library/ConstraintFactory.hpp"
#include "plato/criteria/library/ObjectiveFactory.hpp"
#include "plato/geometry/library/GeometryFactory.hpp"
#include "plato/optimizer/OptimizerFactory.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"
#include "plato/rol_integration/ROLConstraintFunction.hpp"
#include "plato/rol_integration/ROLObjectiveFunction.hpp"

namespace plato::process_manager::library
{
///@brief The struct that contains the functions used to create a ROL objective, as well as the ROL parameters to create
/// a ROL problem
struct ProcessManagerData
{
    plato::geometry::library::FactoryTypes mGeometry;
    plato::criteria::library::ObjectiveFunction mObjective;
    std::vector<plato::criteria::library::Constraint<const core::MeshProxy&>> mConstraints;
    Teuchos::ParameterList mROLOptions;
};

///@brief Convert validated parsed input into a populated ProcessManagerData struct
[[nodiscard]] ProcessManagerData make_process_manager_data(const ValidatedInput& aData);

///@brief Create a ROL objective object from a ProcessManagerData by composing the mGeometryFunction with the mObjective
[[nodiscard]] std::unique_ptr<plato::rol_integration::ROLObjectiveFunction> make_rol_objective(
    const ProcessManagerData& aProblem);

///@brief Create a ROL constraint from a ProcessManagerData by looping through the constraints
[[nodiscard]] std::vector<std::unique_ptr<plato::rol_integration::ROLConstraintFunction>> make_rol_constraints(
    const ProcessManagerData& aProblem);

///@brief Create a specialized ROL objective that helps facilitate testing of the parameter sensitivities
[[nodiscard]] std::unique_ptr<plato::rol_integration::ROLObjectiveFunction> make_rol_sensitivity_objective(
    const ProcessManagerData& aProblem);

///@brief Create the fully posed ROL problem from the ProcessManagerData struct
/// Apply bound constraints, constraints, and finalize the ROL problem
[[nodiscard]] std::unique_ptr<ROL::Problem<double>> make_rol_problem(const ProcessManagerData& aProblem);

}  // namespace plato::process_manager::library

#endif
