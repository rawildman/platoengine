#include <gtest/gtest.h>

#include "Exception.hpp"
#include "InputGeneration.hpp"
#include "ObjectiveFactory.hpp"
#include "ValidatedInput.hpp"

namespace plato::functional::integration_tests::serial
{
namespace
{
Plato::Functional::Validation::ValidatedInput create_two_objective_test_input()
{
    namespace pfv = Plato::Functional::Validation;
    namespace pftu = Plato::Functional::TestUtilities;

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
    const std::string tOptimizerInput = pftu::create_valid_example_optimization_parameters_string();

    return pfv::parse_and_validate(tObjectiveInput + tGeometryInput + tOptimizerInput);
}
}  // namespace

TEST(ObjectiveFactory, ValidAggregate)
{
    namespace pfv = Plato::Functional::Validation;

    const pfv::ValidatedInput tData = create_two_objective_test_input();

    EXPECT_EQ(tData.objectives().rawInput().size(), 2);
    const Plato::Functional::Aggregate tAggregate = criteria::library::detail::make_aggregate(tData.objectives());
    EXPECT_EQ(tAggregate.size(), 2);

    const std::vector tExpected = {42.0, 13.0};
    EXPECT_EQ(tAggregate.weights(), tExpected);
}

TEST(ObjectiveFactory, ValidParallelAggregate)
{
    namespace pfv = Plato::Functional::Validation;

    const pfv::ValidatedInput tData = create_two_objective_test_input();

    EXPECT_EQ(tData.objectives().rawInput().size(), 2);
    const Plato::Functional::ParallelAggregate tAggregate =
        criteria::library::detail::make_parallel_aggregate(tData.objectives());
    EXPECT_EQ(tAggregate.size(), 2);
}

TEST(ObjectiveFactory, ValidAggregateOneObjective)
{
    namespace pfv = Plato::Functional::Validation;
    namespace pftu = Plato::Functional::TestUtilities;

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
    const std::string tOptimizerInput = pftu::create_valid_example_optimization_parameters_string();

    const pfv::ValidatedInput tData = pfv::parse_and_validate(tObjectiveInput + tGeometryInput + tOptimizerInput);

    EXPECT_EQ(tData.objectives().rawInput().size(), 2);
    const Plato::Functional::Aggregate tAggregate = criteria::library::detail::make_aggregate(tData.objectives());
    EXPECT_EQ(tAggregate.size(), 1);

    const std::vector tExpected = {13.0};
    EXPECT_EQ(tAggregate.weights(), tExpected);
}
}  // namespace plato::functional::integration_tests::serial
