#include "plato/criteria/extension/PluginCriteria.hpp"

#include <utility>

#include "plato/criteria/extension/SharedLibCriterion.hpp"
#include "plato/services/AppConfiguration.hpp"
#include "plato/services/AppConfigurationUtilities.hpp"

namespace plato::criteria::extension
{
namespace
{
static const auto kNumberOfPluginsLoaded = register_plugin_apps();

template <typename... Args>
[[nodiscard]] auto make_plugin_app_function(const services::AppConfigurationWithDirectory& aAppConfiguration,
                                            const criteria::library::CriterionInput& aInput,
                                            Args&&... aAdditionalArgs)
{
    return make_shared_lib_function(SharedLibCriterion{services::shared_library_path(aAppConfiguration),
                                                       aInput.mInputFiles.mList,
                                                       std::forward<Args>(aAdditionalArgs)...});
}
}  // namespace

std::size_t number_of_plugins_registered_at_startup() { return kNumberOfPluginsLoaded; }

std::size_t register_plugin_apps(const std::vector<std::filesystem::path>& aAdditionalSearchDirectories)
{
    const auto tAppConfigurations = services::app_configurations(aAdditionalSearchDirectories);
    for (const auto& tAppConfiguration : tAppConfigurations)
    {
        assert(!tAppConfiguration.mConfiguration.mCriteria.empty());
        if (tAppConfiguration.mConfiguration.mCriteria.front().mIsParallelized)
        {
            [[maybe_unused]] auto tAppRegistration = library::ParallelCriterionRegistration{
                tAppConfiguration.mConfiguration.mName,
                [tAppConfiguration](const criteria::library::CriterionInput& aInput,
                                    const boost::mpi::communicator& aComm)
                { return make_plugin_app_function(tAppConfiguration, aInput, aComm); }};
        }
        else
        {
            [[maybe_unused]] auto tAppRegistration =
                library::CriterionRegistration{tAppConfiguration.mConfiguration.mName,
                                               [tAppConfiguration](const criteria::library::CriterionInput& aInput)
                                               { return make_plugin_app_function(tAppConfiguration, aInput); }};
        }
    }
    return tAppConfigurations.size();
}

}  // namespace plato::criteria::extension
