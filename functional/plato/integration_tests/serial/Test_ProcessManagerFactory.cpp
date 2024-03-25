#include <gtest/gtest.h>

#include "plato/process_manager/library/ProcessManagerFactory.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"
#include "plato/test_utilities/InputGeneration.hpp"

namespace plato::integration_tests::serial
{
TEST(ProcessManagerFactory, Optimization)
{
    namespace pftu = plato::test_utilities;

    const auto tRawInput =
        input_parser::ParsedInput{/*.mObjectives=*/{pftu::create_valid_example_objective()},
                                  /*.mConstraints=*/{pftu::create_valid_example_constraint()},
                                  /*.mBrickShapeGeometry=*/pftu::create_valid_brick_shape_geometry(),
                                  /*.mDensityTopology = */ boost::none,
                                  /*.mOptimizationParameters = */ pftu::create_valid_example_optimization_parameters(),
                                  /*.mGradientCheck = */ boost::none};

    const process_manager::library::ValidatedInput tInput = process_manager::library::make_validated_input(tRawInput);
    // TODO: Implement me
//    const auto tProcessManagers = process_manager::library::make_process_managers(tInput.optimizationParameters());

//    EXPECT_EQ(tProcessManagers.size(), 1);
//    EXPECT_TRUE(tProcessManagers.front());
}

}  // namespace plato::integration_tests::serial
