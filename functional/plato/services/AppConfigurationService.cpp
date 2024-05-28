#include "plato/services/AppConfigurationService.hpp"

#include <memory>

#include "plato/services/DefaultAppConfigurationService.hpp"

namespace plato::services
{
namespace
{
auto tActiveAppConfigurationService = std::unique_ptr<AbstractAppConfigurationService>{};
}

namespace detail
{
void new_global_app_configuration_service_impl(
    std::unique_ptr<AbstractAppConfigurationService> aAppConfigurationService)
{
    tActiveAppConfigurationService = std::move(aAppConfigurationService);
}
}  // namespace detail

AbstractAppConfigurationService& app_configuration_service()
{
    [[maybe_unused]] static const bool tInitialized = []()
    {
        if (!tActiveAppConfigurationService)
        {
            tActiveAppConfigurationService = std::make_unique<DefaultAppConfigurationService>();
        }
        return static_cast<bool>(tActiveAppConfigurationService);
    }();
    return *tActiveAppConfigurationService;
}

}  // namespace plato::services
