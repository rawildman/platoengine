#include <gtest/gtest.h>

#include "BrickShapeGeometry.hpp"
#include "MassObjective.hpp"
#include "STKUtilities.hpp"

TEST(MassObjective, Value)
{
    namespace pf = Plato::Functional;
    constexpr double tDensity = 2.0;
    constexpr double tTarget = 1.0;
    const auto tMassObjective = pf::MassObjective{tDensity, tTarget};

    constexpr std::string_view tMeshName = "massTest.exo";
    pf::write_mesh(tMeshName, pf::create_mesh("generated:1x1x1|bbox:-1,-1,-1,1,1,1"));

    constexpr double tExpectedMass = tDensity * 8.0;
    EXPECT_DOUBLE_EQ(tMassObjective.mass(tMeshName), tExpectedMass);
}

TEST(MassObjective, NumMeshNodes)
{
    namespace pf = Plato::Functional;
    const auto tMassObjective = pf::MassObjective{0.0, 0.0};

    constexpr std::string_view tMeshName = "massTest.exo";
    pf::write_mesh(tMeshName, pf::create_mesh("generated:1x2x1|bbox:-1,-1,-1,1,1,1"));

    constexpr unsigned int tExpectedNumNodes = 12;
    EXPECT_DOUBLE_EQ(tMassObjective.numMeshNodes(tMeshName), tExpectedNumNodes);
}
