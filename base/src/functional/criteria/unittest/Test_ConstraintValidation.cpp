#include <gtest/gtest.h>

#include "ConstraintValidation.hpp"
#include "CriterionValidation.hpp"
#include "InputGeneration.hpp"
#include "Plato_InputBlocks.hpp"

TEST(ConstraintValidation, ValidateEqualTo)
{
    namespace pfcd = Plato::Functional::Criteria::detail;
    Plato::constraint tConstraint;
    EXPECT_TRUE(pfcd::validate_equal_to(tConstraint).has_value());
    tConstraint.equal_to = 1.0;  // has only 1 : valid
    EXPECT_FALSE(pfcd::validate_equal_to(tConstraint).has_value());
}

TEST(ConstraintValidation, ErrorMessagesInvalidConstraint)
{
    namespace pfv = Plato::Functional::Validation;
    Plato::constraint tConstraint = Plato::Functional::TestUtilities::create_valid_example_constraint();
    tConstraint.app = boost::none;
    std::vector<std::string> tMessages;
    tMessages = pfv::validate(tConstraint, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 1u);
}

TEST(ConstraintValidation, ErrorMessagesTwoInvalidInput)
{
    namespace pfc = Plato::Functional::Criteria;
    Plato::constraint tConstraint;
    auto tConstraintTwo = tConstraint;
    tConstraint.name = "bad-one";
    tConstraintTwo.name = "bad-two";
    const std::vector<Plato::constraint> tInput{tConstraint, tConstraintTwo};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_constraints(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 4u);
}

TEST(ConstraintValidation, NoErrorMessagesTwoValidConstraints)
{
    namespace pfc = Plato::Functional::Criteria;
    const auto tConstraint = Plato::Functional::TestUtilities::create_valid_example_constraint();
    const std::vector<Plato::constraint> tInput{tConstraint, tConstraint};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_constraints(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 0u);
}

TEST(ConstraintValidation, ErrorMessagesTwoInvalidConstraints)
{
    namespace pfc = Plato::Functional::Criteria;
    auto tConstraint = Plato::Functional::TestUtilities::create_valid_example_constraint();
    tConstraint.equal_to = boost::none;
    auto tConstraintTwo = tConstraint;
    tConstraint.name = "bad-one";
    tConstraintTwo.name = "bad-two";
    const std::vector<Plato::constraint> tInput{tConstraint, tConstraintTwo};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_constraints(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 2u);
}
