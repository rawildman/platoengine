#include <gtest/gtest.h>

#include <ROL_StdVector.hpp>
#include <memory>
#include <vector>

#include "ROLConstraintFunction.hpp"
#include "ROLTestUtilities.hpp"

TEST(ROLConstraintFunction, ConstraintValue)
{
    namespace pft = Plato::Functional::Test;
    namespace pf = Plato::Functional;
    pft::Rosenbrock tRosenbrock{};

    constexpr double tTarget = 5;

    const double tControlValue = 0.5;
    const std::vector<double> tGold = {tRosenbrock.f(tControlValue, tControlValue) - tTarget};

    constexpr bool tIsLinear = false;
    auto tROLConstraintFunction =
        pf::ROLConstraintFunction{pf::ConstraintFactory::Constraint<const ROL::StdVector<double>&>{
            "name", pft::make_rosenbrock_rol_vector_function(pft::Rosenbrock{}), tTarget, tIsLinear}};
    const auto tControls = ROL::StdVector<double>{tControlValue, tControlValue};
    auto tConstraintsVector = ROL::StdVector<double>{.0};
    double tTolerance;

    tROLConstraintFunction.value(tConstraintsVector, tControls, tTolerance);
    EXPECT_EQ(tGold, *tConstraintsVector.getVector());
    EXPECT_FALSE(tROLConstraintFunction.linear());
}

TEST(ROLConstraintFunction, JacobianTimesDirection)
{
    namespace pft = Plato::Functional::Test;
    namespace pf = Plato::Functional;
    pft::Rosenbrock tRosenbrock{};

    constexpr double tTarget = 5;

    const double tControlValue = 0.5;
    const auto tGoldVec = tRosenbrock.df(tControlValue, tControlValue);
    auto tDirection = ROL::StdVector<double>{1.0, -1.0};
    const double tGoldValue = (tGoldVec.mData[0] - tGoldVec.mData[1]);

    constexpr bool tIsLinear = true;
    auto tROLConstraintFunction =
        pf::ROLConstraintFunction{pf::ConstraintFactory::Constraint<const ROL::StdVector<double>&>{
            "name", pft::make_rosenbrock_rol_vector_function(pft::Rosenbrock{}), tTarget, tIsLinear}};

    const auto tControls = ROL::StdVector<double>{tControlValue, tControlValue};
    ROL::StdVector<double> tJacobianTimesDirection{1.0};

    double tTolerance;

    tROLConstraintFunction.applyJacobian(tJacobianTimesDirection, tDirection, tControls, tTolerance);
    ASSERT_EQ(tJacobianTimesDirection.getVector()->size(), 1);

    const double tResult = tJacobianTimesDirection.getVector()->front();
    EXPECT_EQ(tGoldValue, tResult);
    EXPECT_TRUE(tROLConstraintFunction.linear());
}

TEST(ROLConstraintFunction, AdjointJacobianTimesDirection)
{
    namespace pft = Plato::Functional::Test;
    namespace pf = Plato::Functional;

    pft::Rosenbrock tRosenbrock{};

    constexpr double tTarget = 5;
    constexpr std::string_view tConstraintName = "con name";

    const double tControlValue = 0.5;
    auto tGold = tRosenbrock.df(tControlValue, tControlValue);
    const double tDual = 1.2;
    const std::vector<double> tGoldVec{tGold.mData[0] * tDual, tGold.mData[1] * tDual};

    auto tDirection = ROL::StdVector<double>{tDual};
    constexpr bool tIsLinear = true;
    auto tROLConstraintFunction =
        pf::ROLConstraintFunction{pf::ConstraintFactory::Constraint<const ROL::StdVector<double>&>{
            std::string{tConstraintName}, pft::make_rosenbrock_rol_vector_function(pft::Rosenbrock{}), tTarget,
            tIsLinear}};

    const auto tControls = ROL::StdVector<double>{tControlValue, tControlValue};
    ROL::StdVector<double> tAdjointJacobianTimesDirection{1.0, 1.0};

    double tTolerance;

    tROLConstraintFunction.applyAdjointJacobian(tAdjointJacobianTimesDirection, tDirection, tControls, tTolerance);
    ASSERT_EQ(tAdjointJacobianTimesDirection.getVector()->size(), 2);

    const auto tResult = *tAdjointJacobianTimesDirection.getVector();
    EXPECT_EQ(tGoldVec, tResult);
    EXPECT_TRUE(tROLConstraintFunction.linear());
    EXPECT_EQ(tROLConstraintFunction.name(), std::string{tConstraintName});
}