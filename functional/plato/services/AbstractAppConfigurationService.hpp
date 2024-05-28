#ifndef PLATO_SERVICES_ABSTRACTSHAREDLIBSERVICE
#define PLATO_SERVICES_ABSTRACTSHAREDLIBSERVICE

#include <string_view>
#include <vector>

#include "plato/services/AppConfiguration.hpp"

namespace plato::services
{
/// @brief Provides an abstract interface for the shared library service.
///
/// The purpose of this class is to load an app's configuration file and
/// interface with dlopen and dlsym for loading libraries and their functions
/// as described in the configuration files.
/// The main implementation is DefaultAppConfigurationServices.
class AbstractAppConfigurationService
{
   public:
    AbstractAppConfigurationService() = default;
    virtual ~AbstractAppConfigurationService() = default;

    [[nodiscard]] bool libraryIsLoadable(const std::string_view aLibraryName) const
    {
        return libraryIsLoadableImpl(aLibraryName);
    }

    [[nodiscard]] std::vector<AppConfiguration> appConfigurations() const { return appConfigurationsImpl(); }

    AbstractAppConfigurationService(const AbstractAppConfigurationService&) = delete;
    AbstractAppConfigurationService(AbstractAppConfigurationService&&) = delete;
    AbstractAppConfigurationService& operator=(const AbstractAppConfigurationService&) = delete;
    AbstractAppConfigurationService& operator=(AbstractAppConfigurationService&&) = delete;

   private:
    [[nodiscard]] virtual bool libraryIsLoadableImpl(std::string_view aLibraryName) const = 0;
    [[nodiscard]] virtual std::vector<AppConfiguration> appConfigurationsImpl() const = 0;
};
}  // namespace plato::services

#endif
