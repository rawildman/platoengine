#include <gtest/gtest.h>

#include <boost/archive/text_oarchive.hpp>
#include <filesystem>
#include <fstream>

#include "plato/services/AbstractAppConfigurationService.hpp"
#include "plato/services/AppConfiguration.hpp"
#include "plato/services/AppConfigurationService.hpp"

namespace plato::services::unittest
{
namespace
{
const auto kTestConfiguration =
    services::AppConfiguration{/*.name=*/"test-app", /*.lib_name=*/"libtest.so",
                               /*.mHasParallelImplementation=*/true, /*.mHasSerialImplementation=*/true};

class MockAppConfigurationService : public AbstractAppConfigurationService
{
   public:
    MockAppConfigurationService(AppConfiguration aAppConfiguration) : mAppConfiguration{std::move(aAppConfiguration)} {}

   private:
    [[nodiscard]] bool libraryIsLoadableImpl(const std::string_view aLibraryName) const override
    {
        return mAppConfiguration.mName == aLibraryName;
    }

    [[nodiscard]] std::vector<AppConfiguration> appConfigurationsImpl() const override { return {mAppConfiguration}; }

   private:
    AppConfiguration mAppConfiguration;
};
}  // namespace

TEST(AppConfigurationService, LibraryIsLoadable)
{
    services::new_global_app_configuration_service<MockAppConfigurationService>(kTestConfiguration);
    auto& tAppConfigurationService = services::app_configuration_service();
    EXPECT_TRUE(tAppConfigurationService.libraryIsLoadable(kTestConfiguration.mName));
    EXPECT_FALSE(tAppConfigurationService.libraryIsLoadable("iguana"));
}

TEST(AppConfigurationService, AppConfigurations)
{
    services::new_global_app_configuration_service<MockAppConfigurationService>(kTestConfiguration);
    auto& tAppConfigurationService = services::app_configuration_service();
    ASSERT_EQ(tAppConfigurationService.appConfigurations().size(), 1u);
    EXPECT_EQ(tAppConfigurationService.appConfigurations().front(), kTestConfiguration);
}

}  // namespace plato::services::unittest
