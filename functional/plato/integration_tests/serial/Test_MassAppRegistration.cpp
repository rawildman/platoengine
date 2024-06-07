#include <gtest/gtest.h>

#include <filesystem>

#include "plato/criteria/library/ObjectiveFactory.hpp"
#include "plato/geometry/extension/BrickShapeGeometry.hpp"
#include "plato/integration_tests/utilities/MassAppTestUtilities.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"

namespace plato::integration_tests::serial
{
namespace
{
}  // namespace

TEST(MassAppRegistration, RegisterLoadAndRun)
{
    constexpr auto tMassAppLibName = std::string_view{"libPlatoTestMassObjective.so"};
    ASSERT_TRUE(std::filesystem::exists(tMassAppLibName));

    const auto tAppName = input_parser::AppName{"test-mass-app"};
    auto tConfigurationTempDirectory = integration_tests::utilities::register_test_mass_app(tAppName.mToken);
    constexpr auto tNumProcessors = 1u;
    const auto tCriterionName = input_parser::CriterionName{"mass"};
    const auto tValidInput =
        integration_tests::utilities::create_valid_brick_input(tAppName, tCriterionName, tNumProcessors);

    const auto tObjectiveFunction = criteria::library::make_aggregate_objective_function(tValidInput.objectives());
    const auto tGeometry =
        geometry::extension::make_brick_shape_geometry(geometry::extension::BrickShapeGeometry{"brick.exo"});

    const auto [tControls, tExpectedValue] = integration_tests::utilities::brick_shape_geometry_controls_with_volume();
    const auto tResult = tObjectiveFunction.f(tGeometry.f(tControls));
    EXPECT_DOUBLE_EQ(tResult, tExpectedValue);
}
}  // namespace plato::integration_tests::serial