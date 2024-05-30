#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "plato/services/AppConfiguration.hpp"
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
    save(aSerializable, tFilename);
    const auto tRoundTripResult = load(tFilename);

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

TEST(AppConfiguration, AppConfigurations)
{
    const auto tTestDirectory = std::filesystem::path{"test-configuration-directory"};
    std::filesystem::create_directories(tTestDirectory);

    save(kTestConfiguration, tTestDirectory / "test-1.config");
    save(kAnotherTestConfiguration, tTestDirectory / "test-2.config");

    const auto tAppConfigurations = app_configurations({tTestDirectory});

    EXPECT_GE(tAppConfigurations.size(), 2u);

    const auto tTestConfigurationIter =
        std::find(tAppConfigurations.cbegin(), tAppConfigurations.cend(), kTestConfiguration);
    EXPECT_NE(tTestConfigurationIter, tAppConfigurations.cend());

    const auto tAnotherTestConfigurationIter =
        std::find(tAppConfigurations.cbegin(), tAppConfigurations.cend(), kAnotherTestConfiguration);
    EXPECT_NE(tAnotherTestConfigurationIter, tAppConfigurations.cend());
}

}  // namespace plato::services
