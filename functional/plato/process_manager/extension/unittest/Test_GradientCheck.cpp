#include <gtest/gtest.h>

#include <filesystem>

#include "plato/process_manager/extension/GradientCheck.hpp"
#include "plato/process_manager/library/ProcessManagerData.hpp"
#include "plato/process_manager/library/ProcessManagerRegistration.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"
#include "plato/test_utilities/FilesystemTestUtility.hpp"
#include "plato/test_utilities/InputGeneration.hpp"

namespace plato::process_manager::extension::unittest
{
using ProcessManagerInputVector = typename library::ValidatedProcessManagerInputVector::RawInputType;
template <typename BlockType>
std::size_t num_blocks_with_type(const ProcessManagerInputVector& aAllProcessManagerInputs)
{
    return std::count_if(aAllProcessManagerInputs.cbegin(), aAllProcessManagerInputs.cend(),
                         [](const auto& aInput)
                         { return core::block_name(aInput) == input_parser::block_name<BlockType>(); });
}

TEST(GradientCheck, CreateGradientCheckRun)
{
    const input_parser::ParsedInput tInputDeck =
        test_utilities::create_valid_shape_geometry_example_input_with_gradient_check();
    const auto tValidatedInput = library::make_validated_input(tInputDeck);
    const library::ProcessManagerData tProblem = library::make_process_manager_data(tValidatedInput);
    const library::ValidatedProcessManagerInputVector tAllProcessManagerInputs = tValidatedInput.processManagers();
    ASSERT_EQ(tAllProcessManagerInputs.rawInput().size(), 2);
    const auto tGradientCheck = GradientCheck{
        library::process_manager_input<input_parser::gradient_check>(tAllProcessManagerInputs.rawInput().back())};
    tGradientCheck.run(tProblem);

    test_utilities::test_for_existence_and_delete({tInputDeck.mGradientCheck.value().output_file_name.value().mName,
                                                   tInputDeck.mBrickShapeGeometry.value().mesh_name.value().mName});
}

TEST(GradientCheck, UnwrapValidatedGradientCheckInput)
{
    const input_parser::ParsedInput tInputDeck =
        test_utilities::create_valid_shape_geometry_example_input_with_gradient_check();

    const auto tValidatedInput = library::make_validated_input(tInputDeck);
    const auto tUnwrappedValidatedInput = tValidatedInput.processManagers().rawInput();

    constexpr auto tExpectedNumGradientCheckInputs = std::size_t{1};
    EXPECT_EQ(num_blocks_with_type<input_parser::gradient_check>(tUnwrappedValidatedInput),
              tExpectedNumGradientCheckInputs);

    constexpr auto tExpectedNumROLOptimizerInputs = std::size_t{1};
    EXPECT_EQ(num_blocks_with_type<input_parser::rol_optimization>(tUnwrappedValidatedInput),
              tExpectedNumROLOptimizerInputs);

    constexpr auto tExpectedTotalProcessManagerInputs = std::size_t{2};
    EXPECT_EQ(tUnwrappedValidatedInput.size(), tExpectedTotalProcessManagerInputs);
}

}  // namespace plato::process_manager::extension::unittest