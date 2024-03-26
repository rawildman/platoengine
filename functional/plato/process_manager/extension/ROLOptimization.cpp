#include "plato/process_manager/extension/ROLOptimization.hpp"

#include "plato/process_manager/library/ProcessManagerRegistration.hpp"

namespace plato::process_manager::extension
{
namespace
{
[[nodiscard]] library::ProcessManager make_rol_optimization_process_manager(
    const library::ValidatedProcessManagerInput& aValidInput)

{
    return [aValidInput](const library::ProcessManagerData& aProcessManangerData)
    {
        const auto& tInput = library::process_manager_raw_input<input_parser::optimization_parameters>(aValidInput);
        const ROLOptimization tROLOptimization(tInput);
        tROLOptimization.run(aProcessManangerData);
    };
}

[[maybe_unused]] static auto kROLOptimizerProcessManagerRegistration =
    library::ProcessManagerRegistration{input_parser::block_name<input_parser::optimization_parameters>(),
                                        [](const library::ValidatedProcessManagerInput& aValidInput)
                                        { return make_rol_optimization_process_manager(aValidInput); }};
}  // namespace

ROLOptimization::ROLOptimization(const input_parser::optimization_parameters&) {}

void ROLOptimization::run(const library::ProcessManagerData&) const
{
    std::cout << " This is me running an optimization problem.... beep boop beep beep ." << std::endl;
}

}  // namespace plato::process_manager::extension