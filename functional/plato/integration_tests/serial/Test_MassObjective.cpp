#include <gtest/gtest.h>

#include <filesystem>

#include "plato/geometry/extension/BrickShapeGeometry.hpp"
#include "plato/integration_tests/test_mass_objective/MassObjective.hpp"
#include "plato/utilities/STKUtilities.hpp"

namespace plato::integration_tests::serial
{
TEST(MassObjective, Value)
{
    namespace pfu = plato::utilities;

    constexpr double tDensity = 2.0;
    const auto tMassObjective = test_mass_objective::MassObjective{tDensity};

    constexpr std::string_view tMeshName = "massTest.exo";
    pfu::write_mesh(tMeshName, pfu::create_mesh("generated:1x1x1|bbox:-1,-1,-1,1,1,1"));

    constexpr double tExpectedMass = tDensity * 8.0;
    EXPECT_DOUBLE_EQ(tMassObjective.mass(tMeshName), tExpectedMass);

    std::filesystem::remove(tMeshName);
}

TEST(MassObjective, NumMeshNodes)
{
    namespace pfu = plato::utilities;

    const auto tMassObjective = test_mass_objective::MassObjective{0.0};

    constexpr std::string_view tMeshName = "massTest.exo";
    pfu::write_mesh(tMeshName, pfu::create_mesh("generated:1x2x1|bbox:-1,-1,-1,1,1,1"));

    constexpr unsigned int tExpectedNumNodes = 12;
    EXPECT_DOUBLE_EQ(tMassObjective.numMeshNodes(tMeshName), tExpectedNumNodes);

    std::filesystem::remove(tMeshName);
}
}  // namespace plato::integration_tests::serial