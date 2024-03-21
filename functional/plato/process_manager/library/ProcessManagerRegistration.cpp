#include "plato/process_manager/library/ProcessManagerRegistration.hpp"

#include "plato/core/InputVariantUtilities.hpp"

namespace plato::process_manager::library
{

[[nodiscard]] std::vector<ProcessManagerInput> process_manager_input(const input_parser::ParsedInput& aInput)
{
    return core::all_input_blocks_in_variant<ProcessManagerInput>(aInput);
}

[[nodiscard]] bool is_process_manager_function_registered(const std::string_view aFunctionName)
{
    return core::is_factory_function_registered<ProcessManager, ValidatedProcessManagerInput>(aFunctionName);
}

}  // namespace plato::process_manager::library
