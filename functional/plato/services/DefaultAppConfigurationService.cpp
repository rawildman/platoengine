#include "plato/services/DefaultAppConfigurationService.hpp"

namespace plato::services
{
bool DefaultAppConfigurationService::libraryIsLoadableImpl(const std::string_view) const { return false; }

std::vector<AppConfiguration> DefaultAppConfigurationService::appConfigurationsImpl() const
{
    return mAppConfigurations;
}

}  // namespace plato::services
