#include "plato/integration_tests/utilities/MassAppTestUtilities.hpp"

#include "plato/criteria/extension/PluginCriteria.hpp"
#include "plato/input_parser/InputBlockUtilities.hpp"
#include "plato/services/AppConfiguration.hpp"
#include "plato/test_utilities/InputGeneration.hpp"

namespace plato::integration_tests::utilities
{
services::ConfigurationDirectorySetupTeardown register_test_mass_app(const std::string_view aAppName,
                                                                     const boost::mpi::communicator& aComm)
{
    const auto tTestPluginDirectory = std::filesystem::path{"test-plugin-directory"};
    auto tConfigurationTempDirectory = services::ConfigurationDirectorySetupTeardown{tTestPluginDirectory, aComm};

    const auto tCriterionSerialConfiguration =
        services::CriterionConfiguration{/*.mName=*/"mass", /*.mFunctionName=*/"plato_create_criterion",
                                         /*.mIsParallelized=*/false};
    const auto tCriterionParallelConfiguration =
        services::CriterionConfiguration{/*.mName=*/"mass", /*.mFunctionName=*/"plato_create_parallel_criterion",
                                         /*.mIsParallelized=*/true};
    tConfigurationTempDirectory.addConfiguration(
        services::AppConfiguration{/*.mName=*/std::string{aAppName},
                                   /*.mLibraryFileName=*/"../libPlatoTestMassObjective.so",
                                   /*.mCriteria=*/{tCriterionSerialConfiguration, tCriterionParallelConfiguration}},
        "test-mass-app.config");
    aComm.barrier();
    criteria::extension::register_plugin_apps({tTestPluginDirectory});
    return tConfigurationTempDirectory;
}

process_manager::library::ValidatedInput create_valid_brick_input(const std::string_view aAppName,
                                                                  const unsigned int aNumProcessors)
{
    auto tObjective = input_parser::objective{};
    tObjective.number_of_processors = aNumProcessors;
    tObjective.aggregation_weight = 1.0;
    tObjective.app = input_parser::AppName{std::string{aAppName}};
    tObjective.name = "test_1";

    const auto tInput = tObjective | test_utilities::create_valid_brick_shape_geometry() |
                        test_utilities::create_valid_example_rol_optimization();
    return process_manager::library::make_validated_input(tInput);
}

std::pair<linear_algebra::DynamicVector<double>, double> brick_shape_geometry_controls_with_volume()
{
    constexpr auto tX = double{2.0};
    constexpr auto tY = double{4.0};
    constexpr auto tZ = double{6.0};
    constexpr auto tCenter = double{0.0};
    return std::make_pair(linear_algebra::DynamicVector<double>{tCenter, tCenter, tCenter, tX, tY, tZ}, tX * tY * tZ);
}

}  // namespace plato::integration_tests::utilities
