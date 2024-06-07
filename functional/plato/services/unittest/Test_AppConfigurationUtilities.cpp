#include <gtest/gtest.h>

#include "plato/services/AppConfigurationUtilities.hpp"

namespace plato::services::unittest
{
TEST(AppConfigurationUtilities, SharedLibraryPath)
{
    const auto tTestPath = std::filesystem::path{"/fake/path/to/"};
    const auto tTestLibName = std::filesystem::path{"libpatterns.so"};
    const auto tPaisleyCriterion =
        CriterionConfiguration{/*.mName=*/"paisley", /*.mIsParallelized=*/false, /*.mFunctionName=*/"paisley_function"};
    const auto tConfiguration = AppConfiguration{/*.mName=*/"patterns",
                                                 /*.mLibraryFileName=*/tTestLibName.string(),
                                                 {tPaisleyCriterion}};
    EXPECT_EQ(tTestPath / tTestLibName, shared_library_path(AppConfigurationWithDirectory{tConfiguration, tTestPath}));
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
