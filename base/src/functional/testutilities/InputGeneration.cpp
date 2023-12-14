#include "InputGeneration.hpp"

#include <fstream>

namespace Plato::Functional::TestUtilities
{
const std::filesystem::path kTestFileName = "testInput.i";

void create_input_file(const std::filesystem::path aTestFileName)
{
    std::ofstream tOutFile(aTestFileName);
    const std::string tInput =
        R"(
          begin brick_shape_geometry
            mesh_name my_mesh.exo
          end
          begin objective test
            active true
            app nodal_sum
            number_of_processors 4
            input_files test-input.inp
            aggregation_weight 42.0
            objective_type minimize
          end
          begin optimization_parameters
            input_file_name its-a_file.txt
            step_tolerance 10
            gradient_tolerance 100.0
            
          end
       )";
    tOutFile << tInput << std::endl;
    tOutFile.close();
}

Plato::constraint create_valid_example_constraint()
{
    return Plato::constraint{/*.name=*/std::string{"bike-shed"},
                             /*.active=*/true,
                             /*.app=*/Plato::CodeOptions::kCustomApp,
                             /*.shared_library_path=*/Plato::FileName{"/sweet/potato/ravioli.so"},
                             /*.number_of_processors=*/42u,
                             /*.input_files=*/Plato::FileList{{"brown.txt", "butter.txt", "sauce.txt"}},
                             /*.equal_to=*/0.0,
                             /*.less_than=*/boost::none,
                             /*.greater_than=*/boost::none,
                             /*.is_linear=*/true};
}

std::string create_valid_example_constraint_string()
{
    return R"(
          begin constraint test
            active true
            app nodal_sum
            number_of_processors 4
            input_files test-input.inp
            equal_to 13
            is_linear true
          end
       )";
}

Plato::objective create_valid_example_objective()
{
    return Plato::objective{/*.name=*/std::string{"bike-shed"},
                            /*.active=*/true,
                            /*.app=*/Plato::CodeOptions::kCustomApp,
                            /*.shared_library_path=*/Plato::FileName{"/sweet/potato/ravioli.so"},
                            /*.number_of_processors=*/42u,
                            /*.input_files=*/Plato::FileList{{"brown.txt", "butter.txt", "sauce.txt"}},
                            /*.aggregation_weight=*/13.0,
                            /*.objective_type=*/Plato::ObjectiveTypes::kMaximize};
}

std::string create_valid_example_objective_string()
{
    return R"(
          begin objective test
            active true
            app nodal_sum
            number_of_processors 4
            input_files test-input.inp
            aggregation_weight 42.0
            objective_type minimize
          end
       )";
}

}  // namespace Plato::Functional::TestUtilities