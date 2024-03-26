#include <gtest/gtest.h>

#include <cmath>
#include <functional>

#include "plato/core/ValidationRegistration.hpp"
#include "plato/process_manager/extension/OptimizerValidation.hpp"
#include "plato/test_utilities/InputGeneration.hpp"

namespace plato::process_manager::extension::unittest
{
TEST(OptimizerValidation, ValidateMaxIterations)
{
    input_parser::optimization_parameters tOptimizationParameters;
    EXPECT_TRUE(detail::validate_max_iterations(tOptimizationParameters).has_value());
    tOptimizationParameters.input_file_name = input_parser::FileName{"filler"};
    EXPECT_FALSE(detail::validate_max_iterations(tOptimizationParameters).has_value());

    tOptimizationParameters.max_iterations = 0;
    EXPECT_TRUE(detail::validate_max_iterations(tOptimizationParameters).has_value());

    tOptimizationParameters.input_file_name = boost::none;
    EXPECT_TRUE(detail::validate_max_iterations(tOptimizationParameters).has_value());

    tOptimizationParameters.max_iterations = 1.0;
    EXPECT_FALSE(detail::validate_max_iterations(tOptimizationParameters).has_value());
    tOptimizationParameters.max_iterations = 100.0;
    EXPECT_FALSE(detail::validate_max_iterations(tOptimizationParameters).has_value());
}

TEST(OptimizerValidation, ValidateStepTolerance)
{
    input_parser::optimization_parameters tOptimizationParameters;
    EXPECT_TRUE(detail::validate_step_tolerance(tOptimizationParameters).has_value());  // Empty
    tOptimizationParameters.input_file_name = input_parser::FileName{"filler"};
    EXPECT_FALSE(
        detail::validate_step_tolerance(tOptimizationParameters).has_value());  // external file trumps missing others

    tOptimizationParameters.step_tolerance = -1.0;
    EXPECT_TRUE(detail::validate_step_tolerance(tOptimizationParameters)
                    .has_value());  // external file will be overwritten by bad entry
    tOptimizationParameters.input_file_name = boost::none;
    EXPECT_TRUE(detail::validate_step_tolerance(tOptimizationParameters).has_value());  // bad entry

    tOptimizationParameters.step_tolerance = std::nextafter(0.0, 1.0);
    EXPECT_FALSE(detail::validate_step_tolerance(tOptimizationParameters).has_value());  // good entry
    tOptimizationParameters.step_tolerance = 1e-8;
    EXPECT_FALSE(detail::validate_step_tolerance(tOptimizationParameters).has_value());
}

TEST(OptimizerValidation, ValidateGradientTolerance)
{
    input_parser::optimization_parameters tOptimizationParameters;
    EXPECT_TRUE(detail::validate_gradient_tolerance(tOptimizationParameters).has_value());
    tOptimizationParameters.input_file_name = input_parser::FileName{"filler"};
    EXPECT_FALSE(detail::validate_gradient_tolerance(tOptimizationParameters).has_value());

    tOptimizationParameters.gradient_tolerance = -1.0;
    EXPECT_TRUE(detail::validate_gradient_tolerance(tOptimizationParameters).has_value());

    tOptimizationParameters.input_file_name = boost::none;
    EXPECT_TRUE(detail::validate_gradient_tolerance(tOptimizationParameters).has_value());

    tOptimizationParameters.gradient_tolerance = std::nextafter(0.0, 1.0);
    EXPECT_FALSE(detail::validate_gradient_tolerance(tOptimizationParameters).has_value());
    tOptimizationParameters.gradient_tolerance = 1e-8;
    EXPECT_FALSE(detail::validate_gradient_tolerance(tOptimizationParameters).has_value());
}

TEST(OptimizerValidation, ErrorMessagesValidOptimizationParameters)
{
    input_parser::optimization_parameters tOptimizationParameters =
        plato::test_utilities::create_valid_example_optimization_parameters();

    std::vector<std::string> tMessages;
    tMessages = validate_optimization_parameters(tOptimizationParameters, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 0u);
}

TEST(OptimizerValidation, ErrorMessagesInvalidOptimizationParameters)
{
    input_parser::optimization_parameters tOptimizationParameters =
        plato::test_utilities::create_valid_example_optimization_parameters();
    tOptimizationParameters.gradient_tolerance = -1;
    tOptimizationParameters.max_iterations = 0;
    tOptimizationParameters.step_tolerance = boost::none;

    std::vector<std::string> tMessages;
    tMessages = core::validate(tOptimizationParameters, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 3u);
}
}  // namespace plato::process_manager::extension::unittest
