#ifndef PLATO_SERVICES_APPCONFIGURATION
#define PLATO_SERVICES_APPCONFIGURATION

#include <filesystem>
#include <string>
#include <vector>

namespace plato::services
{
/// @brief Configuration data for defining an app's interface with plato.
///
/// A file, written using the serialize function is expected to be associated with each
/// shared library containing an app.
struct AppConfiguration
{
    std::string mName;
    std::string mLibraryFileName;
    bool mHasParallelImplementation = false;
    bool mHasSerialImplementation = false;
};

/// @brief Associates an AppConfiguration with the directory in which it was found.
///
/// The purpose of this is to generate a path for the shared library associated with the
/// AppConfiguration, `mLibraryFileName`.
struct AppConfigurationWithDirectory
{
    AppConfiguration mConfiguration;
    std::filesystem::path mLibraryDirectory;
};

/// @brief Returns all app configurations found in the plugins installation directory as well as those found in the
/// additional search directories given in @a aAdditionalSearchDirectories.
std::vector<AppConfigurationWithDirectory> app_configurations(
    std::vector<std::filesystem::path> aAdditionalSearchDirectories = {});

/// @brief Adds the directory @a aDirectory to the app configuration for generating absolute paths.
AppConfigurationWithDirectory app_configuration_with_directory(AppConfiguration aAppConfiguration,
                                                               std::filesystem::path aDirectory);

/// @brief Returns the path to the shared library contained in @a aAppConfiguration.
std::filesystem::path shared_library_path(const AppConfigurationWithDirectory& aAppConfiguration);

/// @brief Writes @a aAppConfiguration to disk, at path @a aFilename.
void save_configuration(const AppConfiguration& aAppConfiguration, const std::filesystem::path& aFilename);

/// @brief Reads an AppConfiguration from disk, at path @a aFilename.
[[nodiscard]] AppConfiguration load_configuration(const std::filesystem::path& aFilename);

/// @todo Use `operator==() = default` in c++20
[[nodiscard]] bool operator==(const AppConfiguration& aAppConfigurationLeft,
                              const AppConfiguration& aAppConfigurationRight);

template <class Archive>
void serialize(Archive& aArchive, AppConfiguration& aAppConfiguration, const unsigned int /*version*/)
{
    aArchive& aAppConfiguration.mName;
    aArchive& aAppConfiguration.mLibraryFileName;
    aArchive& aAppConfiguration.mHasParallelImplementation;
    aArchive& aAppConfiguration.mHasSerialImplementation;
}

}  // namespace plato::services

#endif
