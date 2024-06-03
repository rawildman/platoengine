#include <gtest/gtest.h>

#include <filesystem>
#include <string_view>

#include "plato/criteria/extension/PluginCriteria.hpp"
#include "plato/criteria/library/CriterionRegistration.hpp"
#include "plato/criteria/library/CriterionValidation.hpp"
#include "plato/input_parser/InputBlocks.hpp"
#include "plato/services/AppConfiguration.hpp"
#include "plato/services/ConfigurationDirectorySetupTeardown.hpp"
#include "plato/utilities/StringUtilities.hpp"

namespace plato::criteria::extension::unittest
{
[[nodiscard]] services::ConfigurationDirectorySetupTeardown create_test_app_configurations(
    const std::vector<std::string_view>& aAppNames)
{
    auto tConfigurationTempDirectory = services::ConfigurationDirectorySetupTeardown{"test-plugin-directory"};

    for (const auto tAppName : aAppNames)
    {
        const auto tLibName = utilities::concatenate("lib", tAppName, ".so");
        const auto tConfigName = utilities::concatenate(tAppName, ".config");
        const auto tCriterionConfiguration =
            services::CriterionConfiguration{/*.mName=*/"test-criterion", /*.mFunctionName=*/"plato_create_criterion",
                                             /*.mIsParallelized=*/true};
        tConfigurationTempDirectory.addConfiguration(
            services::AppConfiguration{/*.mName=*/std::string{tAppName},
                                       /*mLibraryFileName=*/tLibName,
                                       /*.mCriteria=*/{tCriterionConfiguration}},
            tConfigName);
    }
    return tConfigurationTempDirectory;
}

TEST(PluginCriteria, NumberOfPluginsRegistered)
{
    // Not much to test since we don't know how many there might be.
    // Just check that it's equal to the number of apps found.
    EXPECT_EQ(services::app_configurations().size(), number_of_plugins_registered_at_startup());
}

TEST(PluginCriteria, RegisterApps)
{
    // Checks that some fake apps get registered via register_plugin_apps
    constexpr auto tVampireAppName = std::string_view{"vampire"};
    constexpr auto tMummyAppName = std::string_view{"mummy"};
    auto tConfigurationTempDirectory = create_test_app_configurations({tVampireAppName, tMummyAppName});
    const auto tNumRegistered = register_plugin_apps({tConfigurationTempDirectory.directory()});
    EXPECT_GE(tNumRegistered, 2u);
    EXPECT_TRUE(library::is_criterion_function_registered(tVampireAppName));
    EXPECT_TRUE(library::is_criterion_function_registered(tMummyAppName));
}

TEST(PluginCriteria, Validation)
{
    constexpr auto tFrankensteinAppName = std::string_view{"frankenstein"};
    constexpr auto tMedusaAppName = std::string_view{"medusa"};
    auto tConfigurationTempDirectory = create_test_app_configurations({tFrankensteinAppName, tMedusaAppName});
    const auto tNumRegistered = register_plugin_apps({tConfigurationTempDirectory.directory()});
    EXPECT_GE(tNumRegistered, 2u);

    auto tCriteria = input_parser::objective{};
    EXPECT_TRUE(library::detail::validate_app_is_registered(tCriteria).has_value());
    tCriteria.app = input_parser::AppName{std::string{tFrankensteinAppName}};
    EXPECT_FALSE(library::detail::validate_app_is_registered(tCriteria).has_value());
    tCriteria.app = input_parser::AppName{std::string{tMedusaAppName}};
    EXPECT_FALSE(library::detail::validate_app_is_registered(tCriteria).has_value());
    tCriteria.app = input_parser::AppName{std::string{"bog-monster"}};
    EXPECT_TRUE(library::detail::validate_app_is_registered(tCriteria).has_value());
}

}  // namespace plato::criteria::extension::unittest
