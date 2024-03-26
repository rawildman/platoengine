#include <gtest/gtest.h>

#include "plato/process_manager/extension/GradientCheck.hpp"
#include "plato/process_manager/library/ProcessManagerData.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"
#include "plato/test_utilities/InputGeneration.hpp"

namespace plato::process_manager::extension::unittest
{

TEST(GradientCheck, CreateGradientCheckRun)
{
    const input_parser::ParsedInput tInputDeck =
        test_utilities::create_valid_shape_geometry_example_input_with_gradient_check();
    const auto tValidatedInput = library::make_validated_input(tInputDeck);
    const library::ProcessManagerData tProblem = library::make_process_manager_data(tValidatedInput);
    const auto tGradientCheck = GradientCheck{tInputDeck.mGradientCheck.value()};
    tGradientCheck.run(tProblem);
}

TEST(GradientCheck, UnwrapValidatedGradientCheckInput)
{
    const input_parser::ParsedInput tInputDeck =
        test_utilities::create_valid_shape_geometry_example_input_with_gradient_check();

    const auto tValidatedInput = library::make_validated_input(tInputDeck);
    const auto tUnwrappedValidatedInput = tValidatedInput.processManagers().rawInput();
    EXPECT_EQ(tUnwrappedValidatedInput.size(), 2u);
}

}  // namespace plato::process_manager::extension::unittest