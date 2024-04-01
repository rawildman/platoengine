#include <gtest/gtest.h>

#include "plato/core/ValidationUtilities.hpp"
#include "plato/process_manager/extension/GradientCheck.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"
#include "plato/test_utilities/InputGeneration.hpp"
#include "plato/utilities/Exception.hpp"

namespace plato::process_manager::extension::unittest
{
namespace
{
std::vector<std::string> validate_gradient_check(const input_parser::gradient_check& aInput,
                                                 std::vector<std::string>&& aCurrentMessageList)
{
    return core::validate(aInput, std::move(aCurrentMessageList));
}

}  // namespace

TEST(ValidateGradientCheck, ValidateNumberOfSteps)
{
    input_parser::gradient_check tGradientCheck;
    EXPECT_TRUE(detail::validate_number_of_steps(tGradientCheck).has_value());
    tGradientCheck.number_of_steps = 0;  // out of bounds
    EXPECT_TRUE(detail::validate_number_of_steps(tGradientCheck).has_value());
    tGradientCheck.number_of_steps = 10;  // in bounds
    EXPECT_FALSE(detail::validate_number_of_steps(tGradientCheck).has_value());
}

TEST(ValidateGradientCheck, ValidateInitialDirectionMagnitude)
{
    input_parser::gradient_check tGradientCheck;
    EXPECT_TRUE(detail::validate_initial_direction_magnitude(tGradientCheck).has_value());
    tGradientCheck.initial_direction_magnitude = 0;  // out of bounds
    EXPECT_TRUE(detail::validate_initial_direction_magnitude(tGradientCheck).has_value());
    tGradientCheck.initial_direction_magnitude = 10;  // in bounds
    EXPECT_FALSE(detail::validate_initial_direction_magnitude(tGradientCheck).has_value());
}

TEST(ValidateGradientCheck, ValidateStepSizeReductionFactor)
{
    input_parser::gradient_check tGradientCheck;
    EXPECT_TRUE(detail::validate_step_size_reduction_factor(tGradientCheck).has_value());
    tGradientCheck.step_size_reduction_factor = 0;  // out of bounds
    EXPECT_TRUE(detail::validate_step_size_reduction_factor(tGradientCheck).has_value());
    tGradientCheck.step_size_reduction_factor = 0.5;  // in bounds
    EXPECT_FALSE(detail::validate_step_size_reduction_factor(tGradientCheck).has_value());
    tGradientCheck.step_size_reduction_factor = 10;  // out of bounds
    EXPECT_TRUE(detail::validate_step_size_reduction_factor(tGradientCheck).has_value());
}

TEST(ValidateGradientCheck, ValidateRandomDirectionSeed)
{
    input_parser::gradient_check tGradientCheck;
    EXPECT_TRUE(detail::validate_random_direction_seed(tGradientCheck).has_value());
    tGradientCheck.random_direction_seed = 0;  // out of bounds
    EXPECT_TRUE(detail::validate_random_direction_seed(tGradientCheck).has_value());
}

TEST(ValidateGradientCheck, ValidateOuputFileName)
{
    auto tGradientCheck = input_parser::gradient_check{};
    EXPECT_TRUE(detail::validate_output_file_name(tGradientCheck).has_value());
    tGradientCheck.output_file_name = input_parser::FileName{"file.txt"};  // Requires an input
    EXPECT_FALSE(detail::validate_output_file_name(tGradientCheck).has_value());
}

TEST(ValidateGradientCheck, NoErrorMessagesValidGradientCheck)
{
    const input_parser::gradient_check tGradientCheck = plato::test_utilities::create_valid_example_gradient_check();
    const auto tMessages = validate_gradient_check(tGradientCheck, std::vector<std::string>{});
    EXPECT_TRUE(tMessages.empty());
}

TEST(ValidateGradientCheck, ErrorMessagesInvalidGradientCheck)
{
    const input_parser::gradient_check tGradientCheck;
    const auto tMessages = validate_gradient_check(tGradientCheck, std::vector<std::string>{});
    const auto tNumberOfGradientCheckValidationFunctions =
        core::detail::registered_validation_functions<input_parser::gradient_check>().size();
    EXPECT_EQ(tMessages.size(), tNumberOfGradientCheckValidationFunctions);
}

}  // namespace plato::process_manager::extension::unittest