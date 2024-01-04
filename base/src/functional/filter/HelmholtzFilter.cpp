#include "HelmholtzFilter.hpp"

#include <filesystem>

#include "FilterInterface.hpp"
#include "FilterRegistration.hpp"
#include "Plato_InputEnumTypes.hpp"
#include "ValidationRegistration.hpp"
#include "ValidationUtilities.hpp"

namespace Plato::Functional
{
namespace
{
const auto kHelmholtzFilterLibName = std::filesystem::path{"libAnalyzeFunctionalInterface.so"};

[[maybe_unused]] static auto kHelmholtzFilterRegistration = Plato::Functional::FilterFactory::FilterRegistration{
    Plato::kFilterTypesTable.toString(Plato::FilterTypes::kHelmholtz).value(), [](const Plato::density_topology& aInput)
    {
        return FilterFactory::make_filter_function_from_interface(
            FilterFactory::load_filter(aInput, kHelmholtzFilterLibName));
    }};

[[maybe_unused]] static auto kHelmholtzFilterValidationRegistration = Validation::Registration<Plato::density_topology>{
    [](const Plato::density_topology& aInput) { return validate_helmholtz_filter_radius(aInput); },
    [](const Plato::density_topology& aInput) { return validate_helmholtz_filter_boundary_sticking_penalty(aInput); }};
}  // namespace

std::optional<std::string> validate_helmholtz_filter_radius(const Plato::density_topology& aInput)
{
    if (aInput.filter_type && aInput.filter_type.value() == Plato::FilterTypes::kHelmholtz)
    {
        return Plato::Functional::Validation::error_message_for_parameter_out_of_bounds(
            Plato::block_name<Plato::density_topology>(), aInput.filter_radius, "filter_radius", 1e-16, std::nullopt);
    }
    else
    {
        return std::nullopt;
    }
}

[[nodiscard]] std::optional<std::string> validate_helmholtz_filter_boundary_sticking_penalty(
    const Plato::density_topology& aInput)
{
    if (aInput.filter_type && aInput.filter_type.value() == Plato::FilterTypes::kHelmholtz &&
        aInput.boundary_sticking_penalty)
    {
        // boundary_sticking_penalty is optional, so only validate bounds if it exists
        return Plato::Functional::Validation::error_message_for_parameter_out_of_bounds(
            Plato::block_name<Plato::density_topology>(), aInput.boundary_sticking_penalty, "boundary_sticking_penalty",
            0.0, std::nullopt);
    }
    else
    {
        return std::nullopt;
    }
}
}  // namespace Plato::Functional
