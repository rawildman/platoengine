#include "plato/services/AppConfiguration.hpp"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <fstream>

#include "plato/services/PluginDirectoryPath.hpp"
#include "plato/utilities/TransformIf.hpp"

namespace plato::services
{
namespace
{
constexpr auto kConfigFileExtension = std::string_view{".config"};
}  // namespace

std::vector<AppConfiguration> app_configurations(std::vector<std::filesystem::path> aAdditionalSearchDirectories)
{
    auto tSearchDirectories = std::move(aAdditionalSearchDirectories);
    if (auto tPluginPath = plugin_directory_path())
    {
        tSearchDirectories.push_back(std::move(tPluginPath).value());
    }

    auto tAppConfigurations = std::vector<AppConfiguration>{};
    for (const auto& tDirectory : tSearchDirectories)
    {
        utilities::transform_if(
            std::filesystem::directory_iterator{tDirectory}, std::back_inserter(tAppConfigurations),
            [](const auto& tDirectoryEntry) { return load(tDirectoryEntry.path()); },
            [](const auto& tDirectoryEntry) { return tDirectoryEntry.path().extension() == kConfigFileExtension; });
    }
    return tAppConfigurations;
}

void save(const AppConfiguration& aAppConfiguration, const std::filesystem::path& aFilename)
{
    auto tOutFileStream = std::ofstream{aFilename};
    auto tOutputArchive = boost::archive::text_oarchive{tOutFileStream};
    tOutputArchive << aAppConfiguration;
}

AppConfiguration load(const std::filesystem::path& aFilename)
{
    auto tInFileStream = std::ifstream{aFilename};
    auto tInputArchive = boost::archive::text_iarchive{tInFileStream};
    auto tAppConfiguration = AppConfiguration{};
    tInputArchive >> tAppConfiguration;
    return tAppConfiguration;
}

bool operator==(const AppConfiguration& aAppConfigurationLeft, const AppConfiguration& aAppConfigurationRight)
{
    return aAppConfigurationLeft.mName == aAppConfigurationRight.mName &&
           aAppConfigurationLeft.mLibraryFileName == aAppConfigurationRight.mLibraryFileName &&
           aAppConfigurationLeft.mHasParallelImplementation == aAppConfigurationRight.mHasParallelImplementation &&
           aAppConfigurationLeft.mHasSerialImplementation == aAppConfigurationRight.mHasSerialImplementation;
}
}  // namespace plato::services
