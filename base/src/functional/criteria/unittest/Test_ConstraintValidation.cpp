#include <gtest/gtest.h>

#include "ConstraintValidation.hpp"
#include "CriterionValidation.hpp"
#include "InputGeneration.hpp"
#include "Plato_InputBlocks.hpp"

TEST(ConstraintValidation, ValidateOnlyOneType)
{
    namespace pfcd = Plato::Functional::Criteria::detail;
    Plato::constraint tConstraint;
    tConstraint.equal_to = 1.0;  // has only 1 : valid
    EXPECT_FALSE(pfcd::validate_only_one_type(tConstraint).has_value());
    tConstraint.less_than = 1.0;  // now has 2 : invalid
    EXPECT_TRUE(pfcd::validate_only_one_type(tConstraint).has_value());
    tConstraint.equal_to = boost::none;  // has only 1 : valid
    EXPECT_FALSE(pfcd::validate_only_one_type(tConstraint).has_value());
    tConstraint.greater_than = 2.0;  // now has 2 : invalid
    EXPECT_TRUE(pfcd::validate_only_one_type(tConstraint).has_value());
    tConstraint.less_than = boost::none;  // has only 1 : valid
    EXPECT_FALSE(pfcd::validate_only_one_type(tConstraint).has_value());
    tConstraint.greater_than = boost::none;  // has 0 : invalid
    EXPECT_TRUE(pfcd::validate_only_one_type(tConstraint).has_value());
}

TEST(ConstraintValidation, ErrorMessagesInvalidConstraint)
{
    namespace pfv = Plato::Functional::Validation;
    Plato::constraint tConstraint = Plato::Functional::TestUtilities::create_valid_example_constraint();
    tConstraint.app = boost::none;
    std::vector<std::string> tMessages;
    tMessages = pfv::validate<Plato::constraint>(tConstraint, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 1u);
    Plato::Functional::Validation::print_messages(tMessages);
}

TEST(ConstraintValidation, ErrorMessagesInvalidInput)
{
    namespace pfc = Plato::Functional::Criteria;
    Plato::constraint tConstraint;
    auto tConstraintTwo = tConstraint;
    tConstraint.name = "bad-one";
    tConstraintTwo.name = "bad-two";
    const std::vector<Plato::constraint> tInput{tConstraint, tConstraintTwo};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_constraints(tInput, std::move(tMessages));
    EXPECT_TRUE(tMessages.size() > 0);
    Plato::Functional::Validation::print_messages(tMessages);
}

TEST(ConstraintValidation, NoErrorMessagesValidConstraints)
{
    namespace pfc = Plato::Functional::Criteria;
    const auto tConstraint = Plato::Functional::TestUtilities::create_valid_example_constraint();
    const std::vector<Plato::constraint> tInput{tConstraint, tConstraint};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_constraints(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 0u);
}

TEST(ConstraintValidation, ErrorMessagesInvalidConstraints)
{
    namespace pfc = Plato::Functional::Criteria;
    auto tConstraint = Plato::Functional::TestUtilities::create_valid_example_constraint();
    tConstraint.less_than = 1.0;
    auto tConstraintTwo = tConstraint;
    tConstraint.name = "bad-one";
    tConstraintTwo.name = "bad-two";
    const std::vector<Plato::constraint> tInput{tConstraint, tConstraintTwo};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_constraints(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 2u);
    Plato::Functional::Validation::print_messages(tMessages);
}