#ifndef PLATO_PROCESSMANAGER_EXTENSION_ROLUTILITIES
#define PLATO_PROCESSMANAGER_EXTENSION_ROLUTILITIES

#include <memory>

#include "plato/rol_integration/OptimizerFactory.hpp"
#include "plato/rol_integration/ROLConstraintFunction.hpp"
#include "plato/rol_integration/ROLObjectiveFunction.hpp"

namespace plato::process_manager::library
{
struct ProcessManagerData;
}

namespace plato::process_manager::extension
{
///@brief Create a ROL objective object from a ProcessManagerData by composing the mGeometryFunction with the mObjective
[[nodiscard]] auto make_rol_objective(const library::ProcessManagerData& aProblem)
    -> std::unique_ptr<plato::rol_integration::ROLObjectiveFunction>;

///@brief Create a ROL constraint from a ProcessManagerData by looping through the constraints
[[nodiscard]] auto make_rol_constraints(const library::ProcessManagerData& aProblem)
    -> std::vector<std::unique_ptr<plato::rol_integration::ROLConstraintFunction>>;

///@brief Create the fully posed ROL problem from the ProcessManagerData struct
/// Apply bound constraints, constraints, and finalize the ROL problem
[[nodiscard]] auto make_rol_problem(const library::ProcessManagerData& aProblem)
    -> std::unique_ptr<ROL::Problem<double>>;
}  // namespace plato::process_manager::extension

#endif
