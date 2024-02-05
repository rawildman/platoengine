#include <gtest/gtest.h>

#include <cmath>
#include <functional>

#include "InputGeneration.hpp"
#include "OptimizerValidation.hpp"
#include "ValidationRegistration.hpp"

namespace plato::functional::optimizer::unittest
{
TEST(OptimizerValidation, ValidateMaxIterations)
{
    namespace pfod = plato::functional::optimizer::detail;
    Plato::optimization_parameters tOptimizationParameters;
    EXPECT_TRUE(pfod::validate_max_iterations(tOptimizationParameters).has_value());
    tOptimizationParameters.input_file_name = Plato::FileName{"filler"};
    EXPECT_FALSE(pfod::validate_max_iterations(tOptimizationParameters).has_value());

    tOptimizationParameters.max_iterations = 0;
    EXPECT_TRUE(pfod::validate_max_iterations(tOptimizationParameters).has_value());

    tOptimizationParameters.input_file_name = boost::none;
    EXPECT_TRUE(pfod::validate_max_iterations(tOptimizationParameters).has_value());

    tOptimizationParameters.max_iterations = 1.0;
    EXPECT_FALSE(pfod::validate_max_iterations(tOptimizationParameters).has_value());
    tOptimizationParameters.max_iterations = 100.0;
    EXPECT_FALSE(pfod::validate_max_iterations(tOptimizationParameters).has_value());
}

TEST(OptimizerValidation, ValidateStepTolerance)
{
    namespace pfod = plato::functional::optimizer::detail;
    Plato::optimization_parameters tOptimizationParameters;
    EXPECT_TRUE(pfod::validate_step_tolerance(tOptimizationParameters).has_value());  // Empty
    tOptimizationParameters.input_file_name = Plato::FileName{"filler"};
    EXPECT_FALSE(
        pfod::validate_step_tolerance(tOptimizationParameters).has_value());  // external file trumps missing others

    tOptimizationParameters.step_tolerance = -1.0;
    EXPECT_TRUE(pfod::validate_step_tolerance(tOptimizationParameters)
                    .has_value());  // external file will be overwritten by bad entry
    tOptimizationParameters.input_file_name = boost::none;
    EXPECT_TRUE(pfod::validate_step_tolerance(tOptimizationParameters).has_value());  // bad entry

    tOptimizationParameters.step_tolerance = std::nextafter(0.0, 1.0);
    EXPECT_FALSE(pfod::validate_step_tolerance(tOptimizationParameters).has_value());  // good entry
    tOptimizationParameters.step_tolerance = 1e-8;
    EXPECT_FALSE(pfod::validate_step_tolerance(tOptimizationParameters).has_value());
}

TEST(OptimizerValidation, ValidateGradientTolerance)
{
    namespace pfod = plato::functional::optimizer::detail;
    Plato::optimization_parameters tOptimizationParameters;
    EXPECT_TRUE(pfod::validate_gradient_tolerance(tOptimizationParameters).has_value());
    tOptimizationParameters.input_file_name = Plato::FileName{"filler"};
    EXPECT_FALSE(pfod::validate_gradient_tolerance(tOptimizationParameters).has_value());

    tOptimizationParameters.gradient_tolerance = -1.0;
    EXPECT_TRUE(pfod::validate_gradient_tolerance(tOptimizationParameters).has_value());

    tOptimizationParameters.input_file_name = boost::none;
    EXPECT_TRUE(pfod::validate_gradient_tolerance(tOptimizationParameters).has_value());

    tOptimizationParameters.gradient_tolerance = std::nextafter(0.0, 1.0);
    EXPECT_FALSE(pfod::validate_gradient_tolerance(tOptimizationParameters).has_value());
    tOptimizationParameters.gradient_tolerance = 1e-8;
    EXPECT_FALSE(pfod::validate_gradient_tolerance(tOptimizationParameters).has_value());
}

TEST(OptimizerValidation, ErrorMessagesValidOptimizationParameters)
{
    Plato::optimization_parameters tOptimizationParameters =
        Plato::Functional::TestUtilities::create_valid_example_optimization_parameters();

    std::vector<std::string> tMessages;
    tMessages = validate_optimization_parameters(tOptimizationParameters, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 0u);
}

TEST(OptimizerValidation, ErrorMessagesInvalidOptimizationParameters)
{
    Plato::optimization_parameters tOptimizationParameters =
        Plato::Functional::TestUtilities::create_valid_example_optimization_parameters();
    tOptimizationParameters.gradient_tolerance = -1;
    tOptimizationParameters.max_iterations = 0;
    tOptimizationParameters.step_tolerance = boost::none;

    namespace pfv = Plato::Functional::Validation;
    std::vector<std::string> tMessages;
    tMessages = pfv::validate(tOptimizationParameters, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 3u);
}
}  // namespace plato::functional::optimizer
