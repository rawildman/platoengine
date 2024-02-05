#include "HelmholtzFilter.hpp"

#include <filesystem>

#include "FilterInterface.hpp"
#include "FilterRegistration.hpp"
#include "InputEnumTypes.hpp"
#include "ValidationRegistration.hpp"
#include "ValidationUtilities.hpp"

namespace plato::functional::filter::extension
{
namespace
{
const auto kHelmholtzFilterLibName = std::filesystem::path{"libAnalyzeFunctionalInterface.so"};

[[maybe_unused]] static auto kHelmholtzFilterRegistration = library::FilterRegistration{
    Plato::kFilterTypesTable.toString(Plato::FilterTypes::kHelmholtz).value(), [](const Plato::density_topology& aInput)
    {
        return library::make_filter_function_from_interface(
            library::load_filter(aInput, kHelmholtzFilterLibName));
    }};

[[maybe_unused]] static auto kHelmholtzFilterValidationRegistration = Plato::Functional::Validation::Registration<Plato::density_topology>{
    [](const Plato::density_topology& aInput) { return validate_helmholtz_filter_radius(aInput); },
    [](const Plato::density_topology& aInput) { return validate_helmholtz_filter_boundary_sticking_penalty(aInput); }};
}  // namespace

std::optional<std::string> validate_helmholtz_filter_radius(const Plato::density_topology& aInput)
{
    namespace pfv = Plato::Functional::Validation;
    namespace pfc = Plato::Functional::Core;
    if (aInput.filter_type && aInput.filter_type.value() == Plato::FilterTypes::kHelmholtz)
    {
        return pfv::error_message_for_parameter_out_of_bounds(Plato::block_name<Plato::density_topology>(),
                                                              aInput.filter_radius, "filter_radius",
                                                              pfc::lower_bounded(pfc::Exclusive{0.0}));
    }
    else
    {
        return std::nullopt;
    }
}

[[nodiscard]] std::optional<std::string> validate_helmholtz_filter_boundary_sticking_penalty(
    const Plato::density_topology& aInput)
{
    namespace pfc = Plato::Functional::Core;
    if (aInput.filter_type && aInput.filter_type.value() == Plato::FilterTypes::kHelmholtz &&
        aInput.boundary_sticking_penalty)
    {
        // boundary_sticking_penalty is optional, so only validate bounds if it exists
        return Plato::Functional::Validation::error_message_for_parameter_out_of_bounds(
            Plato::block_name<Plato::density_topology>(), aInput.boundary_sticking_penalty, "boundary_sticking_penalty",
            pfc::unit_bounded());
    }
    else
    {
        return std::nullopt;
    }
}
}  // namespace Plato::Functional
