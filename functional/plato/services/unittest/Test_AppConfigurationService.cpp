#include <gtest/gtest.h>

#include <algorithm>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <filesystem>
#include <fstream>
#include <iterator>

#include "plato/services/AbstractAppConfigurationService.hpp"
#include "plato/services/AppConfiguration.hpp"
#include "plato/services/AppConfigurationService.hpp"
#include "plato/utilities/TransformIf.hpp"

namespace plato::services::unittest
{
namespace
{
const auto kTestConfiguration =
    services::AppConfiguration{/*.name=*/"test-app", /*.lib_name=*/"libtest.so",
                               /*.mHasParallelImplementation=*/true, /*.mHasSerialImplementation=*/true};

const auto kAnotherTestConfiguration =
    services::AppConfiguration{/*.name=*/"another-test-app", /*.lib_name=*/"libanothertest.so",
                               /*.mHasParallelImplementation=*/false, /*.mHasSerialImplementation=*/true};

constexpr auto kConfigFileExtension = std::string_view{".config"};

[[nodiscard]] std::vector<AppConfiguration> app_configurations_from_directory(
    const std::filesystem::path& aSearchDirectory)
{
    auto tAppConfigurations = std::vector<AppConfiguration>{};
    utilities::transform_if(
        std::filesystem::directory_iterator{aSearchDirectory}, std::back_inserter(tAppConfigurations),
        [](const auto& tDirectoryEntry) { return load(tDirectoryEntry.path()); },
        [](const auto& tDirectoryEntry) { return tDirectoryEntry.path().extension() == kConfigFileExtension; });
    return tAppConfigurations;
}

class MockSingleAppConfigurationService : public AbstractAppConfigurationService
{
   public:
    MockSingleAppConfigurationService(AppConfiguration aAppConfiguration)
        : mAppConfiguration{std::move(aAppConfiguration)}
    {
    }

   private:
    [[nodiscard]] bool libraryIsLoadableImpl(const std::string_view aLibraryName) const override
    {
        return mAppConfiguration.mName == aLibraryName;
    }

    [[nodiscard]] std::vector<AppConfiguration> appConfigurationsImpl() const override { return {mAppConfiguration}; }

   private:
    AppConfiguration mAppConfiguration;
};

class MockDirectoryScannedAppConfigurationService : public AbstractAppConfigurationService
{
   public:
    MockDirectoryScannedAppConfigurationService(const std::filesystem::path& aDirectory)
        : mAppConfigurations{app_configurations_from_directory(aDirectory)}
    {
    }

   private:
    [[nodiscard]] bool libraryIsLoadableImpl(const std::string_view aLibraryName) const override
    {
        return std::any_of(mAppConfigurations.cbegin(), mAppConfigurations.cend(),
                           [aLibraryName](const auto& aConfiguration) { return aConfiguration.mName == aLibraryName; });
    }

    [[nodiscard]] std::vector<AppConfiguration> appConfigurationsImpl() const override { return {mAppConfigurations}; }

   private:
    std::vector<AppConfiguration> mAppConfigurations;
};
}  // namespace

TEST(AppConfigurationService, LibraryIsLoadable)
{
    services::new_global_app_configuration_service<MockSingleAppConfigurationService>(kTestConfiguration);
    auto& tAppConfigurationService = services::app_configuration_service();
    EXPECT_TRUE(tAppConfigurationService.libraryIsLoadable(kTestConfiguration.mName));
    EXPECT_FALSE(tAppConfigurationService.libraryIsLoadable("iguana"));
}

TEST(AppConfigurationService, AppConfigurations)
{
    services::new_global_app_configuration_service<MockSingleAppConfigurationService>(kTestConfiguration);
    auto& tAppConfigurationService = services::app_configuration_service();
    ASSERT_EQ(tAppConfigurationService.appConfigurations().size(), 1u);
    EXPECT_EQ(tAppConfigurationService.appConfigurations().front(), kTestConfiguration);
}

TEST(AppConfigurationService, LoadFromDirectory)
{
    const auto tTestDirectory = std::filesystem::path{"test-configuration-directory"};
    std::filesystem::create_directories(tTestDirectory);

    save(kTestConfiguration, tTestDirectory / "test-1.config");
    save(kAnotherTestConfiguration, tTestDirectory / "test-2.config");

    services::new_global_app_configuration_service<MockDirectoryScannedAppConfigurationService>(tTestDirectory);
    auto& tAppConfigurationService = services::app_configuration_service();

    EXPECT_TRUE(tAppConfigurationService.libraryIsLoadable(kTestConfiguration.mName));
    ASSERT_EQ(tAppConfigurationService.appConfigurations().size(), 2u);
    const auto tAppConfigurations = tAppConfigurationService.appConfigurations();

    const auto tTestConfigurationIter =
        std::find(tAppConfigurations.cbegin(), tAppConfigurations.cend(), kTestConfiguration);
    EXPECT_NE(tTestConfigurationIter, tAppConfigurations.cend());

    const auto tAnotherTestConfigurationIter =
        std::find(tAppConfigurations.cbegin(), tAppConfigurations.cend(), kAnotherTestConfiguration);
    EXPECT_NE(tAnotherTestConfigurationIter, tAppConfigurations.cend());

    std::filesystem::remove_all(tTestDirectory);
}

}  // namespace plato::services::unittest
