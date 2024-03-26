#ifndef PLATO_PROCESSMANAGER_LIBRARY_PROCESSMANAGERFACTORY
#define PLATO_PROCESSMANAGER_LIBRARY_PROCESSMANAGERFACTORY

#include "plato/process_manager/library/ProcessManagerRegistration.hpp"

namespace plato::process_manager::library
{

[[nodiscard]] std::vector<ProcessManager> make_process_managers(
    const ValidatedProcessManagerInputVector& aValidatedProcessManagerInput);

}  // namespace plato::process_manager::library

#endif
