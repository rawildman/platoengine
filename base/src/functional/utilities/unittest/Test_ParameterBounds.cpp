#include <gtest/gtest.h>

#include "ParameterBounds.hpp"

TEST(ParameterBounds, DirectConstructionAllInclusive)
{
    namespace pfc = Plato::Functional::Core;

    const auto tBounds = pfc::ParameterBounds{pfc::Inclusive{0.0}, pfc::Inclusive{1.0}};
    EXPECT_FALSE(tBounds.contains(-0.5));
    EXPECT_TRUE(tBounds.contains(0.0));
    EXPECT_TRUE(tBounds.contains(0.5));
    EXPECT_TRUE(tBounds.contains(1.0));
    EXPECT_FALSE(tBounds.contains(1.5));
}

TEST(ParameterBounds, DirectConstructionAllExclusive)
{
    namespace pfc = Plato::Functional::Core;

    const auto tBounds = pfc::ParameterBounds{pfc::Exclusive{0.0}, pfc::Exclusive{1.0}};
    EXPECT_FALSE(tBounds.contains(-0.5));
    EXPECT_FALSE(tBounds.contains(0.0));
    EXPECT_TRUE(tBounds.contains(0.5));
    EXPECT_FALSE(tBounds.contains(1.0));
    EXPECT_FALSE(tBounds.contains(1.5));
}

TEST(ParameterBounds, DirectConstructionInclusiveLowerExclusiveUpper)
{
    namespace pfc = Plato::Functional::Core;

    const auto tBounds = pfc::ParameterBounds{pfc::Inclusive{0.25}, pfc::Exclusive{0.5}};
    EXPECT_FALSE(tBounds.contains(-0.5));
    EXPECT_TRUE(tBounds.contains(0.25));
    EXPECT_TRUE(tBounds.contains(0.3));
    EXPECT_FALSE(tBounds.contains(0.5));
    EXPECT_FALSE(tBounds.contains(2.5));
}

TEST(ParameterBounds, DirectConstructionExclusiveLowerInclusiveUpper)
{
    namespace pfc = Plato::Functional::Core;

    const auto tBounds = pfc::ParameterBounds{pfc::Exclusive{-10.0}, pfc::Inclusive{1.0}};
    EXPECT_FALSE(tBounds.contains(-11.0));
    EXPECT_FALSE(tBounds.contains(-10.0));
    EXPECT_TRUE(tBounds.contains(0.5));
    EXPECT_TRUE(tBounds.contains(1.0));
    EXPECT_FALSE(tBounds.contains(1.5));
}

TEST(ParameterBounds, Unbounded)
{
    namespace pfc = Plato::Functional::Core;

    const auto tBounds = pfc::unbounded<double>();
    EXPECT_TRUE(tBounds.contains(-11.0));
    EXPECT_TRUE(tBounds.contains(-10.0));
    EXPECT_TRUE(tBounds.contains(0.5));
    EXPECT_TRUE(tBounds.contains(1.0));
    EXPECT_TRUE(tBounds.contains(1.5));
}

TEST(ParameterBounds, LowerBounded)
{
    namespace pfc = Plato::Functional::Core;

    const auto tBounds = pfc::lower_bounded(pfc::Inclusive{0.0});
    EXPECT_FALSE(tBounds.contains(-11.0));
    EXPECT_TRUE(tBounds.contains(0.0));
    EXPECT_TRUE(tBounds.contains(1.5));
}

TEST(ParameterBounds, UpperBounded)
{
    namespace pfc = Plato::Functional::Core;

    const auto tBounds = pfc::upper_bounded(pfc::Exclusive{0.0});
    EXPECT_TRUE(tBounds.contains(-11.0));
    EXPECT_FALSE(tBounds.contains(0.0));
    EXPECT_FALSE(tBounds.contains(1.5));
}

TEST(ParameterBounds, DirectConstructionExclusiveLowerInclusiveUpperInt)
{
    namespace pfc = Plato::Functional::Core;

    const auto tBounds = pfc::ParameterBounds{pfc::Exclusive{0}, pfc::Inclusive{10}};
    EXPECT_FALSE(tBounds.contains(-11.0));
    EXPECT_FALSE(tBounds.contains(0));
    EXPECT_TRUE(tBounds.contains(5));
    EXPECT_TRUE(tBounds.contains(10));
    EXPECT_FALSE(tBounds.contains(15));
}

TEST(ParameterBounds, DirectConstructionExclusiveLowerInclusiveUpperUnsignedInt)
{
    namespace pfc = Plato::Functional::Core;

    const auto tBounds = pfc::ParameterBounds{pfc::Exclusive{0u}, pfc::Inclusive{10u}};
    EXPECT_FALSE(tBounds.contains(0));
    EXPECT_TRUE(tBounds.contains(5));
    EXPECT_TRUE(tBounds.contains(10));
    EXPECT_FALSE(tBounds.contains(15));
}

TEST(ParameterBounds, Description)
{
    namespace pfc = Plato::Functional::Core;

    EXPECT_EQ(pfc::unbounded<double>().description(), "(-inf, inf)");
    EXPECT_EQ((pfc::ParameterBounds{pfc::Inclusive{0}, pfc::Inclusive{10}}.description()), "[0, 10]");
    EXPECT_EQ((pfc::ParameterBounds{pfc::Exclusive{-42}, pfc::Exclusive{0}}.description()), "(-42, 0)");
}
