#include <gtest/gtest.h>

#include "Compose.hpp"
#include "Penalty.hpp"
#include "Rosenbrock.hpp"
#include "Utilities.hpp"

TEST(Rosenbrock, Rosenbrock)
{
    const auto tRosenbrock = Plato::Functional::Test::Rosenbrock{1.0, 100.0};
    EXPECT_EQ(tRosenbrock.f(1.0, 1.0), 0.0);
    EXPECT_EQ(tRosenbrock.df(1.0, 1.0)(0), 0.0);
    EXPECT_EQ(tRosenbrock.df(1.0, 1.0)(1), 0.0);
}

TEST(Rosenbrock, MakeRosenbrock)
{
    namespace pft = Plato::Functional::Test;

    constexpr double tA = 2.0;
    constexpr double tB = 50.0;
    const auto tRosenbrock = pft::Rosenbrock{tA, tB};
    const auto tRosenbrockFunction = pft::make_rosenbrock_function(tRosenbrock);

    {
        const auto tX = pft::TwoDVector{0.5, 10.0};
        EXPECT_EQ(tRosenbrock.f(tX(0), tX(1)), tRosenbrockFunction.f(tX));
        EXPECT_EQ(tRosenbrock.df(tX(0), tX(1)), tRosenbrockFunction.df(tX));
    }
    {
        const auto tX = pft::TwoDVector{-0.5, 5.0};
        EXPECT_EQ(tRosenbrock.f(tX(0), tX(1)), tRosenbrockFunction.f(tX));
        EXPECT_EQ(tRosenbrock.df(tX(0), tX(1)), tRosenbrockFunction.df(tX));
    }
}

TEST(Rosenbrock, RosenbrockAndPenalty)
{
    namespace pft = Plato::Functional::Test;

    const auto tRosenbrock = pft::Rosenbrock{1.0, 100.0};
    const auto tPenalty = pft::Penalty{0.5 - 3, 3.0};

    const auto tH =
        Plato::Functional::compose(pft::make_rosenbrock_function(tRosenbrock), pft::make_penalty_function(tPenalty));

    const auto tX = pft::TwoDVector{1.0, 1.0};
    EXPECT_EQ(tH.f(tX), 0.0);
    EXPECT_EQ(tH.df(tX)(0), 0.0);
    EXPECT_EQ(tH.df(tX)(1), 0.0);
}
