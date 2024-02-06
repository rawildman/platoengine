#include "CriterionRegistration.hpp"

#include "InputBlocks.hpp"

namespace plato::functional::criteria::library
{
bool is_criterion_function_registered(const std::string_view aFunctionName)
{
    return core::is_factory_function_registered<CriterionFunction, CriterionInput>(aFunctionName);
}
}  // namespace plato::functional::criteria::library
