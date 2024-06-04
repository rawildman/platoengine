#include "plato/services/AppSharedLibrary.hpp"

#include "plato/services/AppConfigurationUtilities.hpp"

namespace plato::services
{
AppSharedLibrary::AppSharedLibrary(AppConfigurationWithDirectory aAppConfiguration)
    : mAppConfiguration{std::move(aAppConfiguration)}, mSharedLibrary{shared_library_path(mAppConfiguration)}
{
}
}  // namespace plato::services
