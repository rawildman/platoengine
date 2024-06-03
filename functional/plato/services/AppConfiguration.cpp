#include "plato/services/AppConfiguration.hpp"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <fstream>

#include "plato/services/AppConfigurationUtilities.hpp"
#include "plato/services/PluginDirectoryPath.hpp"
#include "plato/utilities/TransformIf.hpp"

namespace plato::services
{
namespace
{
constexpr auto kConfigFileExtension = std::string_view{".config"};
}  // namespace

std::vector<AppConfigurationWithDirectory> app_configurations(
    std::vector<std::filesystem::path> aAdditionalSearchDirectories)
{
    auto tSearchDirectories = std::move(aAdditionalSearchDirectories);
    if (auto tPluginPath = plugin_directory_path())
    {
        tSearchDirectories.push_back(std::move(tPluginPath).value());
    }

    auto tAppConfigurations = std::vector<AppConfigurationWithDirectory>{};
    for (const auto& tDirectory : tSearchDirectories)
    {
        utilities::transform_if(
            std::filesystem::directory_iterator{tDirectory}, std::back_inserter(tAppConfigurations),
            [&tDirectory](const auto& tDirectoryEntry)
            { return app_configuration_with_directory(load_configuration(tDirectoryEntry.path()), tDirectory); },
            [](const auto& tDirectoryEntry) { return tDirectoryEntry.path().extension() == kConfigFileExtension; });
    }
    return tAppConfigurations;
}

void save_configuration(const AppConfiguration& aAppConfiguration, const std::filesystem::path& aFilename)
{
    auto tOutFileStream = std::ofstream{aFilename};
    auto tOutputArchive = boost::archive::text_oarchive{tOutFileStream};
    tOutputArchive << aAppConfiguration;
}

AppConfiguration load_configuration(const std::filesystem::path& aFilename)
{
    auto tInFileStream = std::ifstream{aFilename};
    auto tInputArchive = boost::archive::text_iarchive{tInFileStream};
    auto tAppConfiguration = AppConfiguration{};
    tInputArchive >> tAppConfiguration;
    return tAppConfiguration;
}

bool operator==(const CriterionConfiguration& aCriterionConfigurationLeft,
                const CriterionConfiguration& aCriterionConfigurationRight)
{
    return aCriterionConfigurationLeft.mName == aCriterionConfigurationRight.mName &&
           aCriterionConfigurationLeft.mFunctionName == aCriterionConfigurationRight.mFunctionName &&
           aCriterionConfigurationLeft.mIsParallelized == aCriterionConfigurationRight.mIsParallelized;
}

bool operator==(const AppConfiguration& aAppConfigurationLeft, const AppConfiguration& aAppConfigurationRight)
{
    return aAppConfigurationLeft.mName == aAppConfigurationRight.mName &&
           aAppConfigurationLeft.mLibraryFileName == aAppConfigurationRight.mLibraryFileName &&
           aAppConfigurationLeft.mCriteria == aAppConfigurationRight.mCriteria;
}
}  // namespace plato::services
