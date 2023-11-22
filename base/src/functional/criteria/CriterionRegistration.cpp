#include "CriterionRegistration.hpp"

#include "Plato_InputBlocks.hpp"

namespace Plato::Functional::CriterionFactory
{
bool is_criterion_function_registered(const std::string_view aFunctionName)
{
    return is_function_registered<CriterionFunction, CriterionInput>(aFunctionName);
}
}  // namespace Plato::Functional::CriterionFactory
