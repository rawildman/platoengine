#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "InputParser.hpp"

namespace
{
const std::filesystem::path kTestFileName = "testInput.i";

void createInput()
{
    std::ofstream tOutFile(kTestFileName);
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

}  // namespace

TEST(InputParser, ParseFromFile)
{
    createInput();
    const Plato::PlatoInput tInput = Plato::Functional::parse_input_from_file(kTestFileName);

    EXPECT_FALSE(tInput.mOptimizationParameters.max_iterations.has_value());
    ASSERT_TRUE(tInput.mOptimizationParameters.gradient_tolerance.has_value());
    EXPECT_EQ(tInput.mOptimizationParameters.gradient_tolerance.value(), 100.0);
    ASSERT_TRUE(tInput.mOptimizationParameters.step_tolerance.has_value());
    EXPECT_EQ(tInput.mOptimizationParameters.step_tolerance.value(), 10.0);

    ASSERT_EQ(tInput.mObjectives.size(), 1);
    const Plato::objective& tObjective = tInput.mObjectives.front();
    ASSERT_TRUE(tObjective.active.has_value());
    EXPECT_TRUE(tObjective.active.value());
    ASSERT_TRUE(tObjective.app.has_value());
    EXPECT_EQ(tObjective.app.value(), Plato::CodeOptions::kNodalSum);
    ASSERT_TRUE(tObjective.number_of_processors.has_value());
    EXPECT_EQ(tObjective.number_of_processors.value(), 4);
    ASSERT_TRUE(tObjective.input_files.has_value());
    ASSERT_EQ(tObjective.input_files->mList.size(), 1);
    EXPECT_EQ(tObjective.input_files->mList.front(), "test-input.inp");
    ASSERT_TRUE(tObjective.aggregation_weight.has_value());
    EXPECT_EQ(tObjective.aggregation_weight.value(), 42.0);
    ASSERT_TRUE(tObjective.objective_type.has_value());
    EXPECT_EQ(tObjective.objective_type.value(), Plato::ObjectiveTypes::kMinimize);

    EXPECT_EQ(tInput.mConstraints.size(), 0);

    ASSERT_TRUE(tInput.mBrickShapeGeometry.has_value());
    ASSERT_TRUE(tInput.mBrickShapeGeometry->mesh_name.has_value());
    EXPECT_EQ(tInput.mBrickShapeGeometry->mesh_name->mName, "my_mesh.exo");

    std::filesystem::remove(kTestFileName);
}
