#include <gtest/gtest.h>

#include <filesystem>
#include <string_view>

#include "plato/criteria/extension/PluginCriteria.hpp"
#include "plato/criteria/library/CriterionRegistration.hpp"
#include "plato/services/AppConfiguration.hpp"
#include "plato/services/AppConfigurationUtilities.hpp"

namespace plato::criteria::extension::unittest
{

TEST(PluginCriteria, NumberOfPluginsRegistered)
{
    // Not much to test since we don't know how many there might be.
    // Just check that it's equal to the number of apps found.
    EXPECT_EQ(services::app_configurations().size(), number_of_plugins_registered_at_startup());
}

TEST(PluginCriteria, RegisterApps)
{
    // Checks that some fake apps get registered via register_plugin_apps
    auto tConfigurationTempDirectory = services::ConfigurationDirectorySetupTeardown{"test-plugin-directory"};

    constexpr auto tVampireAppName = std::string_view{"vampire"};
    constexpr auto tMummyAppName = std::string_view{"mummy"};
    tConfigurationTempDirectory
        .addConfiguration(
            services::AppConfiguration{/*.mName=*/std::string{tVampireAppName}, /*mLibraryFileName=*/"libvampire.so",
                                       /*.mHasParallelImplementation=*/true, /*.mHasSerialImplementation=*/true},
            "vampire.config")
        .addConfiguration(
            services::AppConfiguration{/*.mName=*/std::string{tMummyAppName}, /*.mLibraryFileName=*/"libmummy.so",
                                       /*.mHasParallelImplementation=*/true, /*.mHasSerialImplementation=*/true},
            "mummy.config");

    const auto tNumRegistered = register_plugin_apps({tConfigurationTempDirectory.directory()});

    EXPECT_EQ(tNumRegistered, 2u);
    EXPECT_TRUE(library::is_criterion_function_registered(tVampireAppName));
    EXPECT_TRUE(library::is_criterion_function_registered(tMummyAppName));
}

}  // namespace plato::criteria::extension::unittest
