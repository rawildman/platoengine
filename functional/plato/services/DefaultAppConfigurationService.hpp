#ifndef PLATO_SERVICES_DEFAULTSHAREDLIBSERVICE
#define PLATO_SERVICES_DEFAULTSHAREDLIBSERVICE

#include <string_view>
#include <vector>

#include "plato/services/AbstractAppConfigurationService.hpp"
#include "plato/services/AppConfiguration.hpp"

namespace plato::services
{
class DefaultAppConfigurationService : public AbstractAppConfigurationService
{
   public:
    DefaultAppConfigurationService() = default;

   private:
    [[nodiscard]] bool libraryIsLoadableImpl(std::string_view aLibraryName) const override;
    [[nodiscard]] std::vector<AppConfiguration> appConfigurationsImpl() const override;

   private:
    std::vector<AppConfiguration> mAppConfigurations;
};

}  // namespace plato::services

#endif
