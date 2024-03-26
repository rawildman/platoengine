#include <gtest/gtest.h>

#include <filesystem>

#include "plato/process_manager/extension/ROLOptimization.hpp"
#include "plato/process_manager/library/ProcessManagerData.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"
#include "plato/test_utilities/InputGeneration.hpp"

namespace plato::process_manager::extension::unittest
{
namespace 
{
constexpr std::string_view kROLOptimizerFileName = "ROL_Optimizer.txt";
}

TEST(ROLOptimization, Create) {
     const input_parser::ParsedInput tInputDeck =
        test_utilities::create_valid_shape_geometry_example_input_with_gradient_check();
    const auto tValidatedInput = library::make_validated_input(tInputDeck);
    const library::ProcessManagerData tProblem = library::make_process_manager_data(tValidatedInput);
    const auto tROLOptimization = ROLOptimization{tInputDeck.mGradientCheck.value()};
    tROLOptimization.run(tProblem);
    EXPECT_TRUE(std::filesystem::exists(kROLOptimizerFileName));
    EXPECT_TRUE(std::filesystem::remove(kROLOptimizerFileName));
}

}  // namespace plato::process_manager::extension::unittest