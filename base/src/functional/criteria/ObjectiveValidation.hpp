#ifndef PLATO_FUNCTIONAL_OBJECTIVEVALIDATION
#define PLATO_FUNCTIONAL_OBJECTIVEVALIDATION

#include "Plato_InputBlocks.hpp"
#include "ValidationRegistration.hpp"

namespace Plato::Functional::Criteria
{

namespace detail
{
[[nodiscard]] std::optional<std::string> validate_aggregation_weight(const Plato::objective& aInput);
[[nodiscard]] std::optional<std::string> validate_at_least_one_objective(const std::vector<Plato::objective>& aInput);
}  // namespace detail

[[nodiscard]] std::vector<std::string> validate_objectives(const std::vector<Plato::objective>& aInput,
                                                           std::vector<std::string>&& aCurrentMessageList);

}  // namespace Plato::Functional::Criteria

#endif