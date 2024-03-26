#include "plato/process_manager/library/ProcessManagerFactory.hpp"

#include "plato/core/InputVariantUtilities.hpp"

namespace plato::process_manager::library
{

[[nodiscard]] std::vector<ProcessManager> make_process_managers(
    const ValidatedProcessManagerInputVector& aValidatedProcessManagerInput)
{
    std::vector<ProcessManager> tProcessManagers;
    const auto& tRawInputVector = aValidatedProcessManagerInput.rawInput();

    for (auto& tValidatedProcessInput : tRawInputVector)
    {
        const auto tProcessName = core::block_name(tValidatedProcessInput);
        auto tProcess = core::create_object_from_factory<ProcessManager, ValidatedProcessManagerInput>(
            tProcessName, tValidatedProcessInput);
        if (tProcess.has_value())
        {
            tProcessManagers.push_back(std::move(tProcess).value());
        }
    }
    return tProcessManagers;
}

}  // namespace plato::process_manager::library
