#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "plato/services/AppConfiguration.hpp"
#include "plato/test_utilities/TestContext.hpp"

namespace plato::services
{
namespace
{
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
}  // namespace plato::services
