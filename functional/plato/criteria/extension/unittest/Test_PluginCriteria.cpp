#include <gtest/gtest.h>

#include <filesystem>
#include <string_view>

#include "plato/criteria/extension/PluginCriteria.hpp"
#include "plato/criteria/library/CriterionRegistration.hpp"
#include "plato/services/AppConfiguration.hpp"

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
    // Register fake apps and check
    const auto tTestDirectory = std::filesystem::path{"test-plugin-directory"};
    std::filesystem::create_directories(tTestDirectory);

    constexpr auto tVampireAppName = std::string_view{"vampire"};
    const auto tVampireApp =
        services::AppConfiguration{/*.mName=*/std::string{tVampireAppName}, /*mLibraryFileName=*/"libvampire.so",
                                   /*.mHasParallelImplementation=*/true, /*.mHasSerialImplementation=*/true};
    services::save_configuration(tVampireApp, tTestDirectory / "vampire.config");

    constexpr auto tMummyAppName = std::string_view{"mummy"};
    const auto tMummyApp =
        services::AppConfiguration{/*.mName=*/std::string{tMummyAppName}, /*.mLibraryFileName=*/"libmummy.so",
                                   /*.mHasParallelImplementation=*/true, /*.mHasSerialImplementation=*/true};
    services::save_configuration(tMummyApp, tTestDirectory / "mummy.config");

    const auto tNumRegistered = register_plugin_apps({tTestDirectory});

    EXPECT_EQ(tNumRegistered, 2u);
    EXPECT_TRUE(library::is_criterion_function_registered(tVampireAppName));
    EXPECT_TRUE(library::is_criterion_function_registered(tMummyAppName));

    std::filesystem::remove_all(tTestDirectory);
}

}  // namespace plato::criteria::extension::unittest
