#include "FilterValidation.hpp"

#include "InputBlocks.hpp"
#include "ValidationRegistration.hpp"
#include "ValidationUtilities.hpp"

namespace plato::functional::filter::library
{
namespace
{
[[maybe_unused]] static auto kFilterValidationRegistration = Plato::Functional::Validation::Registration<Plato::density_topology>{
    [](const Plato::density_topology& aInput) { return validate_filter_type(aInput); }};
}

std::optional<std::string> validate_filter_type(const Plato::density_topology& aInput)
{
    if (!aInput.filter_type)
    {
        return Plato::Functional::Validation::error_message_for_empty_parameter(
            Plato::block_name<Plato::density_topology>(), aInput.filter_type, "filter_type");
    }
    else
    {
        return std::nullopt;
    }
}
}  // namespace plato::functional::filter::library
