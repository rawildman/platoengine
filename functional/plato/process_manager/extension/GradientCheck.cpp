#include "plato/process_manager/extension/GradientCheck.hpp"

#include <cstdlib>
#include <fstream>

#include "plato/process_manager/extension/LogspaceGenerator.hpp"
#include "plato/process_manager/extension/ROLUtilities.hpp"
#include "plato/process_manager/library/ProcessManagerData.hpp"
#include "plato/process_manager/library/ProcessManagerRegistration.hpp"
#include "plato/rol_integration/ROLHelpers.hpp"

namespace plato::process_manager::extension
{
namespace
{
[[nodiscard]] library::ProcessManager make_gradient_check_process_manager(
    const library::ValidatedProcessManagerInput& aValidInput)

{
    return [aValidInput](const library::ProcessManagerData& aProcessManangerData)
    {
        const auto& tInput = library::process_manager_input<input_parser::gradient_check>(aValidInput);
        GradientCheck{tInput}.run(aProcessManangerData);
    };
}

[[maybe_unused]] static auto kGradientCheckProcessManagerRegistration =
    library::ProcessManagerRegistration{input_parser::block_name<input_parser::gradient_check>(),
                                        [](const library::ValidatedProcessManagerInput& aValidInput)
                                        { return make_gradient_check_process_manager(aValidInput); }};

[[maybe_unused]] static auto kGradientCheckValidationRegistration =
    core::ValidationRegistration<input_parser::gradient_check>{
        [](const input_parser::gradient_check& aInput) { return detail::validate_output_file_name(aInput); },
        [](const input_parser::gradient_check& aInput) { return detail::validate_number_of_steps(aInput); },
        [](const input_parser::gradient_check& aInput) { return detail::validate_initial_direction_magnitude(aInput); },
        [](const input_parser::gradient_check& aInput) { return detail::validate_step_size_reduction_factor(aInput); },
        [](const input_parser::gradient_check& aInput) { return detail::validate_random_direction_seed(aInput); }};

}  // namespace

GradientCheck::GradientCheck(const ValidatedGradientCheckInput& aInput)
    : mOutputFileName(aInput.rawInput().output_file_name.value().mName),
      mNumberOfSteps(aInput.rawInput().number_of_steps.value()),
      mInitialDirectionMagnitude(aInput.rawInput().initial_direction_magnitude.value()),
      mStepSizeReductionFactor(aInput.rawInput().step_size_reduction_factor.value()),
      mRandomDirectionSeed(aInput.rawInput().random_direction_seed.value())
{
}

void GradientCheck::run(const library::ProcessManagerData& aProblem) const
{
    std::ofstream tOutFile(mOutputFileName);
    constexpr bool tPrintOutput = true;
    auto tROLProblem = ROL::Ptr<ROL::Problem<double>>{make_rol_problem(aProblem).release()};
    const LogspaceGenerator tLogspaceGenerator{mInitialDirectionMagnitude, mStepSizeReductionFactor, mNumberOfSteps};

    std::srand(mRandomDirectionSeed);
    tROLProblem->getObjective()->checkGradient(
        rol_integration::to_rol_vector(aProblem.mGeometry.mInitialGuess),
        rol_integration::generate_perturbation(aProblem.mGeometry.mInitialGuess.size()), tLogspaceGenerator.steps(),
        tPrintOutput, tOutFile);
}

std::vector<std::string> validate_gradient_check(const input_parser::gradient_check& aInput,
                                                 std::vector<std::string>&& aCurrentMessageList)
{
    return core::validate(aInput, std::move(aCurrentMessageList));
}

namespace detail
{
std::optional<std::string> validate_output_file_name(const input_parser::gradient_check& aInput)
{
    return core::error_message_for_empty_parameter(input_parser::block_name<input_parser::gradient_check>(),
                                                   aInput.output_file_name, "output_file_name");
}

std::optional<std::string> validate_number_of_steps(const input_parser::gradient_check& aInput)
{
    namespace pfu = plato::utilities;
    return core::error_message_for_parameter_out_of_bounds(input_parser::block_name<input_parser::gradient_check>(),
                                                           aInput.number_of_steps, "number_of_steps",
                                                           pfu::lower_bounded(pfu::Inclusive{1u}));
}

std::optional<std::string> validate_initial_direction_magnitude(const input_parser::gradient_check& aInput)
{
    namespace pfu = plato::utilities;
    return core::error_message_for_parameter_out_of_bounds(
        input_parser::block_name<input_parser::gradient_check>(), aInput.initial_direction_magnitude,
        "initial_direction_magnitude", pfu::lower_bounded(pfu::Exclusive{0.0}));
}

std::optional<std::string> validate_step_size_reduction_factor(const input_parser::gradient_check& aInput)
{
    namespace pfu = plato::utilities;
    return core::error_message_for_parameter_out_of_bounds(
        input_parser::block_name<input_parser::gradient_check>(), aInput.step_size_reduction_factor,
        "step_size_reduction_factor", pfu::ParameterBounds{pfu::Exclusive{0.0}, pfu::Exclusive{1.0}});
}

std::optional<std::string> validate_random_direction_seed(const input_parser::gradient_check& aInput)
{
    namespace pfu = plato::utilities;
    return core::error_message_for_parameter_out_of_bounds(input_parser::block_name<input_parser::gradient_check>(),
                                                           aInput.random_direction_seed, "random_direction_seed",
                                                           pfu::lower_bounded(pfu::Inclusive{1u}));
}

}  // namespace detail

}  // namespace plato::process_manager::extension
