#include <gtest/gtest.h>

#include "Function.hpp"
#include "TwoDTestTypes.hpp"

TEST(PlatoFunctional, Evaluate)
{
    const auto tF = Plato::Functional::Function<double, double, double>{[](const double) { return 42.0; },
                                                                        [](const double) { return 84.0; }};
    EXPECT_EQ(tF.f(0.0), 42.0);
}

TEST(PlatoFunctional, EvaluateGradient)
{
    const auto tF = Plato::Functional::Function<double, double, double>{[](const double) { return 42.0; },
                                                                        [](const double) { return 84.0; }};
    EXPECT_EQ(tF.f(0.0), 42.0);
    EXPECT_EQ(tF.df(0.0), 84.0);
}

TEST(PlatoFunctional, MakeFunction)
{
    const auto tF =
        Plato::Functional::make_function([](const double aX) { return aX; }, [](const double aX) { return aX * aX; });
    EXPECT_EQ(tF.f(0.0), 0.0);
    EXPECT_EQ(tF.f(42.0), 42.0);
    EXPECT_EQ(tF.df(2.0), 4.0);
}

TEST(PlatoFunctional, TwoD)
{
    namespace pf = Plato::Functional;
    namespace pft = Plato::Functional::Test;
    const auto tF = pf::make_function(pft::TwoDVectorFunction{}, pft::TwoDVectorFunctionJacobian{});

    {
        const auto tX = pft::TwoDVector{0.0, 0.0};
        const auto tExpectedF = pft::makeTwoDVector(0.0, 0.0);
        const auto tExpectedDF = pft::makeTwoDMatrix(0.0, 0.0, 1.0, 1.0);
        EXPECT_EQ(tF.f(tX), tExpectedF);
        EXPECT_EQ(tF.df(tX), tExpectedDF);
    }
    {
        const auto tX = pft::makeTwoDVector(2.0, 1.0);
        const auto tExpectedF = pft::makeTwoDVector(2.0, 3.0);
        const auto tExpectedDF = pft::makeTwoDMatrix(1.0, 2.0, 1.0, 1.0);
        EXPECT_EQ(tF.f(tX), tExpectedF);
        EXPECT_EQ(tF.df(tX), tExpectedDF);
    }
}
