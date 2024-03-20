#include "plato/main/library/GradientCheckValidation.hpp"

#include "plato/core/ValidationRegistration.hpp"
#include "plato/core/ValidationUtilities.hpp"
#include "plato/input_parser/InputBlocks.hpp"

namespace plato::main::library
{
[[maybe_unused]] static auto kGradientCheckValidationRegistration =
    core::ValidationRegistration<input_parser::gradient_check>{
        [](const input_parser::gradient_check& aInput) { return detail::validate_output_file_name(aInput); },
        [](const input_parser::gradient_check& aInput) { return detail::validate_number_of_steps(aInput); },
        [](const input_parser::gradient_check& aInput) { return detail::validate_initial_direction_magnitude(aInput); },
        [](const input_parser::gradient_check& aInput) { return detail::validate_step_size_reduction_factor(aInput); },
        [](const input_parser::gradient_check& aInput) { return detail::validate_random_direction_seed(aInput); }};

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

}  // namespace plato::main::library