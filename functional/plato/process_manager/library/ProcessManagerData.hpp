#ifndef PLATO_PROCESSMANAGER_LIBRARY_PROCESSMANAGERDATA
#define PLATO_PROCESSMANAGER_LIBRARY_PROCESSMANAGERDATA

#include <Teuchos_ParameterList.hpp>
#include <memory>
#include <vector>

#include "plato/criteria/library/ConstraintFactory.hpp"
#include "plato/criteria/library/ObjectiveFactory.hpp"
#include "plato/geometry/library/GeometryFactory.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"
#include "plato/rol_integration/OptimizerFactory.hpp"
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
[[nodiscard]] auto make_rol_objective(const ProcessManagerData& aProblem)
    -> std::unique_ptr<plato::rol_integration::ROLObjectiveFunction>;

///@brief Create a ROL constraint from a ProcessManagerData by looping through the constraints
[[nodiscard]] auto make_rol_constraints(const ProcessManagerData& aProblem)
    -> std::vector<std::unique_ptr<plato::rol_integration::ROLConstraintFunction>>;

///@brief Create a specialized ROL objective that helps facilitate testing of the parameter sensitivities
[[nodiscard]] auto make_rol_sensitivity_objective(const ProcessManagerData& aProblem)
    -> std::unique_ptr<plato::rol_integration::ROLObjectiveFunction>;

///@brief Create the fully posed ROL problem from the ProcessManagerData struct
/// Apply bound constraints, constraints, and finalize the ROL problem
[[nodiscard]] auto make_rol_problem(const ProcessManagerData& aProblem) -> std::unique_ptr<ROL::Problem<double>>;

}  // namespace plato::process_manager::library

#endif
