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

TEST(ValidateUtilities, IsWithinLowerBounds)
{
    namespace pfvd = Plato::Functional::Validation::detail;
    EXPECT_TRUE(pfvd::is_within_lower_bounds(10, std::nullopt));
    EXPECT_TRUE(pfvd::is_within_lower_bounds(10, 9));
    EXPECT_TRUE(pfvd::is_within_lower_bounds(10, 10));
    EXPECT_FALSE(pfvd::is_within_lower_bounds(10, 11));
}

TEST(ValidateUtilities, IsWithinUpperBounds)
{
    namespace pfvd = Plato::Functional::Validation::detail;
    EXPECT_TRUE(pfvd::is_within_upper_bounds(10, std::nullopt));
    EXPECT_FALSE(pfvd::is_within_upper_bounds(10, 9));
    EXPECT_TRUE(pfvd::is_within_upper_bounds(10, 10));
    EXPECT_TRUE(pfvd::is_within_upper_bounds(10, 11));
}

TEST(ValidateUtilities, IsWithinBounds)
{
    namespace pfvd = Plato::Functional::Validation::detail;
    constexpr double tLowerBound = 0;
    constexpr double tUpperBound = 1;

    EXPECT_FALSE(pfvd::is_within_bounds(-0.5, tLowerBound, tUpperBound));
    EXPECT_TRUE(pfvd::is_within_bounds(0, tLowerBound, tUpperBound));
    EXPECT_TRUE(pfvd::is_within_bounds(0.5, tLowerBound, tUpperBound));
    EXPECT_TRUE(pfvd::is_within_bounds(1, tLowerBound, tUpperBound));
    EXPECT_FALSE(pfvd::is_within_bounds(1.5, tLowerBound, tUpperBound));
}

TEST(ValidateUtilities, ValidateParameterWhenParameterDoesNotExistWithinBounds)
{
    namespace pfv = Plato::Functional::Validation;
    Plato::objective tObjectiveInput;
    constexpr std::optional<double> tLowerBound = std::nullopt;
    constexpr std::optional<double> tUpperBound = std::nullopt;

    EXPECT_TRUE(
        pfv::error_message_for_parameter_out_of_bounds("Objective: ", tObjectiveInput.aggregation_weight, "aggregation_weight", tLowerBound, tUpperBound)
            .has_value());
}

TEST(ValidateUtilities, ValidateParameterExistsWithinBounds)
{
    namespace pfv = Plato::Functional::Validation;
    Plato::objective tObjectiveInput;
    constexpr double tLowerBound = 0;
    constexpr std::optional<double> tUpperBound = std::nullopt;

    tObjectiveInput.aggregation_weight = 23;
    EXPECT_FALSE(
        pfv::error_message_for_parameter_out_of_bounds("Objective: ", tObjectiveInput.aggregation_weight, "aggregation_weight", tLowerBound, tUpperBound)
            .has_value());
}

TEST(ValidateUtilities, ValidateParameterExistsOutOfBounds)
{
    namespace pfv = Plato::Functional::Validation;
    Plato::objective tObjectiveInput;
    constexpr double tLowerBound = 0;
    constexpr std::optional<double> tUpperBound = std::nullopt;

    tObjectiveInput.aggregation_weight = -23;
    EXPECT_TRUE(
        pfv::error_message_for_parameter_out_of_bounds("Objective: ", tObjectiveInput.aggregation_weight, "aggregation_weight", tLowerBound, tUpperBound)
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
