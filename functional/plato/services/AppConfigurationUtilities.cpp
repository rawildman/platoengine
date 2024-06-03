#include "plato/services/AppConfigurationUtilities.hpp"

namespace plato::services
{
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

}  // namespace plato::services
