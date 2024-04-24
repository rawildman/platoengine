#include <gtest/gtest.h>

#include "plato/criteria/library/ObjectiveFactory.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"
#include "plato/test_utilities/InputGeneration.hpp"
#include "plato/utilities/Exception.hpp"

namespace plato::integration_tests::serial
{
namespace
{
process_manager::library::ValidatedInput create_two_objective_test_input()
{
    namespace pftu = plato::test_utilities;

    // Input for the actual test
    const std::string tObjectiveInput =
        R"(
          begin objective test1
            app nodal_sum
            aggregation_weight 42.0
          end
          begin objective test2
            active true
            app nodal_sum
            aggregation_weight 13.0
          end
       )";
    // Other inputs to make sure we have valid input
    const std::string tGeometryInput = pftu::create_valid_density_topology_geometry_string();
    const std::string tOptimizerInput = pftu::create_valid_example_rol_optimization_string();

    return process_manager::library::parse_and_validate(tObjectiveInput + tGeometryInput + tOptimizerInput);
}
}  // namespace

TEST(ObjectiveFactory, ValidAggregate)
{
    const process_manager::library::ValidatedInput tData = create_two_objective_test_input();

    EXPECT_EQ(tData.objectives().rawInput().size(), 2);
    const core::Aggregate tAggregate = criteria::library::detail::make_aggregate(tData.objectives());
    EXPECT_EQ(tAggregate.size(), 2);

    const std::vector tExpected = {42.0, 13.0};
    EXPECT_EQ(tAggregate.weights(), tExpected);
}

TEST(ObjectiveFactory, ValidParallelAggregate)
{
    const process_manager::library::ValidatedInput tData = create_two_objective_test_input();

    EXPECT_EQ(tData.objectives().rawInput().size(), 2);
    const core::ParallelAggregate tAggregate = criteria::library::detail::make_parallel_aggregate(tData.objectives());
    EXPECT_EQ(tAggregate.size(), 2);
}

TEST(ObjectiveFactory, ValidAggregateOneObjective)
{
    namespace pftu = plato::test_utilities;

    // Input for the actual test
    const std::string tObjectiveInput =
        R"(
          begin objective test1
            active false
            app nodal_sum
            aggregation_weight 42.0
          end
          begin objective test2
            active true
            app nodal_sum
            aggregation_weight 13.0
          end
       )";
    // Other inputs to make sure we have valid input
    const std::string tGeometryInput = pftu::create_valid_density_topology_geometry_string();
    const std::string tOptimizerInput = pftu::create_valid_example_rol_optimization_string();

    const process_manager::library::ValidatedInput tData =
        process_manager::library::parse_and_validate(tObjectiveInput + tGeometryInput + tOptimizerInput);

    EXPECT_EQ(tData.objectives().rawInput().size(), 2);
    const core::Aggregate tAggregate = criteria::library::detail::make_aggregate(tData.objectives());
    EXPECT_EQ(tAggregate.size(), 1);

    const std::vector tExpected = {13.0};
    EXPECT_EQ(tAggregate.weights(), tExpected);
}

TEST(ObjectiveFactory, ParallelObjectives)
{
    auto tInput = test_utilities::create_valid_example_input();
    {
        // Check example, which sets number_of_processors to 42
        const auto tValidatedInput = process_manager::library::make_validated_input(tInput);
        EXPECT_EQ(criteria::library::total_number_of_processors(tValidatedInput.objectives()), 42u);
        EXPECT_TRUE(criteria::library::has_parallel_objective(tValidatedInput.objectives()));
    }
    {
        // Set number_of_processors to boost::none, default is 1
        tInput.mObjectives.front().number_of_processors = boost::none;
        const auto tValidatedInput = process_manager::library::make_validated_input(tInput);
        EXPECT_EQ(criteria::library::total_number_of_processors(tValidatedInput.objectives()), 1u);
        EXPECT_FALSE(criteria::library::has_parallel_objective(tValidatedInput.objectives()));
    }
    {
        // Set number_of_processors to 1
        tInput.mObjectives.front().number_of_processors = 1u;
        const auto tValidatedInput = process_manager::library::make_validated_input(tInput);
        EXPECT_EQ(criteria::library::total_number_of_processors(tValidatedInput.objectives()), 1u);
        EXPECT_FALSE(criteria::library::has_parallel_objective(tValidatedInput.objectives()));
    }
    {
        // Add another objective with 42 processors
        tInput.mObjectives.push_back(test_utilities::create_valid_example_objective());
        const auto tValidatedInput = process_manager::library::make_validated_input(tInput);
        EXPECT_EQ(criteria::library::total_number_of_processors(tValidatedInput.objectives()), 43u);
        EXPECT_TRUE(criteria::library::has_parallel_objective(tValidatedInput.objectives()));
    }
}

}  // namespace plato::integration_tests::serial
