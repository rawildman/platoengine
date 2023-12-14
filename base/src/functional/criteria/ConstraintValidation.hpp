#ifndef PLATO_FUNCTIONAL_CONSTRAINTVALIDATION
#define PLATO_FUNCTIONAL_CONSTRAINTVALIDATION

#include "Plato_InputBlocks.hpp"
#include "ValidationRegistration.hpp"

namespace Plato::Functional::Criteria
{

namespace detail
{

std::optional<std::string> validate_only_one_type(const Plato::constraint& aInput);

}  // namespace detail

std::vector<std::string> validate_constraints(const std::vector<Plato::constraint>& aInput,
                                              const std::vector<std::string>& aCurrentMessageList);

}  // namespace Plato::Functional::Criteria

#endif