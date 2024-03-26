#include <gtest/gtest.h>

#include "plato/core/ValidationUtilities.hpp"
#include "plato/process_manager/extension/GradientCheck.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"
#include "plato/test_utilities/InputGeneration.hpp"
#include "plato/utilities/Exception.hpp"

namespace plato::process_manager::extension::unittest
{

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
    tGradientCheck.initial_direction_magnitude = 0;  // out of bounds
    EXPECT_TRUE(detail::validate_step_size_reduction_factor(tGradientCheck).has_value());
    tGradientCheck.initial_direction_magnitude = 0.5;  // in bounds
    EXPECT_TRUE(detail::validate_step_size_reduction_factor(tGradientCheck).has_value());
    tGradientCheck.initial_direction_magnitude = 10;  // out of bounds
    EXPECT_TRUE(detail::validate_step_size_reduction_factor(tGradientCheck).has_value());
}

TEST(ValidateGradientCheck, ValidateRandomDirectionSeed)
{
    input_parser::gradient_check tGradientCheck;
    EXPECT_TRUE(detail::validate_random_direction_seed(tGradientCheck).has_value());
    tGradientCheck.random_direction_seed = 0;  // out of bounds
    EXPECT_TRUE(detail::validate_random_direction_seed(tGradientCheck).has_value());
}

TEST(ValidateGradientCheck, NoErrorMessagesValidGradientCheck)
{
    input_parser::gradient_check tGradientCheck = plato::test_utilities::create_valid_example_gradient_check();

    std::vector<std::string> tMessages;
    tMessages = validate_gradient_check(tGradientCheck, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 0u);
}

TEST(ValidateGradientCheck, ErrorMessagesInvalidGradientCheck)
{
    input_parser::gradient_check tGradientCheck;

    std::vector<std::string> tMessages;
    tMessages = validate_gradient_check(tGradientCheck, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 5u);
}

}  // namespace plato::process_manager::extension::unittest