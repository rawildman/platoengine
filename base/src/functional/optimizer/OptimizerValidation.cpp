#include "OptimizerValidation.hpp"

#include "InputBlocks.hpp"
#include "ValidationRegistration.hpp"
#include "ValidationUtilities.hpp"

namespace plato::functional::optimizer
{
[[maybe_unused]] static auto kOptimizerValidationRegistration =
    Plato::Functional::Validation::Registration<Plato::optimization_parameters>{
        [](const Plato::optimization_parameters& aInput) { return detail::validate_max_iterations(aInput); },
        [](const Plato::optimization_parameters& aInput) { return detail::validate_step_tolerance(aInput); },
        [](const Plato::optimization_parameters& aInput) { return detail::validate_gradient_tolerance(aInput); }};

std::vector<std::string> validate_optimization_parameters(const Plato::optimization_parameters& aInput,
                                                          std::vector<std::string>&& aCurrentMessageList)
{
    return Plato::Functional::Validation::validate(aInput, std::move(aCurrentMessageList));
}

namespace detail
{
std::optional<std::string> validate_max_iterations(const Plato::optimization_parameters& aInput)
{
    namespace pfv = Plato::Functional::Validation;
    namespace pfu = plato::functional::utilities;
    if (!aInput.input_file_name || (aInput.input_file_name && aInput.max_iterations))
    {
        return pfv::error_message_for_parameter_out_of_bounds(Plato::block_name<Plato::optimization_parameters>(),
                                                              aInput.max_iterations, "max_iterations",
                                                              pfu::lower_bounded(pfu::Inclusive{1u}));
    }
    else
    {
        return std::nullopt;
    }
}

[[nodiscard]] std::optional<std::string> validate_step_tolerance(const Plato::optimization_parameters& aInput)
{
    namespace pfv = Plato::Functional::Validation;
    namespace pfu = plato::functional::utilities;
    if (!aInput.input_file_name || (aInput.input_file_name && aInput.step_tolerance))
    {
        return pfv::error_message_for_parameter_out_of_bounds(Plato::block_name<Plato::optimization_parameters>(),
                                                              aInput.step_tolerance, "step_tolerance",
                                                              pfu::lower_bounded(pfu::Exclusive{0.0}));
    }
    else
    {
        return std::nullopt;
    }
}

[[nodiscard]] std::optional<std::string> validate_gradient_tolerance(const Plato::optimization_parameters& aInput)
{
    namespace pfv = Plato::Functional::Validation;
    namespace pfu = plato::functional::utilities;
    if (!aInput.input_file_name || (aInput.input_file_name && aInput.gradient_tolerance))
    {
        return Plato::Functional::Validation::error_message_for_parameter_out_of_bounds(
            Plato::block_name<Plato::optimization_parameters>(), aInput.gradient_tolerance, "gradient_tolerance",
            pfu::lower_bounded(pfu::Exclusive{0.0}));
    }
    else
    {
        return std::nullopt;
    }
}

}  // namespace detail
}  // namespace plato::functional::optimizer
