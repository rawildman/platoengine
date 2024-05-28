#include "plato/services/AppConfiguration.hpp"

namespace plato::services
{
bool operator==(const AppConfiguration& aAppConfigurationLeft, const AppConfiguration& aAppConfigurationRight)
{
    return aAppConfigurationLeft.mName == aAppConfigurationRight.mName &&
           aAppConfigurationLeft.mLibraryFileName == aAppConfigurationRight.mLibraryFileName &&
           aAppConfigurationLeft.mHasParallelImplementation == aAppConfigurationRight.mHasParallelImplementation &&
           aAppConfigurationLeft.mHasSerialImplementation == aAppConfigurationRight.mHasSerialImplementation;
}
}  // namespace plato::services
