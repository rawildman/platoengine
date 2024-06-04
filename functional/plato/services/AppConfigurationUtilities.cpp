#include "plato/services/AppConfigurationUtilities.hpp"

namespace plato::services
{
namespace
{
constexpr auto kDefaultName = std::string_view{"default"};
constexpr auto kDefaultSerialFunctionName = std::string_view{"plato_create_criterion"};
constexpr auto kDefaultParallelFunctionName = std::string_view{"plato_create_parallel_criterion"};
}  // namespace

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
    auto tSerialCriterion = CriterionConfiguration{/*.mName=*/std::string{kDefaultName},
                                                   std::string{kDefaultSerialFunctionName}, /*.mIsParallelized=*/false};
    auto tParallelCriterion = CriterionConfiguration{
        /*.mName=*/std::string{kDefaultName}, std::string{kDefaultParallelFunctionName}, /*.mIsParallelized=*/true};
    auto tAppConfiguration =
        AppConfiguration{/*.mName=*/std::string{kDefaultName}, /*mLibraryName=*/aSharedLibPath.filename().string(),
                         /*.mCriteria=*/{std::move(tSerialCriterion), std::move(tParallelCriterion)}};
    return {/*.mConfiguration=*/std::move(tAppConfiguration), /*.mLibraryDirectory=*/aSharedLibPath.parent_path()};
}
}  // namespace plato::services
