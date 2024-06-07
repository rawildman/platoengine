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
                                            const services::CriterionConfiguration& aCriterionConfiguration,
                                            const criteria::library::CriterionInput& aInput,
                                            Args&&... aAdditionalArgs)
{
    return make_shared_lib_function(SharedLibCriterion{
        aAppConfiguration, aCriterionConfiguration, aInput.mInputFiles.mList, std::forward<Args>(aAdditionalArgs)...});
}

void register_all_criteria(const services::AppConfigurationWithDirectory& aAppConfiguration)
{
    for (const auto& tCriterionConfiguration : aAppConfiguration.mConfiguration.mCriteria)
    {
        if (tCriterionConfiguration.mIsParallelized)
        {
            [[maybe_unused]] auto tAppRegistration = library::ParallelCriterionRegistration{
                library::criterion_registration_name(aAppConfiguration.mConfiguration, tCriterionConfiguration),
                [aAppConfiguration, tCriterionConfiguration](const criteria::library::CriterionInput& aInput,
                                                             const boost::mpi::communicator& aComm)
                { return make_plugin_app_function(aAppConfiguration, tCriterionConfiguration, aInput, aComm); }};
        }
        else
        {
            [[maybe_unused]] auto tAppRegistration = library::CriterionRegistration{
                library::criterion_registration_name(aAppConfiguration.mConfiguration, tCriterionConfiguration),
                [aAppConfiguration, tCriterionConfiguration](const criteria::library::CriterionInput& aInput)
                { return make_plugin_app_function(aAppConfiguration, tCriterionConfiguration, aInput); }};
        }
    }
}
}  // namespace

std::size_t number_of_plugins_registered_at_startup() { return kNumberOfPluginsLoaded; }

std::size_t register_plugin_apps(const std::vector<std::filesystem::path>& aAdditionalSearchDirectories)
{
    const auto tAppConfigurations = services::app_configurations(aAdditionalSearchDirectories);
    for (const auto& tAppConfiguration : tAppConfigurations)
    {
        register_all_criteria(tAppConfiguration);
    }
    return tAppConfigurations.size();
}

}  // namespace plato::criteria::extension
