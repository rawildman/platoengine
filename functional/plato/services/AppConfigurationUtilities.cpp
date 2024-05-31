#include "plato/services/AppConfigurationUtilities.hpp"

namespace plato::services
{
ConfigurationDirectorySetupTeardown::ConfigurationDirectorySetupTeardown(std::filesystem::path aDirectory)
    : mDirectory(std::move(aDirectory))
{
    std::filesystem::create_directories(mDirectory);
}

ConfigurationDirectorySetupTeardown::~ConfigurationDirectorySetupTeardown() { std::filesystem::remove_all(mDirectory); }

ConfigurationDirectorySetupTeardown& ConfigurationDirectorySetupTeardown::addConfiguration(
    const services::AppConfiguration& aConfiguration, const std::filesystem::path& aFilename)
{
    services::save_configuration(aConfiguration, mDirectory / aFilename);
    return *this;
}

const std::filesystem::path& ConfigurationDirectorySetupTeardown::directory() const { return mDirectory; }

}  // namespace plato::services
