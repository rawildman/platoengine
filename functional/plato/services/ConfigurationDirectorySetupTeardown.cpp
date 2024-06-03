#include "plato/services/ConfigurationDirectorySetupTeardown.hpp"

#include "plato/services/AppConfiguration.hpp"

namespace plato::services
{
namespace
{
constexpr auto kRootRank = int{0};
}
ConfigurationDirectorySetupTeardown::ConfigurationDirectorySetupTeardown(std::filesystem::path aDirectory,
                                                                         const boost::mpi::communicator& aComm)
    : mDirectory{std::move(aDirectory)}, mComm{aComm}
{
    if (mComm.rank() == kRootRank)
    {
        std::filesystem::create_directories(mDirectory);
    }
}

ConfigurationDirectorySetupTeardown::~ConfigurationDirectorySetupTeardown()
{
    if (mComm.rank() == kRootRank)
    {
        std::filesystem::remove_all(mDirectory);
    }
}

ConfigurationDirectorySetupTeardown& ConfigurationDirectorySetupTeardown::addConfiguration(
    const services::AppConfiguration& aConfiguration, const std::filesystem::path& aFilename)
{
    if (mComm.rank() == kRootRank)
    {
        services::save_configuration(aConfiguration, mDirectory / aFilename);
    }
    return *this;
}

const std::filesystem::path& ConfigurationDirectorySetupTeardown::directory() const { return mDirectory; }

}  // namespace plato::services
