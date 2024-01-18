#ifndef PLATO_FUNCTIONAL_OPTIMIZERVALIDATION
#define PLATO_FUNCTIONAL_OPTIMIZERVALIDATION

#include <vector>

#include "Plato_InputBlocks.hpp"

namespace Plato::Functional::Optimizer
{
[[nodiscard]] std::vector<std::string> validate_optimization_parameters(const Plato::optimization_parameters& aInput,
                                                                        std::vector<std::string>&& aCurrentMessageList);

namespace detail
{

[[nodiscard]] std::optional<std::string> validate_max_iterations(const Plato::optimization_parameters& aInput);
[[nodiscard]] std::optional<std::string> validate_step_tolerance(const Plato::optimization_parameters& aInput);
[[nodiscard]] std::optional<std::string> validate_gradient_tolerance(const Plato::optimization_parameters& aInput);

}  // namespace detail

}  // namespace Plato::Functional::Optimizer

#endif