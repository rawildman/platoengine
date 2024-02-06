#ifndef PLATO_FUNCTIONAL_OPTIMIZER_OPTIMIZERVALIDATION
#define PLATO_FUNCTIONAL_OPTIMIZER_OPTIMIZERVALIDATION

#include <vector>

#include "InputBlocks.hpp"

namespace plato::functional::optimizer
{
[[nodiscard]] std::vector<std::string> validate_optimization_parameters(
    const input_parser::optimization_parameters& aInput, std::vector<std::string>&& aCurrentMessageList);

namespace detail
{
[[nodiscard]] std::optional<std::string> validate_max_iterations(const input_parser::optimization_parameters& aInput);
[[nodiscard]] std::optional<std::string> validate_step_tolerance(const input_parser::optimization_parameters& aInput);
[[nodiscard]] std::optional<std::string> validate_gradient_tolerance(
    const input_parser::optimization_parameters& aInput);

}  // namespace detail

}  // namespace plato::functional::optimizer

#endif