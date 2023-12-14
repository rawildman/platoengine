#include <gtest/gtest.h>

#include "AffirmUtilities.hpp"
#include "ConstraintValidation.hpp"
#include "CriterionValidation.hpp"
#include "Plato_InputBlocks.hpp"

namespace
{
Plato::constraint create_valid_example_constraint()
{
    return Plato::constraint{/*.name=*/std::string{"bike-shed"},
                             /*.active=*/true,
                             /*.app=*/Plato::CodeOptions::kCustomApp,
                             /*.shared_library_path=*/Plato::FileName{"/sweet/potato/ravioli.so"},
                             /*.number_of_processors=*/42u,
                             /*.input_files=*/Plato::FileList{{"brown.txt", "butter.txt", "sauce.txt"}},
                             /*.equal_to=*/0.0,
                             /*.less_than=*/boost::none,
                             /*.greater_than=*/boost::none,
                             /*.is_linear=*/true};
}

}  // namespace

TEST(ConstraintValidation, ValidateApp)
{
    namespace pfcd = Plato::Functional::Criteria::detail;
    Plato::constraint tConstraint;
    EXPECT_TRUE(pfcd::validate_app(tConstraint).has_value());
    tConstraint.app = Plato::CodeOptions::kCustomApp;
    EXPECT_FALSE(pfcd::validate_app(tConstraint).has_value());
}

TEST(ConstraintValidation, ValidateCustomApp)
{
    namespace pfcd = Plato::Functional::Criteria::detail;
    Plato::constraint tConstraint;
    tConstraint.app = Plato::CodeOptions::kCustomApp;
    EXPECT_TRUE(pfcd::validate_custom_app(tConstraint).has_value());
    tConstraint.shared_library_path = Plato::FileName{"/sweet/potato/ravioli.so"};
    EXPECT_FALSE(pfcd::validate_custom_app(tConstraint).has_value());
}

TEST(ConstraintValidation, ValidateOnlyOneType)
{
    namespace pfcd = Plato::Functional::Criteria::detail;
    Plato::constraint tConstraint;
    tConstraint.equal_to = 1.0;  // has only 1 : valid
    EXPECT_FALSE(pfcd::validate_only_one_type(tConstraint).has_value());
    tConstraint.less_than = 1.0;  // now has 2 : invalid
    EXPECT_TRUE(pfcd::validate_only_one_type(tConstraint).has_value());
    tConstraint.equal_to = boost::none;  // has only 1 : valid
    tConstraint.greater_than = 2.0;      // now has 2 : invalid
    EXPECT_TRUE(pfcd::validate_only_one_type(tConstraint).has_value());
    tConstraint.less_than = boost::none;  // has only 1 : valid
    EXPECT_FALSE(pfcd::validate_only_one_type(tConstraint).has_value());
    tConstraint.greater_than = boost::none;  // has 0 : invalid
    EXPECT_TRUE(pfcd::validate_only_one_type(tConstraint).has_value());
}

TEST(ConstraintValidation, ErrorMessagesInvalidConstraint)
{
    namespace pfv = Plato::Functional::Validation;
    Plato::constraint tConstraint = create_valid_example_constraint();
    tConstraint.app = boost::none;
    std::vector<std::string> tMessages;
    tMessages = pfv::validate<Plato::constraint>(tConstraint, tMessages);
    EXPECT_TRUE(tMessages.size() > 0);
    Plato::Functional::Affirmations::print_messages(tMessages);
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
    tMessages = pfc::validate_constraints(tInput, tMessages);
    EXPECT_TRUE(tMessages.size() > 0);
    Plato::Functional::Affirmations::print_messages(tMessages);
}

TEST(ConstraintValidation, NoErrorMessagesValidConstraints)
{
    namespace pfc = Plato::Functional::Criteria;
    const auto tConstraint = create_valid_example_constraint();
    const std::vector<Plato::constraint> tInput{tConstraint, tConstraint};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_constraints(tInput, tMessages);
    EXPECT_EQ(tMessages.size(), 0u);
}

TEST(ConstraintValidation, ErrorMessagesInvalidConstraints)
{
    namespace pfc = Plato::Functional::Criteria;
    auto tConstraint = create_valid_example_constraint();
    tConstraint.less_than = 1.0;
    auto tConstraintTwo = tConstraint;
    tConstraint.name = "bad-one";
    tConstraintTwo.name = "bad-two";
    const std::vector<Plato::constraint> tInput{tConstraint, tConstraintTwo};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_constraints(tInput, tMessages);
    EXPECT_EQ(tMessages.size(), 2u);
    Plato::Functional::Affirmations::print_messages(tMessages);
}