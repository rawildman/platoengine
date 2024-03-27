#include "plato/process_manager/library/ProcessManagerData.hpp"

namespace plato::process_manager::library
{
ProcessManagerData make_process_manager_data(const ValidatedInput& aData)
{
    return ProcessManagerData{plato::geometry::library::make_geometry_data(aData.geometry()),
                              plato::criteria::library::make_aggregate_objective_function(aData.objectives()),
                              plato::criteria::library::make_constraints(aData.constraints())};
}

}  // namespace plato::process_manager::library
