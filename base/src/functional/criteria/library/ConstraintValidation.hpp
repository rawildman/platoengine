#ifndef PLATO_FUNCTIONAL_CONSTRAINTVALIDATION
#define PLATO_FUNCTIONAL_CONSTRAINTVALIDATION

#include "Plato_InputBlocks.hpp"
#include "ValidationRegistration.hpp"

namespace Plato::Functional::Criteria
{

namespace detail
{

[[nodiscard]] std::optional<std::string> validate_equal_to(const Plato::constraint& aInput);

}  // namespace detail

[[nodiscard]] std::vector<std::string> validate_constraints(const std::vector<Plato::constraint>& aInput,
                                                            std::vector<std::string>&& aCurrentMessageList);

}  // namespace Plato::Functional::Criteria

#endif