#include "plato/process_manager/extension/ROLOptimization.hpp"

#include <fstream>
#include <string_view>

#include "plato/process_manager/library/ProcessManagerData.hpp"
#include "plato/process_manager/library/ProcessManagerRegistration.hpp"
#include "plato/rol_integration/OptimizerFactory.hpp"

namespace plato::process_manager::extension
{
namespace
{
constexpr std::string_view kROLOptimizerFileName = "ROL_Optimizer.txt";

[[nodiscard]] library::ProcessManager make_rol_optimization_process_manager(
    const library::ValidatedProcessManagerInput& aValidInput)

{
    return [aValidInput](const library::ProcessManagerData& aProcessManangerData)
    {
        const auto& tInput = library::process_manager_input<input_parser::optimization_parameters>(aValidInput);
        ROLOptimization{tInput}.run(aProcessManangerData);
    };
}

[[maybe_unused]] static auto kROLOptimizerProcessManagerRegistration =
    library::ProcessManagerRegistration{input_parser::block_name<input_parser::optimization_parameters>(),
                                        [](const library::ValidatedProcessManagerInput& aValidInput)
                                        { return make_rol_optimization_process_manager(aValidInput); }};
}  // namespace

ROLOptimization::ROLOptimization(const ValidatedOptimizationParameters& aInput)
    : mROLOptions{rol_integration::rol_parameter_list(aInput)}
{
}

void ROLOptimization::run(const library::ProcessManagerData& aProblem) const
{
    auto tROLProblem = ROL::Ptr<ROL::Problem<double>>{library::make_rol_problem(aProblem).release()};
    auto tROLInputs = aProblem.mROLOptions;
    auto tROLSolver = rol_integration::make_rol_solver(tROLInputs, tROLProblem);

    auto tOutFile = std::ofstream{std::string{kROLOptimizerFileName}};
    tROLSolver.solve(tOutFile);
}

}  // namespace plato::process_manager::extension