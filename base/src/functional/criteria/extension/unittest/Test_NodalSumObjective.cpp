#include <gtest/gtest.h>

#include "NodalSumObjective.hpp"
#include "STKUtilities.hpp"

TEST(NodalSumObjective, Value111)
{
    namespace pf = Plato::Functional;
    constexpr std::string_view tFileName = "test.exo";
    pf::write_mesh(tFileName, pf::create_mesh("generated:1x1x1|bbox:0,0,0,1,1,1"));

    constexpr double tExpectedValue = 12.0;
    EXPECT_EQ(pf::NodalSumObjective{}.f(pf::MeshProxy{tFileName, {}}), tExpectedValue);
    EXPECT_TRUE(std::filesystem::remove(tFileName));
}

TEST(NodalSumObjective, Value211)
{
    namespace pf = Plato::Functional;
    constexpr std::string_view tFileName = "test.exo";
    pf::write_mesh(tFileName, pf::create_mesh("generated:2x1x1|bbox:0,0,0,1,1,1"));

    constexpr double tExpectedValue = 18.0;
    EXPECT_EQ(pf::NodalSumObjective{}.f(pf::MeshProxy{tFileName, {}}), tExpectedValue);
    EXPECT_TRUE(std::filesystem::remove(tFileName));
}

TEST(NodalSumObjective, Value0)
{
    namespace pf = Plato::Functional;
    constexpr std::string_view tFileName = "test.exo";
    pf::write_mesh(tFileName, pf::create_mesh("generated:1x1x1|bbox:-2,-1,-3,2,1,3"));

    constexpr double tExpectedValue = 0.0;
    EXPECT_EQ(pf::NodalSumObjective{}.f(pf::MeshProxy{tFileName, {}}), tExpectedValue);
    EXPECT_TRUE(std::filesystem::remove(tFileName));
}

TEST(NodalSumObjective, Gradient111)
{
    namespace pf = Plato::Functional;
    constexpr std::string_view tFileName = "test.exo";
    auto tBulk = pf::create_mesh("generated:1x2x3|bbox:0,0,0,1,1,1");
    pf::write_mesh(tFileName, tBulk);

    const auto tNodalSum = pf::NodalSumObjective{};

    constexpr int tNumCoordsPerNode = 3;
    constexpr int tNumNodes = 24;
    const auto tExpected = std::vector<double>(tNumCoordsPerNode * tNumNodes, 1.0);
    const std::vector tComputed = tNodalSum.df(pf::MeshProxy{tFileName, {}}).stdVector();
    EXPECT_EQ(tComputed, tExpected);
    EXPECT_TRUE(std::filesystem::remove(tFileName));
}
