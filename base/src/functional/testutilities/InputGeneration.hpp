#ifndef PLATO_FUNCTIONAL_INPUTGENERATION
#define PLATO_FUNCTIONAL_INPUTGENERATION

#include <filesystem>
#include <string>

#include "Plato_InputBlocks.hpp"

namespace Plato::Functional::TestUtilities
{

void create_input_file(const std::filesystem::path aTestFileName);

Plato::brick_shape_geometry create_valid_brick_shape_geometry();
Plato::density_topology create_valid_density_topology_geometry();

Plato::constraint create_valid_example_constraint();
std::string create_valid_example_constraint_string();
Plato::objective create_valid_example_objective();
std::string create_valid_example_objective_string();

}  // namespace Plato::Functional::TestUtilities
#endif