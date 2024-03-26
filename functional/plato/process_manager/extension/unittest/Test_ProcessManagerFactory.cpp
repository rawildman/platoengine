#include <gtest/gtest.h>

#include "plato/process_manager/library/ProcessManagerFactory.hpp"
#include "plato/process_manager/library/ProcessManagerRegistration.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"
#include "plato/test_utilities/InputGeneration.hpp"

namespace plato::process_manager::library::unittest
{

TEST(ProcessManagerFactory, ValidOptimization)
{
    const input_parser::ParsedInput tInput = test_utilities::create_valid_example_input();
    const ValidatedInput tData = make_validated_input(tInput);
    const auto tProcesses = make_process_managers(tData.processManagers());
    EXPECT_EQ(tProcesses.size(), 0u);  // until optimization problem becomes a process manager
}

TEST(ProcessManagerFactory, ValidOptimizationAndGradientCheck)
{
    const input_parser::ParsedInput tInput = test_utilities::create_valid_example_input_with_gradient_check();
    const ValidatedInput tData = make_validated_input(tInput);
    const auto tProcesses = make_process_managers(tData.processManagers());
    EXPECT_EQ(tProcesses.size(), 1u);  // until optimization problem becomes a process manager
}

}  // namespace plato::process_manager::library::unittest