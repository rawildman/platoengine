#include <gtest/gtest.h>

#include "ConstraintFactory.hpp"
#include "ConstraintValidation.hpp"
#include "Exception.hpp"
#include "InputGeneration.hpp"
#include "InputParser.hpp"
#include "ValidatedInput.hpp"

TEST(ConstraintFactory, MultipleValidConstraints)
{
    namespace pf = Plato::Functional;
    Plato::PlatoInput tInput = pf::TestUtilities::create_valid_example_input();
    Plato::constraint tConstraint = pf::TestUtilities::create_valid_example_constraint();
    tConstraint.name = "eq";
    tConstraint.app = Plato::CodeOptions::kNodalSum;
    tConstraint.equal_to = 13;
    tInput.mConstraints = {tConstraint};

    tConstraint.name = "le";
    tConstraint.equal_to = 17;
    tConstraint.is_linear = false;
    tInput.mConstraints.push_back(tConstraint);

    tConstraint.name = "ge";
    tConstraint.equal_to = 10;
    tConstraint.is_linear = false;
    tInput.mConstraints.push_back(tConstraint);

    pf::Validation::ValidatedInput tData = pf::Validation::make_validated_input(tInput);
    auto tCons = Plato::Functional::ConstraintFactory::make_constraints(tData.constraints());
    ASSERT_EQ(tCons.size(), 3);

    EXPECT_TRUE(tCons[0].mLinear);
    EXPECT_EQ(tCons[0].mConstraintTarget, 13);
    EXPECT_FALSE(tCons[1].mLinear);
    EXPECT_EQ(tCons[1].mConstraintTarget, 17);
    EXPECT_FALSE(tCons[2].mLinear);
    EXPECT_EQ(tCons[2].mConstraintTarget, 10);
}