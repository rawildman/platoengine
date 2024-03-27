#ifndef PLATO_PROCESSMANAGER_LIBRARY_PROCESSMANAGERDATA
#define PLATO_PROCESSMANAGER_LIBRARY_PROCESSMANAGERDATA

#include <Teuchos_ParameterList.hpp>
#include <vector>

#include "plato/criteria/library/ConstraintFactory.hpp"
#include "plato/criteria/library/ObjectiveFactory.hpp"
#include "plato/geometry/library/GeometryFactory.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"

namespace plato::process_manager::library
{
///@brief The struct that contains the functions used in a ProcessManager, 
struct ProcessManagerData
{
    plato::geometry::library::FactoryTypes mGeometry;
    plato::criteria::library::ObjectiveFunction mObjective;
    std::vector<plato::criteria::library::Constraint<const core::MeshProxy&>> mConstraints;
};

///@brief Convert validated parsed input into a populated ProcessManagerData struct
[[nodiscard]] ProcessManagerData make_process_manager_data(const ValidatedInput& aData);

}  // namespace plato::process_manager::library

#endif
