#include <gtest/gtest.h>

#include <string>

#include "plato/criteria/extension/VolumeCriterion.hpp"
#include "plato/utilities/STKCommandGenerator.hpp"
#include "plato/utilities/STKUtilities.hpp"

namespace plato::criteria::extension::unittest
{
namespace
{
constexpr std::string_view kMeshFile = "brick.exo";

void test_volume_criteria_scaled_and_unscaled(const double aGoldVolume)
{
    namespace pfu = plato::utilities;
    constexpr double tConstantControls = 0.75;

    const core::MeshProxy tMeshProxy{kMeshFile, std::vector<double>(pfu::element_size(kMeshFile), tConstantControls)};

    EXPECT_EQ(VolumeCriterion{}.f(tMeshProxy), aGoldVolume * tConstantControls);
    constexpr double tScaleFactor = 2;
    EXPECT_EQ(VolumeCriterion{tScaleFactor}.f(tMeshProxy), aGoldVolume * tScaleFactor * tConstantControls);

    EXPECT_TRUE(std::filesystem::remove(kMeshFile));
}

}  // namespace

TEST(VolumeCriterion, Volume)
{
    namespace pfu = plato::utilities;
    const pfu::STKCommandGenerator tSTKCommandGenerator{
        {4, 4, 4}, {0, 0, 0}, {2.0, 2.0, 3.0}, utilities::STKCommandElementType::Hex};
    pfu::write_mesh(kMeshFile, pfu::create_mesh(tSTKCommandGenerator.toString()));
    test_volume_criteria_scaled_and_unscaled(tSTKCommandGenerator.volume());
}

TEST(VolumeCriterion, VolumeTetMesh)
{
    namespace pfu = plato::utilities;
    const pfu::STKCommandGenerator tSTKCommandGenerator{
        {4, 4, 4}, {0, 0, 0}, {1.0, 2.0, 3.0}, utilities::STKCommandElementType::Tet};
    pfu::write_mesh(kMeshFile, pfu::create_mesh(tSTKCommandGenerator.toString()));
    test_volume_criteria_scaled_and_unscaled(tSTKCommandGenerator.volume());
}

TEST(VolumeCriterion, DerivativeOfScaledVolumeOnControls)
{
    namespace pfu = plato::utilities;
    const pfu::STKCommandGenerator tSTKCommandGenerator{
        {1, 1, 3}, {0, 0, 0}, {2.0, 1.0, 3.0}, utilities::STKCommandElementType::Hex};
    pfu::write_mesh(kMeshFile, pfu::create_mesh(tSTKCommandGenerator.toString()));

    const std::vector<double> tGold{2, 2, 2};

    const core::MeshProxy tMeshProxy{kMeshFile, std::vector<double>{0.5, 0.4, 0.3}};
    const auto tResult = VolumeCriterion{}.df(tMeshProxy);
    ASSERT_EQ(tResult.size(), pfu::element_size(kMeshFile));
    ASSERT_EQ(tResult.size(), tGold.size());

    EXPECT_DOUBLE_EQ(tResult[0], tGold[0]);
    EXPECT_DOUBLE_EQ(tResult[1], tGold[1]);
    EXPECT_DOUBLE_EQ(tResult[2], tGold[2]);
}

}  // namespace plato::criteria::extension::unittest
