#include <gtest/gtest.h>

#include <filesystem>

#include "plato/process_manager/extension/SensitivityCheck.hpp"
#include "plato/process_manager/library/ProcessManagerData.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"
#include "plato/test_utilities/FilesystemTestUtility.hpp"
#include "plato/test_utilities/InputGeneration.hpp"

namespace plato::process_manager::extension::unittest
{
TEST(SensitivityCheck, CreateSensitivityCheckRun)
{
    input_parser::ParsedInput tInputDeck =
        test_utilities::create_valid_shape_geometry_example_input_with_gradient_check();
    tInputDeck.mSensitivityCheck = test_utilities::create_valid_example_sensitivity_check();
    tInputDeck.mGradientCheck = boost::none;

    const auto tValidatedInput = library::make_validated_input(tInputDeck);
    const library::ValidatedProcessManagerInputVector tAllProcessManagerInputs = tValidatedInput.processManagers();
    EXPECT_EQ(tAllProcessManagerInputs.rawInput().size(), 2u);

    const library::ProcessManagerData tProblem = library::make_process_manager_data(tValidatedInput);
    const auto tSensitivityCheck = SensitivityCheck{
        library::process_manager_input<input_parser::sensitivity_check>(tAllProcessManagerInputs.rawInput().back())};
    tSensitivityCheck.run(tProblem);
    test_utilities::test_for_existence_and_remove({tInputDeck.mSensitivityCheck.value().output_file_name.value().mName,
                                                   tInputDeck.mBrickShapeGeometry.value().mesh_name.value().mName});
}

}  // namespace plato::process_manager::extension::unittest
