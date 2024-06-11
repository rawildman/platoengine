#ifndef PLATO_SERVICES_APPCONFIGURATIONUTILITIES
#define PLATO_SERVICES_APPCONFIGURATIONUTILITIES

#include <filesystem>
#include <optional>

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

/// @brief A helper function object for writing an AppConfiguration to disk.
struct AppConfigurationWriter
{
    void operator()(const std::filesystem::path& aFilePath) const;
    AppConfiguration mAppConfiguration{};
};
}  // namespace plato::services

#endif
