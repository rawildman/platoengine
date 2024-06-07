#include <gtest/gtest.h>

#include <filesystem>
#include <string_view>

#include "plato/criteria/extension/PluginCriteria.hpp"
#include "plato/criteria/library/CriterionRegistration.hpp"
#include "plato/criteria/library/CriterionValidation.hpp"
#include "plato/input_parser/InputBlocks.hpp"
#include "plato/input_parser/InputFieldTypes.hpp"
#include "plato/services/AppConfiguration.hpp"
#include "plato/services/AppConfigurationUtilities.hpp"
#include "plato/test_utilities/TestDirectorySetupTeardown.hpp"
#include "plato/utilities/StringUtilities.hpp"

namespace plato::criteria::extension::unittest
{
namespace
{
const auto kTestCriterionName = std::string{"test-criterion"};

[[nodiscard]] test_utilities::TestDirectorySetupTeardown create_test_app_configurations(
    const std::vector<std::string_view>& aAppNames)
{
    auto tConfigurationTempDirectory = test_utilities::TestDirectorySetupTeardown{"test-plugin-directory"};

    for (const auto tAppName : aAppNames)
    {
        const auto tLibName = utilities::concatenate("lib", tAppName, ".so");
        const auto tConfigName = utilities::concatenate(tAppName, ".config");
        const auto tCriterionConfiguration = services::CriterionConfiguration{
            /*.mName=*/kTestCriterionName,
            /*.mIsParallelized=*/true,
            /*.mFunctionName=*/"plato_create_criterion",
        };
        auto tAppConfiguration = services::AppConfiguration{/*.mName=*/std::string{tAppName},
                                                            /*mLibraryFileName=*/tLibName,
                                                            /*.mCriteria=*/{tCriterionConfiguration}};
        tConfigurationTempDirectory.writeFile(services::AppConfigurationWriter{std::move(tAppConfiguration)},
                                              tConfigName);
    }
    return tConfigurationTempDirectory;
}
}  // namespace

TEST(PluginCriteria, NumberOfPluginsRegistered)
{
    // Not much to test since we don't know how many there might be.
    // Just check that it's equal to the number of apps found.
    EXPECT_EQ(services::app_configurations().size(), number_of_plugins_registered_at_startup());
}

TEST(PluginCriteria, RegisterApps)
{
    // Checks that some fake apps get registered via register_plugin_apps
    const auto tVampireAppName = std::string{"vampire"};
    const auto tMummyAppName = std::string{"mummy"};
    auto tConfigurationTempDirectory = create_test_app_configurations({tVampireAppName, tMummyAppName});
    const auto tNumRegistered = register_plugin_apps({tConfigurationTempDirectory.directory()});
    EXPECT_GE(tNumRegistered, 2u);
    EXPECT_TRUE(library::is_parallel_criterion_function_registered(library::criterion_registration_name(
        input_parser::AppName{tVampireAppName}, input_parser::CriterionName{kTestCriterionName})));
    EXPECT_TRUE(library::is_parallel_criterion_function_registered(library::criterion_registration_name(
        input_parser::AppName{tMummyAppName}, input_parser::CriterionName{kTestCriterionName})));
}

TEST(PluginCriteria, Validation)
{
    const auto tFrankensteinAppName = std::string{"frankenstein"};
    const auto tMedusaAppName = std::string{"medusa"};
    auto tConfigurationTempDirectory = create_test_app_configurations({tFrankensteinAppName, tMedusaAppName});
    const auto tNumRegistered = register_plugin_apps({tConfigurationTempDirectory.directory()});
    EXPECT_GE(tNumRegistered, 2u);

    auto tCriteria = input_parser::objective{};
    EXPECT_TRUE(library::detail::validate_criterion_is_registered(tCriteria).has_value());
    tCriteria.app = input_parser::AppName{tFrankensteinAppName};
    tCriteria.criterion = input_parser::CriterionName{kTestCriterionName};
    EXPECT_FALSE(library::detail::validate_criterion_is_registered(tCriteria).has_value())
        << library::detail::validate_criterion_is_registered(tCriteria).value();
    tCriteria.app = input_parser::AppName{tMedusaAppName};
    EXPECT_FALSE(library::detail::validate_criterion_is_registered(tCriteria).has_value())
        << library::detail::validate_criterion_is_registered(tCriteria).value();
    tCriteria.app = input_parser::AppName{std::string{"bog-monster"}};
    EXPECT_TRUE(library::detail::validate_criterion_is_registered(tCriteria).has_value());
}

}  // namespace plato::criteria::extension::unittest
