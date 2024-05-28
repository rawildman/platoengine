#ifndef PLATO_SERVICES_SHAREDLIBSERVICE
#define PLATO_SERVICES_SHAREDLIBSERVICE

#include <memory>

namespace plato::services
{
class AbstractAppConfigurationService;
}

namespace plato::services
{
/// @brief Returns the current AbstractAppConfigurationService, either the default or
///  what was set on a call to set_global_app_configuration_service.
AbstractAppConfigurationService& app_configuration_service();

/// @brief Sets the global shared library service by allocating a new instance of type @a AppConfigurationServiceType
///  and calling its constructor with @a aArgs.
template <typename AppConfigurationServiceType, typename... Args>
void new_global_app_configuration_service(Args&&... aArgs);

namespace detail
{
void new_global_app_configuration_service_impl(
    std::unique_ptr<AbstractAppConfigurationService> aAppConfigurationService);
}

template <typename AppConfigurationServiceType, typename... Args>
void new_global_app_configuration_service(Args&&... aArgs)
{
    detail::new_global_app_configuration_service_impl(
        std::make_unique<AppConfigurationServiceType>(std::forward<Args>(aArgs)...));
}
}  // namespace plato::services

#endif
