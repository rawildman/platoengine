#ifndef PLATO_FUNCTIONAL_OBJECTIVEVALIDATION
#define PLATO_FUNCTIONAL_OBJECTIVEVALIDATION

#include "Plato_InputBlocks.hpp"
#include "ValidationRegistration.hpp"

namespace Plato::Functional::Criteria
{

namespace detail
{
std::optional<std::string> validate_app(const Plato::objective& aInput);
std::optional<std::string> validate_custom_app(const Plato::objective& aInput);
std::optional<std::string> validate_aggregation_weight(const Plato::objective& aInput);
std::optional<std::string> validate_at_least_one_objective(const std::vector<Plato::objective>& aInput);
}  // namespace detail

std::vector<std::string> validate_objectives(const std::vector<Plato::objective>& aInput,
                                             const std::vector<std::string>& aCurrentMessageList);

}  // namespace Plato::Functional::Criteria

#endif