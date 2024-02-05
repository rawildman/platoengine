#include <gtest/gtest.h>

#include <string_view>

#include "MeshProxy.hpp"
#include "NodalSumObjective.hpp"
#include "STKUtilities.hpp"

namespace
{
constexpr std::string_view kBrickFile = "brick.exo";
constexpr std::string_view kOneBlockCommand = "generated:1x1x1|bbox:0,0,0,1,1,1";
}  // namespace

TEST(NodalSumObjective, Value)
{
    namespace pf = Plato::Functional;
    pf::write_mesh(kBrickFile, pf::create_mesh(kOneBlockCommand));
    Plato::Functional::MeshProxy tMeshProxy{kBrickFile, {}};

    Plato::Functional::NodalSumObjective tPass;
    EXPECT_EQ(tPass.f(tMeshProxy), 12);
    EXPECT_TRUE(std::filesystem::exists(kBrickFile));
    EXPECT_TRUE(std::filesystem::remove(kBrickFile));
}

TEST(NodalSumObjective, Gradient)
{
    namespace pf = Plato::Functional;
    pf::write_mesh(kBrickFile, pf::create_mesh(kOneBlockCommand));
    Plato::Functional::MeshProxy tMeshProxy{kBrickFile, {}};

    Plato::Functional::NodalSumObjective tPass;
    std::vector<double> tGold(24, 1);
    EXPECT_EQ(tPass.df(tMeshProxy).stdVector(), tGold);
    EXPECT_TRUE(std::filesystem::exists(kBrickFile));
    EXPECT_TRUE(std::filesystem::remove(kBrickFile));
}
