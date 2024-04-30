#include <gtest/gtest.h>

#include <filesystem>

#include "plato/criteria/library/ObjectiveFactory.hpp"
#include "plato/geometry/extension/BrickShapeGeometry.hpp"
#include "plato/integration_tests/utilities/CheckProcessorsMatchObjectives.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"
#include "plato/test_utilities/InputGeneration.hpp"
#include "plato/utilities/Exception.hpp"

namespace plato::integration_tests::parallel
{
namespace
{
constexpr auto kNumRanks = int{4};

process_manager::library::ValidatedInput create_one_objective_test_input()
{
    namespace pftu = plato::test_utilities;

    // Input for the actual test
    const std::string tObjectiveInput =
        R"(
          begin objective test1
            app nodal_sum
            aggregation_weight 42.0
          end
       )";
    // Other inputs to make sure we have valid input
    const std::string tGeometryInput = pftu::create_valid_density_topology_geometry_string();
    const std::string tOptimizerInput = pftu::create_valid_example_rol_optimization_string();

    return process_manager::library::parse_and_validate(tObjectiveInput + tGeometryInput + tOptimizerInput);
}
}  // namespace

TEST(ObjectiveFactory, MPISize)
{
    auto tComm = boost::mpi::communicator{};
    EXPECT_EQ(tComm.size(), kNumRanks);
}

TEST(ObjectiveFactory, InvalidParallelAggregate)
{
    EXPECT_THROW(const process_manager::library::ValidatedInput tData = create_one_objective_test_input(),
                 plato::utilities::Exception);
}

TEST(ObjectiveFactory, NumberOfProcessors)
{
    namespace pitu = plato::integration_tests::utilities;
    auto tInput = test_utilities::create_valid_example_input();
    {
        // Set number_of_processors to 4
        tInput.mObjectives.front().number_of_processors = static_cast<unsigned int>(kNumRanks);
        auto tValidInput = process_manager::library::make_validated_input(tInput);
        pitu::check_processors_match_objectives(
            criteria::library::number_of_processors_per_objective(tValidInput.objectives()), tValidInput.objectives());
    }
    {
        // Add another objective with 1 processor
        tInput.mObjectives.front().number_of_processors = static_cast<unsigned int>(kNumRanks) - 1u;
        tInput.mObjectives.push_back(test_utilities::create_valid_example_objective());
        auto tValidInput = process_manager::library::make_validated_input(tInput);
        pitu::check_processors_match_objectives(
            criteria::library::number_of_processors_per_objective(tValidInput.objectives()), tValidInput.objectives());
    }
    {
        // Deactivate one objective
        tInput.mObjectives.front().number_of_processors = static_cast<unsigned int>(kNumRanks);
        tInput.mObjectives.back().active = false;
        auto tValidInput = process_manager::library::make_validated_input(tInput);
        pitu::check_processors_match_objectives(
            criteria::library::number_of_processors_per_objective(tValidInput.objectives()), tValidInput.objectives());
    }
}

TEST(ObjectiveFactory, EvaluateParallelMassApp)
{
    auto tObjective = input_parser::objective{};
    constexpr auto tNumGroups = 2;
    tObjective.number_of_processors = kNumRanks / tNumGroups;
    tObjective.aggregation_weight = 1.0;
    tObjective.app = input_parser::CodeOptions::kCustomApp;
    tObjective.shared_library_path = input_parser::FileName{"libPlatoTestMassObjective.so"};
    tObjective.name = "test_1";
    auto tInput = input_parser::ParsedInput{};
    tInput.mObjectives.push_back(tObjective);
    tObjective.name = "test_2";
    tInput.mObjectives.push_back(tObjective);
    tInput.mBrickShapeGeometry = test_utilities::create_valid_brick_shape_geometry();
    tInput.mROLOptimization = test_utilities::create_valid_example_rol_optimization();

    const auto tValidInput = process_manager::library::make_validated_input(tInput);

    const auto tObjectiveFunction = criteria::library::make_aggregate_objective_function(tValidInput.objectives());
    const auto tMeshFileName = tInput.mBrickShapeGeometry->mesh_name.value().mName;
    const auto tGeometry =
        geometry::extension::make_brick_shape_geometry(geometry::extension::BrickShapeGeometry{tMeshFileName});

    constexpr auto tX = double{2.0};
    constexpr auto tY = double{4.0};
    constexpr auto tZ = double{6.0};
    constexpr auto tCenterCoordinate = double{0.0};
    const auto tControls =
        linear_algebra::DynamicVector<double>{tCenterCoordinate, tCenterCoordinate, tCenterCoordinate, tX, tY, tZ};
    const auto tExpectedValue = tX * tY * tZ * tNumGroups;
    const auto tResult = tObjectiveFunction.f(tGeometry.f(tControls));
    // const auto tResult = double{42.0};
    EXPECT_EQ(tResult, tExpectedValue);

    std::filesystem::remove(tMeshFileName);
}

}  // namespace plato::integration_tests::parallel
