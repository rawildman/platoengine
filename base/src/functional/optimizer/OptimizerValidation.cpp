#include "OptimizerValidation.hpp"

#include "Plato_InputBlocks.hpp"
#include "ValidationRegistration.hpp"

namespace Plato::Functional::Optimizer
{
[[maybe_unused]] static auto kOptimizerValidationRegistration =
    Plato::Functional::Validation::Registration<Plato::optimization_parameters>{
        [](const Plato::optimization_parameters& aInput)
        { return detail::validate_max_iterations(aInput); },
        [](const Plato::optimization_parameters& aInput)
        { return detail::validate_step_tolerance(aInput); },
        [](const Plato::optimization_parameters& aInput)
        { return detail::validate_gradient_tolerance(aInput); }};

std::vector<std::string> validate_optimization_parameters(const Plato::optimization_parameters& aInput,
                                                          std::vector<std::string>&& aCurrentMessageList)
{
    return Plato::Functional::Validation::validate<Plato::optimization_parameters>(aInput, std::move(aCurrentMessageList));
}

namespace detail
{

std::optional<std::string> validate_max_iterations(const Plato::optimization_parameters& aInput)
{
    return Plato::Functional::Validation::error_message_for_empty_parameter(Plato::block_name<Plato::optimization_parameters>(),
                                                                            aInput.max_iterations, "max_iterations");
}

[[nodiscard]] std::optional<std::string> validate_step_tolerance(const Plato::optimization_parameters& aInput)
{
    return Plato::Functional::Validation::error_message_for_empty_parameter(Plato::block_name<Plato::optimization_parameters>(),
                                                                            aInput.step_tolerance, "step_tolerance");
}

[[nodiscard]] std::optional<std::string> validate_gradient_tolerance(const Plato::optimization_parameters& aInput)
{
    return Plato::Functional::Validation::error_message_for_empty_parameter(Plato::block_name<Plato::optimization_parameters>(),
                                                                            aInput.gradient_tolerance, "gradient_tolerance");
}

}  // namespace detail
}  // namespace Plato::Functional::Optimizer