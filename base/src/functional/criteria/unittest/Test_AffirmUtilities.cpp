#include <gtest/gtest.h>

#include "AffirmUtilities.hpp"
#include "Exception.hpp"
#include "InputParser.hpp"

TEST(AffirmUtilities, ActiveConstraint)
{
    namespace pfa = Plato::Functional::Affirmations;
    Plato::constraint tConstraintInput;
    EXPECT_TRUE(pfa::is_active(tConstraintInput));
    tConstraintInput.active = true;
    EXPECT_TRUE(pfa::is_active(tConstraintInput));
    tConstraintInput.active = false;
    EXPECT_FALSE(pfa::is_active(tConstraintInput));
}

TEST(AffirmUtilities, ActiveObjective)
{
    namespace pfa = Plato::Functional::Affirmations;
    Plato::objective tObjectiveInput;
    EXPECT_TRUE(pfa::is_active(tObjectiveInput));
    tObjectiveInput.active = true;
    EXPECT_TRUE(pfa::is_active(tObjectiveInput));
    tObjectiveInput.active = false;
    EXPECT_FALSE(pfa::is_active(tObjectiveInput));
}

TEST(AffirmUtilities, AffirmParameterExistsDNE)
{
    namespace pfa = Plato::Functional::Affirmations;
    Plato::objective tObjectiveInput;
    EXPECT_TRUE(
        pfa::error_message_for_empty_parameter("Objective: ", tObjectiveInput.aggregation_weight, "aggregation_weight")
            .has_value());
}

TEST(AffirmUtilities, AffirmParameterExistsDoesExist)
{
    namespace pfa = Plato::Functional::Affirmations;
    Plato::objective tObjectiveInput;
    tObjectiveInput.aggregation_weight = 23;
    EXPECT_FALSE(
        pfa::error_message_for_empty_parameter("Objective: ", tObjectiveInput.aggregation_weight, "aggregation_weight")
            .has_value());
}