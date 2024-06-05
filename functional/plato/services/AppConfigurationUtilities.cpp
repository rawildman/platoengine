#include "plato/services/AppConfigurationUtilities.hpp"

#include <algorithm>

#include "plato/services/PluginDirectoryPath.hpp"
#include "plato/utilities/TransformIf.hpp"

namespace plato::services
{
namespace
{
constexpr auto kDefaultName = std::string_view{"default"};
constexpr auto kDefaultSerialFunctionName = std::string_view{"plato_create_criterion"};
constexpr auto kDefaultParallelFunctionName = std::string_view{"plato_create_parallel_criterion"};
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

AppConfigurationWithDirectory app_configuration_with_directory(AppConfiguration aAppConfiguration,
                                                               std::filesystem::path aDirectory)
{
    return AppConfigurationWithDirectory{/*.mConfiguration=*/std::move(aAppConfiguration),
                                         /*.mLibraryDirectory=*/std::move(aDirectory)};
}

std::filesystem::path shared_library_path(const AppConfigurationWithDirectory& aAppConfiguration)
{
    return aAppConfiguration.mLibraryDirectory / aAppConfiguration.mConfiguration.mLibraryFileName;
}

AppConfigurationWithDirectory default_app_configuration(const std::filesystem::path& aSharedLibPath)
{
    auto tSerialCriterion =
        CriterionConfiguration{/*.mName=*/std::string{kDefaultName},
                               /*.mIsParallelized=*/false, /*.mFunctionName=*/std::string{kDefaultSerialFunctionName}};
    auto tParallelCriterion = CriterionConfiguration{/*.mName=*/std::string{kDefaultName}, /*.mIsParallelized=*/true,
                                                     /*.mFunctionName=*/std::string{kDefaultParallelFunctionName}};
    auto tAppConfiguration =
        AppConfiguration{/*.mName=*/std::string{kDefaultName}, /*mLibraryName=*/aSharedLibPath.filename().string(),
                         /*.mCriteria=*/{std::move(tSerialCriterion), std::move(tParallelCriterion)}};
    return {/*.mConfiguration=*/std::move(tAppConfiguration), /*.mLibraryDirectory=*/aSharedLibPath.parent_path()};
}

std::optional<std::string_view> function_name(const AppConfiguration& aAppConfiguration,
                                              const CriterionConfiguration& aCriterionConfiguration)
{
    const auto tCriterionConfigurationIter = std::find_if(
        aAppConfiguration.mCriteria.cbegin(), aAppConfiguration.mCriteria.cend(),
        [&aCriterionConfiguration](const auto& aCurrentCriterionConfiguration)
        {
            return aCriterionConfiguration.mIsParallelized == aCurrentCriterionConfiguration.mIsParallelized &&
                   aCriterionConfiguration.mName == aCurrentCriterionConfiguration.mName;
        });

    if (tCriterionConfigurationIter == aAppConfiguration.mCriteria.cend())
    {
        return std::nullopt;
    }
    else
    {
        return std::optional<std::string_view>{tCriterionConfigurationIter->mFunctionName};
    }
}

void AppConfigurationWriter::operator()(const std::filesystem::path& aFilePath) const
{
    save_configuration(mAppConfiguration, aFilePath);
}

}  // namespace plato::services
