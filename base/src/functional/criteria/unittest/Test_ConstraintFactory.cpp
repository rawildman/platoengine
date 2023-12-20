#include <gtest/gtest.h>

#include "ConstraintFactory.hpp"
#include "ConstraintValidation.hpp"
#include "Exception.hpp"
#include "InputGeneration.hpp"
#include "InputParser.hpp"

TEST(ConstraintFactory, ValidConstraint)
{
    const std::string tInput = Plato::Functional::TestUtilities::create_valid_example_constraint_string();
    const Plato::PlatoInput tData = Plato::Functional::parse_input(tInput);

    ASSERT_EQ(tData.mConstraints.size(), 1);
    auto tConstraint = Plato::Functional::ConstraintFactory::detail::make_constraint(tData.mConstraints.front());
    EXPECT_TRUE(tConstraint.mLinear);
    EXPECT_EQ(tConstraint.mConstraintTarget, 13.0);
}

TEST(ConstraintFactory, DualVector)
{
    std::unique_ptr<ROL::StdVector<double>> tDualVector = Plato::Functional::ConstraintFactory::make_dual_vector();
    ASSERT_EQ(tDualVector->dimension(), 1);
    EXPECT_EQ(tDualVector->getVector()->front(), 1.0);
}
