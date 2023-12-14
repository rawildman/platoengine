#ifndef PLATO_FUNCTIONAL_INPUTGENERATION
#define PLATO_FUNCTIONAL_INPUTGENERATION

#include <filesystem>
#include <string>

#include "Plato_InputBlocks.hpp"

namespace Plato::Functional::TestUtilities
{

void create_input_file(const std::filesystem::path aTestFileName);

Plato::constraint create_valid_example_constraint();
std::string create_valid_example_constraint_string();
Plato::objective create_valid_example_objective();
std::string create_valid_example_objective_string();

}  // namespace Plato::Functional::TestUtilities
#endif