#ifndef PLATO_PROCESSMANAGER_LIBRARY_PROCESSMANAGERFACTORY
#define PLATO_PROCESSMANAGER_LIBRARY_PROCESSMANAGERFACTORY

#include "plato/process_manager/library/ProcessManagerRegistration.hpp"

namespace plato::process_manager::library
{
/// @brief Creates all ProcessManager objects from the parsed and validated input in
///  @a aValidatedProcessManagerInput.
///
/// @post The order of the objects in the returned is guaranteed to be sorted based on
///  each object's RunStage provided to the factory. Within each RunStage, no order
///  is guaranteed.
[[nodiscard]] std::vector<ProcessManager> make_process_managers(
    const ValidatedProcessManagerInputVector& aValidatedProcessManagerInput);

}  // namespace plato::process_manager::library

#endif
