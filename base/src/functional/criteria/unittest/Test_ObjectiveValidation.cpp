#include <gtest/gtest.h>

#include "ObjectiveValidation.hpp"
#include "Plato_InputBlocks.hpp"

TEST(ObjectiveValidation, ValidateApp)
{
    namespace pfc = Plato::Functional::Criteria;
    Plato::objective tObjective;
    EXPECT_TRUE(pfc::validate_app(tObjective).has_value());
    tObjective.app = Plato::CodeOptions::kCustomApp;
    EXPECT_FALSE(pfc::validate_app(tObjective).has_value());
}

TEST(ObjectiveValidation, ValidateCustomApp)
{
    namespace pfc = Plato::Functional::Criteria;
    Plato::objective tObjective;
    tObjective.app = Plato::CodeOptions::kCustomApp;
    EXPECT_TRUE(pfc::validate_custom_app(tObjective).has_value());
    tObjective.shared_library_path = Plato::FileName{"/sweet/potato/ravioli.so"};
    EXPECT_FALSE(pfc::validate_custom_app(tObjective).has_value());
}

TEST(ObjectiveValidation, ValidateAggregationWeight)
{
    namespace pfc = Plato::Functional::Criteria;
    Plato::objective tObjective;
    EXPECT_TRUE(pfc::validate_aggregation_weight(tObjective).has_value());
    tObjective.aggregation_weight = 13.0;
    EXPECT_FALSE(pfc::validate_aggregation_weight(tObjective).has_value());
}

TEST(ObjectiveValidation, ValidateAtLeastOneObjective)
{
    namespace pfc = Plato::Functional::Criteria;
    EXPECT_TRUE(pfc::validate_at_least_one_objective({}).has_value());
    Plato::objective tObjective;
    EXPECT_FALSE(pfc::validate_at_least_one_objective({tObjective}).has_value());
    tObjective.active = false;
    EXPECT_TRUE(pfc::validate_at_least_one_objective({tObjective}).has_value());
    EXPECT_TRUE(pfc::validate_at_least_one_objective({tObjective, tObjective}).has_value());

    Plato::objective tObjectiveTwo;
    tObjectiveTwo.active = true;
    EXPECT_FALSE(pfc::validate_at_least_one_objective({tObjective, tObjectiveTwo}).has_value());
}
