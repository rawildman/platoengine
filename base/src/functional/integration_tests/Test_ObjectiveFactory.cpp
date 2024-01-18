#include <gtest/gtest.h>

#include "Exception.hpp"
#include "ObjectiveFactory.hpp"
#include "ValidatedInput.hpp"
#include "InputGeneration.hpp"

TEST(ObjectiveFactory, ValidAggregate)
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

    const pfv::ValidatedInput tData = pfv::parse_and_validate(tObjectiveInput + tGeometryInput + tOptimizerInput);

    EXPECT_EQ(tData.objectives().rawInput().size(), 2);
    const Plato::Functional::Aggregate tAggregate =
        Plato::Functional::ObjectiveFactory::detail::make_aggregate(tData.objectives());
    EXPECT_EQ(tAggregate.size(), 2);

    const std::vector tExpected = {42.0, 13.0};
    EXPECT_EQ(tAggregate.weights(), tExpected);
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
    const Plato::Functional::Aggregate tAggregate =
        Plato::Functional::ObjectiveFactory::detail::make_aggregate(tData.objectives());
    EXPECT_EQ(tAggregate.size(), 1);

    const std::vector tExpected = {13.0};
    EXPECT_EQ(tAggregate.weights(), tExpected);
}
