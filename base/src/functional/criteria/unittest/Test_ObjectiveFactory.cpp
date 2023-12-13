#include <gtest/gtest.h>

#include "Exception.hpp"
#include "InputParser.hpp"
#include "ObjectiveFactory.hpp"

TEST(ObjectiveFactory, ValidAggregate)
{
    constexpr std::string_view tInput =
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

    const Plato::PlatoInput tData = Plato::Functional::parse_input(tInput);

    EXPECT_EQ(tData.mObjectives.size(), 2);
    const Plato::Functional::Aggregate tAggregate =
        Plato::Functional::ObjectiveFactory::detail::make_aggregate(tData.mObjectives);
    EXPECT_EQ(tAggregate.size(), 2);

    const std::vector tExpected = {42.0, 13.0};
    EXPECT_EQ(tAggregate.weights(), tExpected);
}

TEST(ObjectiveFactory, ValidAggregateOneObjective)
{
    constexpr std::string_view tInput =
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

    const Plato::PlatoInput tData = Plato::Functional::parse_input(tInput);

    EXPECT_EQ(tData.mObjectives.size(), 2);
    const Plato::Functional::Aggregate tAggregate =
        Plato::Functional::ObjectiveFactory::detail::make_aggregate(tData.mObjectives);
    EXPECT_EQ(tAggregate.size(), 1);

    const std::vector tExpected = {13.0};
    EXPECT_EQ(tAggregate.weights(), tExpected);
}