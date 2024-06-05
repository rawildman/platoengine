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

TEST(AppConfigurationUtilities, FunctionName)
{
    const auto tPepsiCriterion =
        CriterionConfiguration{/*.mName=*/"pepsi", /*.mIsParallelized=*/false, /*.mFunctionName=*/"pepsi_function"};
    const auto tParallelPepsiCriterion = CriterionConfiguration{/*.mName=*/"pepsi", /*.mIsParallelized=*/true,
                                                                /*.mFunctionName=*/"pepsi_parallel_function"};
    const auto tCokeCriterion =
        CriterionConfiguration{/*.mName=*/"coke", /*.mIsParallelized=*/true, /*.mFunctionName=*/"coke_function"};
    const auto tSpriteCriterion =
        CriterionConfiguration{/*.mName=*/"sprite", /*.mIsParallelized=*/false, /*.mFunctionName=*/"sprite_function"};
    const auto tSodaConfigurations =
        AppConfiguration{/*.mName=*/"sodas",
                         /*.mLibraryFileName=*/"libsoda.so",
                         {tParallelPepsiCriterion, tPepsiCriterion, tCokeCriterion, tSpriteCriterion}};

    {
        // Non-existent
        const auto tFruitPunchCriterion = CriterionConfiguration{/*.mName=*/"fruit-punch", /*.mIsParallelized=*/false};
        EXPECT_FALSE(function_name(tSodaConfigurations, tFruitPunchCriterion).has_value());
    }
    {
        // Using same CriterionConfiguration objects
        ASSERT_TRUE(function_name(tSodaConfigurations, tPepsiCriterion).has_value());
        EXPECT_EQ(function_name(tSodaConfigurations, tPepsiCriterion).value(), "pepsi_function");

        ASSERT_TRUE(function_name(tSodaConfigurations, tParallelPepsiCriterion).has_value());
        EXPECT_EQ(function_name(tSodaConfigurations, tParallelPepsiCriterion).value(), "pepsi_parallel_function");

        ASSERT_TRUE(function_name(tSodaConfigurations, tCokeCriterion).has_value());
        EXPECT_EQ(function_name(tSodaConfigurations, tCokeCriterion).value(), "coke_function");

        ASSERT_TRUE(function_name(tSodaConfigurations, tSpriteCriterion).has_value());
        EXPECT_EQ(function_name(tSodaConfigurations, tSpriteCriterion).value(), "sprite_function");
    }
    {
        // Using different objects without the function name
        const auto tStripFunctionName = [](CriterionConfiguration aCriterionConfiguration)
        {
            aCriterionConfiguration.mFunctionName = "";
            return aCriterionConfiguration;
        };
        ASSERT_TRUE(function_name(tSodaConfigurations, tStripFunctionName(tPepsiCriterion)).has_value());
        EXPECT_EQ(function_name(tSodaConfigurations, tStripFunctionName(tPepsiCriterion)).value(), "pepsi_function");

        ASSERT_TRUE(function_name(tSodaConfigurations, tStripFunctionName(tParallelPepsiCriterion)).has_value());
        EXPECT_EQ(function_name(tSodaConfigurations, tStripFunctionName(tParallelPepsiCriterion)).value(),
                  "pepsi_parallel_function");

        ASSERT_TRUE(function_name(tSodaConfigurations, tStripFunctionName(tCokeCriterion)).has_value());
        EXPECT_EQ(function_name(tSodaConfigurations, tStripFunctionName(tCokeCriterion)).value(), "coke_function");

        ASSERT_TRUE(function_name(tSodaConfigurations, tStripFunctionName(tSpriteCriterion)).has_value());
        EXPECT_EQ(function_name(tSodaConfigurations, tStripFunctionName(tSpriteCriterion)).value(), "sprite_function");
    }
}

}  // namespace plato::services::unittest
