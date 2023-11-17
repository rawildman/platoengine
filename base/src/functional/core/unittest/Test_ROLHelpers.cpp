#include <gtest/gtest.h>

#include "ROLHelpers.hpp"

TEST(ROLHelpers, ScalarMultiplication)
{
    const auto tV = ROL::StdVector<double>{1.0, 2.0, 3.0, 4.0};
    const double tA = 2.0;
    using Plato::Functional::operator*;

    const ROL::StdVector tU = tA * tV;

    const auto tExpected = std::vector<double>{2.0, 4.0, 6.0, 8.0};
    EXPECT_EQ(*tU.getVector(), tExpected);
}

TEST(ROLHelpers, Summation)
{
    const auto tV1 = ROL::StdVector<double>{1.0, 2.0, 3.0, 4.0};
    const auto tV2 = ROL::StdVector<double>{-1.0, -2.0, -3.0, -4.0};
    using Plato::Functional::operator+;
    
    const ROL::StdVector tV3 = tV1 + tV2;

    const auto tExpected = std::vector<double>{0.0, 0.0, 0.0, 0.0};
    EXPECT_EQ(*tV3.getVector(), tExpected);
}
