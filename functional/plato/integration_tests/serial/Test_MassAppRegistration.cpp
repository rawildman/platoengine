#include <gtest/gtest.h>

#include <filesystem>

#include "plato/criteria/extension/PluginCriteria.hpp"
#include "plato/criteria/library/ObjectiveFactory.hpp"
#include "plato/geometry/extension/BrickShapeGeometry.hpp"
#include "plato/input_parser/InputBlockUtilities.hpp"
#include "plato/input_parser/InputBlocks.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"
#include "plato/services/AppConfiguration.hpp"
#include "plato/test_utilities/InputGeneration.hpp"

namespace plato::integration_tests::serial
{
TEST(MassAppRegistration, RegisterLoadAndRun)
{
    const auto tTestDirectory = std::filesystem::path{"test-plugin-directory"};
    std::filesystem::create_directories(tTestDirectory);

    constexpr auto tAppName = std::string_view{"test-mass-app"};
    const auto tAppConfiguration = services::AppConfiguration{
        /*.mName=*/std::string{tAppName}, /*.mLibraryFileName=*/"../libPlatoTestMassObjective.so",
        /*.mHasParallelImplementation=*/true, /*.mHasSerialImplementation=*/true};
    services::save_configuration(tAppConfiguration, tTestDirectory / "test-mass-app.config");

    const auto tNumRegistered = criteria::extension::register_plugin_apps({tTestDirectory});

    EXPECT_EQ(tNumRegistered, 1u);

    auto tObjective = input_parser::objective{};
    tObjective.number_of_processors = 1;
    tObjective.aggregation_weight = 1.0;
    tObjective.app = input_parser::AppName{std::string{tAppName}};
    tObjective.name = "test_1";

    const auto tInput = tObjective | test_utilities::create_valid_brick_shape_geometry() |
                        test_utilities::create_valid_example_rol_optimization();
    const auto tValidInput = process_manager::library::make_validated_input(tInput);

    const auto tObjectiveFunction = criteria::library::make_aggregate_objective_function(tValidInput.objectives());
    const auto tGeometry =
        geometry::extension::make_brick_shape_geometry(geometry::extension::BrickShapeGeometry{"brick.exo"});

    constexpr auto tX = double{2.0};
    constexpr auto tY = double{4.0};
    constexpr auto tZ = double{6.0};
    constexpr auto tCenterCoordinate = double{0.0};
    const auto tControls =
        linear_algebra::DynamicVector<double>{tCenterCoordinate, tCenterCoordinate, tCenterCoordinate, tX, tY, tZ};

    constexpr auto tExpectedValue = tX * tY * tZ;
    const auto tResult = tObjectiveFunction.f(tGeometry.f(tControls));
    EXPECT_EQ(tResult, tExpectedValue);

    std::filesystem::remove_all(tTestDirectory);
}
}  // namespace plato::integration_tests::serial