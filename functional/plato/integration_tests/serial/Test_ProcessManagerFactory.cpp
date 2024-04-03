#include <gtest/gtest.h>

#include "plato/process_manager/library/ProcessManagerFactory.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"
#include "plato/test_utilities/InputGeneration.hpp"

namespace plato::integration_tests::serial
{
namespace
{
void verify_number_of_process_managers(const input_parser::ParsedInput& aParsedInput,
                                       unsigned int aNumberOfProcessManangers)
{
    const process_manager::library::ValidatedInput tInput =
        process_manager::library::make_validated_input(aParsedInput);
    const auto tProcessManagers = tInput.processManagers().rawInput();
    EXPECT_EQ(tProcessManagers.size(), aNumberOfProcessManangers);
}

}  // namespace

TEST(ProcessManagerFactory, RightNumberOfProcessManagers)
{
    auto tRawInput = test_utilities::create_valid_shape_geometry_example_input_with_gradient_check();
    verify_number_of_process_managers(tRawInput, 2u);
    tRawInput.mSensitivityCheck = test_utilities::create_valid_example_sensitivity_check();
    verify_number_of_process_managers(tRawInput, 3u);
}

}  // namespace plato::integration_tests::serial
