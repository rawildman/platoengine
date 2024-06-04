#ifndef PLATO_SERVICES_APPCONFIGURATIONUTILITIES
#define PLATO_SERVICES_APPCONFIGURATIONUTILITIES

#include <filesystem>

#include "plato/services/AppConfiguration.hpp"

namespace plato::services
{
/// @brief Returns all app configurations found in the plugins installation directory as well as those found in the
/// additional search directories given in @a aAdditionalSearchDirectories.
std::vector<AppConfigurationWithDirectory> app_configurations(
    std::vector<std::filesystem::path> aAdditionalSearchDirectories = {});

/// @brief Adds the directory @a aDirectory to the app configuration for generating absolute paths.
AppConfigurationWithDirectory app_configuration_with_directory(AppConfiguration aAppConfiguration,
                                                               std::filesystem::path aDirectory);

/// @brief Returns the path to the shared library contained in @a aAppConfiguration.
std::filesystem::path shared_library_path(const AppConfigurationWithDirectory& aAppConfiguration);

/// @brief Generates an AppConfiguration for a `custom_app`.
///
/// This is used when the criterion type is `custom_app` and an AppConfiguration is needed to interface
/// with other code. It contains hard-coded function names for the functions to load from a the shared lib and
/// is assumed that both serial and parallel functions are implemented.
AppConfigurationWithDirectory default_app_configuration(const std::filesystem::path& aSharedLibPath);
}  // namespace plato::services

#endif
