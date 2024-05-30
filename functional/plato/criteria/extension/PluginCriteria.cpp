#include "plato/criteria/extension/PluginCriteria.hpp"

#include "plato/criteria/extension/SharedLibCriterion.hpp"
#include "plato/services/AppConfiguration.hpp"

namespace plato::criteria::extension
{
namespace
{
static const auto kNumberOfPluginsLoaded = register_plugin_apps();
}

std::size_t number_of_plugins_registered_at_startup() { return kNumberOfPluginsLoaded; }

std::size_t register_plugin_apps(const std::vector<std::filesystem::path>& aAdditionalSearchDirectories)
{
    const auto tAppConfigurations = services::app_configurations(aAdditionalSearchDirectories);
    for (const auto& tAppConfiguration : tAppConfigurations)
    {
        [[maybe_unused]] auto tAppRegistration = library::CriterionRegistration{
            tAppConfiguration.mName, [tAppConfiguration](const plato::criteria::library::CriterionInput& aInput) {
                return make_shared_lib_function(
                    SharedLibCriterion{tAppConfiguration.mLibraryFileName, aInput.mInputFiles.mList});
            }};
    }
    return tAppConfigurations.size();
}

}  // namespace plato::criteria::extension
