#ifndef PLATO_PROCESSMANAGER_LIBRARY_PROCESSMANAGERREGISTRATION
#define PLATO_PROCESSMANAGER_LIBRARY_PROCESSMANAGERREGISTRATION

#include "plato/core/FactoryRegistration.hpp"
#include "plato/core/VariantInputBuilder.hpp"
#include "plato/input_parser/InputBlocks.hpp"

namespace plato::process_manager::library
{

struct ProcessManagerData;

using ProcessManager = std::function<void(const ProcessManagerData&)>;
using ProcessManagerInput = core::InputVariant<input_parser::ParsedInput, input_parser::IsProcessManagerInput>;

using ValidatedProcessManagerInputVector = core::ValidatedInputTypeWrapper<
    std::vector<core::ValidatedInputVariant<input_parser::ParsedInput, input_parser::IsProcessManagerInput>>>;
using ValidatedProcessManagerInput = typename ValidatedProcessManagerInputVector::RawInputType::value_type;

using ProcessManagerRegistration = core::FactoryRegistration<ProcessManager, ValidatedProcessManagerInput>;

[[nodiscard]] std::vector<ProcessManagerInput> process_manager_input(const input_parser::ParsedInput& aInput);

[[nodiscard]] bool is_process_manager_function_registered(const std::string_view aFunctionName);

/// @brief Helper to get the raw input from a validated process_manager variant.
template <typename ProcessManagerRawInputType>
[[nodiscard]] const ProcessManagerRawInputType& process_manager_raw_input(
    const ValidatedProcessManagerInput& aValidatedInput)
{
    return std::get<core::ValidatedInputTypeWrapper<ProcessManagerRawInputType>>(aValidatedInput).rawInput();
}

}  // namespace plato::process_manager::library
#endif