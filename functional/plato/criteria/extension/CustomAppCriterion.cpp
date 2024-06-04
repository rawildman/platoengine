#include "plato/criteria/extension/CustomAppCriterion.hpp"

#include "plato/criteria/extension/SharedLibCriterion.hpp"
#include "plato/criteria/library/CriterionRegistration.hpp"
#include "plato/services/AppConfigurationUtilities.hpp"

namespace plato::criteria::extension
{
namespace
{
template <typename... Args>
[[nodiscard]] SharedLibCriterion make_shared_lib_criterion(const plato::criteria::library::CriterionInput& aInput,
                                                           Args&&... aArgs)
{
    return SharedLibCriterion{services::default_app_configuration(aInput.mSharedLibraryPath.mToken),
                              aInput.mInputFiles.mList, std::forward<Args>(aArgs)...};
}

// Criterion registration
[[maybe_unused]] static auto kCustomAppRegistration = library::CriterionRegistration{
    std::string{detail::custom_app_name()}, [](const plato::criteria::library::CriterionInput& aInput)
    { return make_shared_lib_function(make_shared_lib_criterion(aInput)); }};

[[maybe_unused]] static auto kParallelCustomAppRegistration = library::ParallelCriterionRegistration{
    std::string{detail::custom_app_name()},
    [](const plato::criteria::library::CriterionInput& aInput, const boost::mpi::communicator& aComm)
    { return make_shared_lib_function(make_shared_lib_criterion(aInput, aComm)); }};

// Validation registration
[[maybe_unused]] static auto kObjectiveValidationRegistration = core::ValidationRegistration<input_parser::objective>{
    [](const input_parser::objective& aInput) { return detail::validate_custom_app(aInput); }};

[[maybe_unused]] static auto kConstraintValidationRegistration = core::ValidationRegistration<input_parser::constraint>{
    [](const input_parser::constraint& aInput) { return detail::validate_custom_app(aInput); }};
}  // namespace

}  // namespace plato::criteria::extension
