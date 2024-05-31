#ifndef PLATO_SERVICES_APPCONFIGURATIONUTILITIES
#define PLATO_SERVICES_APPCONFIGURATIONUTILITIES

#include <filesystem>

#include "plato/services/AppConfiguration.hpp"

namespace plato::services
{
/// @brief The purpose of this class is to facilitate creating and destroying a temporary directory
/// to hold app configurations using the RAII idiom.
///
/// On construction, a directory is created, which can then be populated with AppConfiguration files
/// using the addConfiguration member. The directory and its contents are removed on destruction.
class ConfigurationDirectorySetupTeardown
{
   public:
    ConfigurationDirectorySetupTeardown(std::filesystem::path aDirectory);
    ~ConfigurationDirectorySetupTeardown();

    /// @brief Create a new AppConfiguration file with name @a aFilename in the directory specified at construction.
    ConfigurationDirectorySetupTeardown& addConfiguration(const services::AppConfiguration& aConfiguration,
                                                          const std::filesystem::path& aFilename);

    /// @brief Get the directory created on construction.
    const std::filesystem::path& directory() const;

   private:
    std::filesystem::path mDirectory;
};

}  // namespace plato::services

#endif
