#ifndef PLATO_FUNCTIONAL_CRITERIA_LIBRARY_CONSTRAINTVALIDATION
#define PLATO_FUNCTIONAL_CRITERIA_LIBRARY_CONSTRAINTVALIDATION

#include "core/ValidationRegistration.hpp"
#include "input_parser/InputBlocks.hpp"

namespace plato::functional::criteria::library
{
namespace detail
{
[[nodiscard]] std::optional<std::string> validate_equal_to(const input_parser::constraint& aInput);

}  // namespace detail

[[nodiscard]] std::vector<std::string> validate_constraints(const std::vector<input_parser::constraint>& aInput,
                                                            std::vector<std::string>&& aCurrentMessageList);

}  // namespace plato::functional::criteria::library

#endif