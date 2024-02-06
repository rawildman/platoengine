#include <gtest/gtest.h>

#include "criteria/library/ConstraintValidation.hpp"
#include "criteria/library/CriterionValidation.hpp"
#include "input_parser/InputBlocks.hpp"
#include "test_utilities/InputGeneration.hpp"

namespace plato::functional::criteria::library::unittest
{
TEST(ConstraintValidation, ValidateEqualTo)
{
    namespace pfcd = plato::functional::criteria::library::detail;
    input_parser::constraint tConstraint;
    EXPECT_TRUE(pfcd::validate_equal_to(tConstraint).has_value());
    tConstraint.equal_to = 1.0;  // has only 1 : valid
    EXPECT_FALSE(pfcd::validate_equal_to(tConstraint).has_value());
}

TEST(ConstraintValidation, ErrorMessagesInvalidConstraint)
{
    input_parser::constraint tConstraint = plato::functional::test_utilities::create_valid_example_constraint();
    tConstraint.app = boost::none;
    std::vector<std::string> tMessages;
    tMessages = core::validate(tConstraint, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 1u);
}

TEST(ConstraintValidation, ErrorMessagesTwoInvalidInput)
{
    namespace pfc = plato::functional::criteria::library;
    input_parser::constraint tConstraint;
    auto tConstraintTwo = tConstraint;
    tConstraint.name = "bad-one";
    tConstraintTwo.name = "bad-two";
    const std::vector<input_parser::constraint> tInput{tConstraint, tConstraintTwo};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_constraints(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 4u);
}

TEST(ConstraintValidation, NoErrorMessagesTwoValidConstraints)
{
    namespace pfc = plato::functional::criteria::library;
    const auto tConstraint = plato::functional::test_utilities::create_valid_example_constraint();
    const std::vector<input_parser::constraint> tInput{tConstraint, tConstraint};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_constraints(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 0u);
}

TEST(ConstraintValidation, ErrorMessagesTwoInvalidConstraints)
{
    namespace pfc = plato::functional::criteria::library;
    auto tConstraint = plato::functional::test_utilities::create_valid_example_constraint();
    tConstraint.equal_to = boost::none;
    auto tConstraintTwo = tConstraint;
    tConstraint.name = "bad-one";
    tConstraintTwo.name = "bad-two";
    const std::vector<input_parser::constraint> tInput{tConstraint, tConstraintTwo};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_constraints(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 2u);
}
}  // namespace plato::functional::criteria::library::unittest
