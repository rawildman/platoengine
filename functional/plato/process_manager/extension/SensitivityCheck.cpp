#include "plato/process_manager/extension/SensitivityCheck.hpp"

#include <fstream>
#include <string_view>

#include "plato/core/ValidationUtilities.hpp"
#include "plato/input_parser/InputBlocks.hpp"
#include "plato/process_manager/library/ProcessManagerData.hpp"
#include "plato/process_manager/library/ProcessManagerRegistration.hpp"
#include "plato/rol_integration/OptimizerFactory.hpp"
#include "plato/rol_integration/ROLHelpers.hpp"

namespace plato::process_manager::extension
{
namespace
{

[[nodiscard]] library::ProcessManager make_rol_sensitivity_check_process_manager(
    const library::ValidatedProcessManagerInput& aValidInput)

{
    return [aValidInput](const library::ProcessManagerData& aProcessManangerData)
    {
        const auto& tInput = library::process_manager_raw_input<input_parser::sensitivity_check>(aValidInput);
        const SensitivityCheck tSensitivityCheck(tInput);
        tSensitivityCheck.run(aProcessManangerData);
    };
}

[[maybe_unused]] static auto kSensitivityCheckProcessManagerRegistration =
    library::ProcessManagerRegistration{input_parser::block_name<input_parser::sensitivity_check>(),
                                        [](const library::ValidatedProcessManagerInput& aValidInput)
                                        { return make_rol_sensitivity_check_process_manager(aValidInput); }};

[[maybe_unused]] static auto kSensitivityCheckValidationRegistration =
    core::ValidationRegistration<input_parser::sensitivity_check>{
        [](const input_parser::sensitivity_check& aInput) { return detail::validate_output_file_name(aInput); }};
}  // namespace

SensitivityCheck::SensitivityCheck(const input_parser::sensitivity_check& aInput)
    : mOutputFileName(aInput.output_file_name.value().mName)
{
}

void SensitivityCheck::run(const library::ProcessManagerData& aProblem) const
{
    std::ofstream tOutFile(mOutputFileName);
    constexpr bool tPrintOutput = true;

    auto tSensitivityObjective = process_manager::library::make_rol_sensitivity_objective(aProblem);
    tSensitivityObjective->checkGradient(
        rol_integration::to_rol_vector(aProblem.mGeometry.mInitialGuess),
        rol_integration::generate_perturbation(aProblem.mGeometry.mInitialGuess.size()), tPrintOutput, tOutFile);
}

std::vector<std::string> validate_sensitivity_check(const input_parser::sensitivity_check& aInput,
                                                    std::vector<std::string>&& aCurrentMessageList)
{
    return core::validate(aInput, std::move(aCurrentMessageList));
}

namespace detail
{
std::optional<std::string> validate_output_file_name(const input_parser::sensitivity_check& aInput)
{
    return core::error_message_for_empty_parameter(input_parser::block_name<input_parser::sensitivity_check>(),
                                                   aInput.output_file_name, "output_file_name");
}
}  // namespace detail
}  // namespace plato::process_manager::extension