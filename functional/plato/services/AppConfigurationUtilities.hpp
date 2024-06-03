#ifndef PLATO_SERVICES_APPCONFIGURATIONUTILITIES
#define PLATO_SERVICES_APPCONFIGURATIONUTILITIES

#include <filesystem>

#include "plato/services/AppConfiguration.hpp"

namespace plato::services
{
/// @brief Adds the directory @a aDirectory to the app configuration for generating absolute paths.
AppConfigurationWithDirectory app_configuration_with_directory(AppConfiguration aAppConfiguration,
                                                               std::filesystem::path aDirectory);

/// @brief Returns the path to the shared library contained in @a aAppConfiguration.
std::filesystem::path shared_library_path(const AppConfigurationWithDirectory& aAppConfiguration);

}  // namespace plato::services

#endif
