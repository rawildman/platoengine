#include <gtest/gtest.h>

#include "ConstraintFactory.hpp"
#include "Exception.hpp"
#include "InputGeneration.hpp"
#include "InputParser.hpp"

TEST(ConstraintFactoryDetail, InvalidConstraintNoRequirements)
{
    Plato::constraint tConstraintInput;
    EXPECT_THROW(Plato::Functional::ConstraintFactory::detail::affirm_only_one_type(tConstraintInput),
                 Plato::Functional::Exception);
}

TEST(ConstraintFactoryDetail, InvalidConstraintMultipleRequirementsEL)
{
    Plato::constraint tConstraintInput;
    tConstraintInput.equal_to = 1;
    tConstraintInput.less_than = 2;
    EXPECT_THROW(Plato::Functional::ConstraintFactory::detail::affirm_only_one_type(tConstraintInput),
                 Plato::Functional::Exception);
}

TEST(ConstraintFactoryDetail, InvalidConstraintMultipleRequirementsEG)
{
    Plato::constraint tConstraintInput;
    tConstraintInput.equal_to = 1;
    tConstraintInput.greater_than = 2;
    EXPECT_THROW(Plato::Functional::ConstraintFactory::detail::affirm_only_one_type(tConstraintInput),
                 Plato::Functional::Exception);
}

TEST(ConstraintFactoryDetail, InvalidConstraintMultipleRequirementsLG)
{
    Plato::constraint tConstraintInput;
    tConstraintInput.less_than = 1;
    tConstraintInput.greater_than = 2;
    EXPECT_THROW(Plato::Functional::ConstraintFactory::detail::affirm_only_one_type(tConstraintInput),
                 Plato::Functional::Exception);
}

TEST(ConstraintFactoryDetail, ValidConstraintRequirements)
{
    Plato::constraint tConstraintInput;
    tConstraintInput.less_than = 1;
    EXPECT_NO_THROW(Plato::Functional::ConstraintFactory::detail::affirm_only_one_type(tConstraintInput));
}

TEST(ConstraintFactory, ValidConstraint)
{
    const std::string tInput = Plato::Functional::TestUtilities::create_valid_example_constraint_string();
    const Plato::PlatoInput tData = Plato::Functional::parse_input(tInput);

    ASSERT_EQ(tData.mConstraints.size(), 1);
    auto tConstraint = Plato::Functional::ConstraintFactory::detail::make_constraint(tData.mConstraints.front());
    EXPECT_TRUE(tConstraint.mLinear);
    EXPECT_EQ(tConstraint.mConstraintTarget, 13.0);
}

TEST(ConstraintFactory, MultipleValidConstraints)
{
    constexpr std::string_view tInput =
        R"(
          begin constraint eq
            app nodal_sum
            equal_to 13
            is_linear true
          end
          begin constraint le
            app nodal_sum
            less_than 17
          end
           begin constraint ge
            app nodal_sum
            greater_than 10
          end
       )";

    const Plato::PlatoInput tData = Plato::Functional::parse_input(tInput);
    EXPECT_EQ(tData.mConstraints.size(), 3);

    EXPECT_NO_THROW(Plato::Functional::ConstraintFactory::detail::affirm_valid_input(tData.mConstraints));

    auto tCons = Plato::Functional::ConstraintFactory::make_constraints(tData.mConstraints);
    EXPECT_EQ(tCons.size(), 3);

    EXPECT_TRUE(tCons[0].mLinear);
    EXPECT_EQ(tCons[0].mConstraintTarget, 13);
    EXPECT_FALSE(tCons[1].mLinear);
    EXPECT_EQ(tCons[1].mConstraintTarget, 17);
    EXPECT_FALSE(tCons[2].mLinear);
    EXPECT_EQ(tCons[2].mConstraintTarget, 10);
}

TEST(ConstraintFactory, DualVector)
{
    std::unique_ptr<ROL::StdVector<double>> tDualVector = Plato::Functional::ConstraintFactory::make_dual_vector();
    ASSERT_EQ(tDualVector->dimension(), 1);
    EXPECT_EQ(tDualVector->getVector()->front(), 1.0);
}
