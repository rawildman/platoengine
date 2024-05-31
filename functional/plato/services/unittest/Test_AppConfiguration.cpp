#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "plato/services/AppConfiguration.hpp"
#include "plato/services/AppConfigurationUtilities.hpp"
#include "plato/test_utilities/TestContext.hpp"

namespace plato::services
{
namespace
{
const auto kTestConfiguration =
    services::AppConfiguration{/*.name=*/"test-app", /*.lib_name=*/"libtest.so",
                               /*.mHasParallelImplementation=*/true, /*.mHasSerialImplementation=*/true};

const auto kAnotherTestConfiguration =
    services::AppConfiguration{/*.name=*/"another-test-app", /*.lib_name=*/"libanothertest.so",
                               /*.mHasParallelImplementation=*/false, /*.mHasSerialImplementation=*/true};

void testSerializeRoundTrip(const AppConfiguration& aSerializable, const test_utilities::TestContext& aTestContext)
{
    const auto tFilename = std::filesystem::path{"out.config"};
    save_configuration(aSerializable, tFilename);
    const auto tRoundTripResult = load_configuration(tFilename);

    EXPECT_EQ(aSerializable, tRoundTripResult) << aTestContext;

    std::filesystem::remove(tFilename);
}

}  // namespace

TEST(AppConfiguration, Serialization)
{
    const auto tAppConfiguration =
        services::AppConfiguration{/*.name=*/"test-app", /*.lib_name=*/"libtest.so",
                                   /*.mHasParallelImplementation=*/true, /*.mHasSerialImplementation=*/true};
    testSerializeRoundTrip(tAppConfiguration, TEST_CONTEXT("App configuration"));
}

TEST(AppConfiguration, AppConfigurationWithDirectory)
{
    const auto tSharedLibName = std::string_view{"libappetizer.so"};
    const auto tAppConfiguration =
        services::AppConfiguration{/*.name=*/"appetizer", /*.lib_name=*/std::string{tSharedLibName},
                                   /*.mHasParallelImplementation=*/true, /*.mHasSerialImplementation=*/true};
    const auto tDirectory = std::filesystem::path{"/path/to/food"};

    const auto tAppConfigurationWithDirectory = app_configuration_with_directory(tAppConfiguration, tDirectory);
    EXPECT_EQ(tAppConfigurationWithDirectory.mAppConfiguration, tAppConfiguration);
    EXPECT_EQ(tAppConfigurationWithDirectory.mLibraryDirectory, tDirectory);
    EXPECT_EQ(shared_library_path(tAppConfigurationWithDirectory), tDirectory / tSharedLibName);
}

TEST(AppConfiguration, AppConfigurations)
{
    auto tConfigurationTempDirectory = ConfigurationDirectorySetupTeardown{"test-configuration-directory"};
    tConfigurationTempDirectory.addConfiguration(kTestConfiguration, "test-1.config")
        .addConfiguration(kAnotherTestConfiguration, "test-2.config");

    const auto tAppConfigurations = app_configurations({tConfigurationTempDirectory.directory()});

    EXPECT_GE(tAppConfigurations.size(), 2u);

    const auto tTestConfigurationIter =
        std::find_if(tAppConfigurations.cbegin(), tAppConfigurations.cend(),
                     [](const auto& aAppConfigurationWithDirectory)
                     { return kTestConfiguration == aAppConfigurationWithDirectory.mAppConfiguration; });
    EXPECT_NE(tTestConfigurationIter, tAppConfigurations.cend());

    const auto tAnotherTestConfigurationIter =
        std::find_if(tAppConfigurations.cbegin(), tAppConfigurations.cend(),
                     [](const auto& aAppConfigurationWithDirectory)
                     { return kAnotherTestConfiguration == aAppConfigurationWithDirectory.mAppConfiguration; });
    EXPECT_NE(tAnotherTestConfigurationIter, tAppConfigurations.cend());
}

}  // namespace plato::services
