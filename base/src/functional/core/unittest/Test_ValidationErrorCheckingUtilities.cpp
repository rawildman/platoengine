#include <gtest/gtest.h>

#include "Exception.hpp"
#include "InputParser.hpp"
#include "ValidationUtilities.hpp"

TEST(ValidateUtilities, ActiveConstraint)
{
    namespace pfv = Plato::Functional::Validation;
    Plato::constraint tConstraintInput;
    EXPECT_TRUE(pfv::is_active(tConstraintInput));
    tConstraintInput.active = true;
    EXPECT_TRUE(pfv::is_active(tConstraintInput));
    tConstraintInput.active = false;
    EXPECT_FALSE(pfv::is_active(tConstraintInput));
}

TEST(ValidateUtilities, ActiveObjective)
{
    namespace pfv = Plato::Functional::Validation;
    Plato::objective tObjectiveInput;
    EXPECT_TRUE(pfv::is_active(tObjectiveInput));
    tObjectiveInput.active = true;
    EXPECT_TRUE(pfv::is_active(tObjectiveInput));
    tObjectiveInput.active = false;
    EXPECT_FALSE(pfv::is_active(tObjectiveInput));
}

TEST(ValidateUtilities, ValidateParameterExistsWhenParameterDoesNotExist)
{
    namespace pfv = Plato::Functional::Validation;
    Plato::objective tObjectiveInput;
    EXPECT_TRUE(
        pfv::error_message_for_empty_parameter("Objective: ", tObjectiveInput.aggregation_weight, "aggregation_weight")
            .has_value());
}

TEST(ValidateUtilities, ValidateParameterExistsDoesExist)
{
    namespace pfv = Plato::Functional::Validation;
    Plato::objective tObjectiveInput;
    tObjectiveInput.aggregation_weight = 23;
    EXPECT_FALSE(
        pfv::error_message_for_empty_parameter("Objective: ", tObjectiveInput.aggregation_weight, "aggregation_weight")
            .has_value());
}

TEST(ValidateUtilities, ValidateParameterWhenParameterDoesNotExistWithinBounds)
{
    namespace pfv = Plato::Functional::Validation;
    namespace pfc = Plato::Functional::Core;
    Plato::objective tObjectiveInput;
    EXPECT_TRUE(
        pfv::error_message_for_parameter_out_of_bounds("Objective: ", tObjectiveInput.aggregation_weight, "aggregation_weight", pfc::unbounded<double>())
            .has_value());
}

TEST(ValidateUtilities, ValidateParameterExistsWithinBounds)
{
    namespace pfv = Plato::Functional::Validation;
    namespace pfc = Plato::Functional::Core;
    Plato::objective tObjectiveInput;
    constexpr double tLowerBound = 0;

    tObjectiveInput.aggregation_weight = 23;
    EXPECT_FALSE(pfv::error_message_for_parameter_out_of_bounds("Objective: ", tObjectiveInput.aggregation_weight,
                                                                "aggregation_weight",
                                                                pfc::lower_bounded(pfc::Inclusive{tLowerBound}))
                     .has_value());
}

TEST(ValidateUtilities, ValidateParameterExistsOutOfBounds)
{
    namespace pfv = Plato::Functional::Validation;
    namespace pfc = Plato::Functional::Core;
    Plato::objective tObjectiveInput;
    constexpr double tLowerBound = 0;

    tObjectiveInput.aggregation_weight = -23;
    EXPECT_TRUE(pfv::error_message_for_parameter_out_of_bounds("Objective: ", tObjectiveInput.aggregation_weight,
                                                               "aggregation_weight",
                                                               pfc::lower_bounded(pfc::Inclusive{tLowerBound}))
                    .has_value());
}

TEST(ValidateUtilities, AllMessages)
{
    const auto tMessage1 = std::string{"one fish"};
    const auto tMessage2 = std::string{"two fish"};
    const auto tMessages = std::vector{tMessage1, tMessage2};
    const auto tAllMessages = Plato::Functional::Validation::all_messages(tMessages);
    const auto tExpected = tMessage1 + "\n" + tMessage2 + "\n";
    EXPECT_EQ(tExpected, tAllMessages);
}
