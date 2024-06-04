#include <gtest/gtest.h>

#include "plato/services/AppConfigurationUtilities.hpp"

namespace plato::services::unittest
{
TEST(AppConfigurationUtilities, CustomAppConfigurationNoPath)
{
    constexpr auto tTestPath = std::string_view{"libtest.so"};
    const auto tDefaultConfiguration = default_app_configuration(tTestPath);

    EXPECT_EQ(tDefaultConfiguration.mLibraryDirectory, "");
    const auto& tAppConfiguration = tDefaultConfiguration.mConfiguration;
    EXPECT_EQ(tAppConfiguration.mName, "default");
    EXPECT_EQ(tAppConfiguration.mLibraryFileName, tTestPath);

    ASSERT_EQ(tAppConfiguration.mCriteria.size(), 2u);
    EXPECT_EQ(tAppConfiguration.mCriteria.front().mName, "default");
    EXPECT_EQ(tAppConfiguration.mCriteria.front().mFunctionName, "plato_create_criterion");
    EXPECT_EQ(tAppConfiguration.mCriteria.front().mIsParallelized, false);

    EXPECT_EQ(tAppConfiguration.mCriteria.back().mName, "default");
    EXPECT_EQ(tAppConfiguration.mCriteria.back().mFunctionName, "plato_create_parallel_criterion");
    EXPECT_EQ(tAppConfiguration.mCriteria.back().mIsParallelized, true);
}

TEST(AppConfigurationUtilities, CustomAppConfigurationRelativePath)
{
    const auto tTestRelativePath = std::filesystem::path{"fake/path"};
    const auto tTestLibName = std::filesystem::path{"libtest.so"};
    const auto tDefaultConfiguration = default_app_configuration(tTestRelativePath / tTestLibName);

    EXPECT_EQ(tDefaultConfiguration.mLibraryDirectory, tTestRelativePath);
    EXPECT_EQ(tDefaultConfiguration.mConfiguration.mLibraryFileName, tTestLibName.string());
}

TEST(AppConfigurationUtilities, CustomAppConfigurationAbsolutePath)
{
    const auto tTestAbsolutePath = std::filesystem::path{"/full/path"};
    const auto tTestLibName = std::filesystem::path{"libtest.so"};
    const auto tDefaultConfiguration = default_app_configuration(tTestAbsolutePath / tTestLibName);

    EXPECT_EQ(tDefaultConfiguration.mLibraryDirectory, tTestAbsolutePath);
    EXPECT_EQ(tDefaultConfiguration.mConfiguration.mLibraryFileName, tTestLibName.string());
}

TEST(AppConfigurationUtilities, SharedLibraryPath)
{
    const auto tTestPath = std::filesystem::path{"/fake/path/to/libtest.so"};
    const auto tDefaultConfiguration = default_app_configuration(tTestPath);
    EXPECT_EQ(tTestPath, shared_library_path(tDefaultConfiguration));
}

}  // namespace plato::services::unittest
