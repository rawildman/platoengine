#include <gtest/gtest.h>

#include <ROL_StdVector.hpp>

#include "Compose.hpp"
#include "Penalty.hpp"
#include "ROLObjectiveFunction.hpp"
#include "ROLPenaltyFunction.hpp"
#include "ROLTestUtilities.hpp"

TEST(ROLPenaltyFunction, ValueAndJacobian)
{
    namespace pft = Plato::Functional::Test;

    constexpr double tXMin = 0.5e-2;
    constexpr double tPower = 2.0;
    const auto tPenalty = make_penalty_rol_vector_function(pft::Penalty{tXMin, tPower});

    // Function and derivative at 1 and 0
    const auto tControl = ROL::StdVector<double>{1.0, 0.0};
    const ROL::StdVector tFOfControl = tPenalty.f(tControl);
    EXPECT_EQ(tFOfControl[0], 1.0);
    EXPECT_EQ(tFOfControl[1], tXMin);

    const auto tdFOfControl = tPenalty.df(tControl);
    EXPECT_EQ(tdFOfControl.mJacobian(0, 0), tPower * (1.0 - tXMin));
    EXPECT_EQ(tdFOfControl.mJacobian(0, 1), 0.0);
    EXPECT_EQ(tdFOfControl.mJacobian(1, 0), 0.0);
    EXPECT_EQ(tdFOfControl.mJacobian(1, 1), 0.0);

    const ROL::StdVector<double> tColumn0 = tdFOfControl.column(0);
    EXPECT_EQ(tColumn0[0], tPower * (1.0 - tXMin));
    EXPECT_EQ(tColumn0[1], 0.0);

    const ROL::StdVector<double> tColumn1 = tdFOfControl.column(1);
    EXPECT_EQ(tColumn1[0], 0.0);
    EXPECT_EQ(tColumn1[1], 0.0);
}

TEST(ROLPenaltyFunction, Multiplication)
{
    namespace pft = Plato::Functional::Test;

    const auto tX = ROL::StdVector<double>{1.0, 2.0};
    const auto tA = pft::ROLVectorJacobian{pft::makeTwoDMatrix(1.0, 2.0, 3.0, 4.0)};
    const auto tb = tX * tA;
    EXPECT_EQ(tb[0], 7.0);
    EXPECT_EQ(tb[1], 10.0);
}

TEST(ROLPenaltyFunction, Composition)
{
    namespace pf = Plato::Functional;
    namespace pft = Plato::Functional::Test;
    const auto tPenalty = pft::make_penalty_rol_vector_function(pft::Penalty{0.0, 2.0});
    const auto tRosenbrock = pft::make_rosenbrock_rol_vector_function(pft::Rosenbrock{});
    const auto tComposition = pf::compose(tRosenbrock, tPenalty);

    const auto tControl = ROL::StdVector<double>{1.0, 1.0};
    const double tCompositionOfX = tComposition.f(tControl);
    EXPECT_EQ(tCompositionOfX, 0.0);

    const ROL::StdVector<double> tDCompositionOfX = tComposition.df(tControl);
    EXPECT_EQ(tDCompositionOfX[0], 0.0);
    EXPECT_EQ(tDCompositionOfX[1], 0.0);
}
