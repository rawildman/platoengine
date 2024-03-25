#ifndef PLATO_PROCESSMANAGER_LIBRARY_PROCESSMANAGERREGISTRATION
#define PLATO_PROCESSMANAGER_LIBRARY_PROCESSMANAGERREGISTRATION

#include "plato/core/FactoryRegistration.hpp"
#include "plato/core/VariantInputBuilder.hpp"
#include "plato/input_parser/InputBlocks.hpp"

namespace plato::process_manager::library
{

struct PlatoProblem;

using ProcessManager = std::function<void(const PlatoProblem&)>;

using ProcessManagerInput = core::InputVariant<input_parser::ParsedInput, input_parser::IsProcessManagerInput>;
using ValidatedProcessManagerInput = core::ValidatedInputTypeWrapper<
    std::vector<core::ValidatedInputVariant<input_parser::ParsedInput, input_parser::IsProcessManagerInput>>>;
using ProcessManagerRegistration = core::FactoryRegistration<ProcessManager, ValidatedProcessManagerInput>;

[[nodiscard]] std::vector<ProcessManagerInput> process_manager_input(const input_parser::ParsedInput& aInput);

[[nodiscard]] bool is_process_manager_function_registered(const std::string_view aFunctionName);

}  // namespace plato::process_manager::library
#endif