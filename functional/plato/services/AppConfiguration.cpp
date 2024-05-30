#include "plato/services/AppConfiguration.hpp"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <fstream>

namespace plato::services
{
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
