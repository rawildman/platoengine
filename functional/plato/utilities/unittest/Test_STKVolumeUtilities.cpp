
#include <gtest/gtest.h>

#include <filesystem>
#include <iomanip>

#include "plato/test_utilities/TestDataFilePath.hpp"
#include "plato/utilities/CoordinateTestUtilities.hpp"
#include "plato/utilities/STKCommandGenerator.hpp"
#include "plato/utilities/STKVolumeUtilities.hpp"
#include "plato/utilities/Vector3.hpp"

namespace plato::utilities::unittest
{

namespace
{

void create_mesh_test_volume(const STKCommandGenerator& aSTKCommandGenerator)
{
    const double tResult = mesh_volume(*generate_stk_mesh(aSTKCommandGenerator));

    std::cout << std::setprecision(16) << tResult << std::endl;
    EXPECT_DOUBLE_EQ(tResult, aSTKCommandGenerator.volume());
}

void create_single_element_mesh_test_centroid(const STKCommandGenerator& aCommandGenerator, const Coordinate aGold)
{
    const auto tMesh = generate_stk_mesh(aCommandGenerator);
    ASSERT_TRUE(tMesh);

    const auto tElements = element_vector(*tMesh);
    ASSERT_EQ(tElements.size(), 1u);

    const Coordinate tElemCentroid = element_centroid(tElements[0], *tMesh);
    test_double_equality_of_components(tElemCentroid, aGold, TEST_CONTEXT("Single element centroid"));
}

void read_mesh_and_test_volume(const std::string_view tMeshFileName, const double aGold)
{
    const auto tFilePath = test_utilities::test_data_file_path(tMeshFileName);
    ASSERT_TRUE(tFilePath);
    const double tResult = mesh_volume(*read_mesh_bulk_data(std::string{tFilePath.value()}));
    std::cout << std::setprecision(16) << tResult << std::endl;
    EXPECT_DOUBLE_EQ(tResult, aGold);
}

void test_first_element_volume_and_coordinates(const STKCommandGenerator& aSTKCommandGenerator,
                                               const double aGoldVolume,
                                               const std::vector<Coordinate>& aGoldCoordinates)
{
    const auto tBulk = generate_stk_mesh(aSTKCommandGenerator);
    const stk::mesh::EntityVector tElements = element_vector(*tBulk);
    ASSERT_FALSE(tElements.empty());
    const auto tOnlyElement = tElements[0];
    const double tVolume = element_volume(tOnlyElement, *tBulk);
    EXPECT_DOUBLE_EQ(tVolume, aGoldVolume);
    std::vector<Coordinate> tCoordinates = element_coordinates(tOnlyElement, *tBulk);
    ASSERT_EQ(tCoordinates.size(), aGoldCoordinates.size());
    for (unsigned int iIndex = 0; iIndex < tCoordinates.size(); ++iIndex)
    {
        test_double_equality_of_components(tCoordinates[iIndex], aGoldCoordinates[iIndex],
                                           TEST_CONTEXT("Element coordinates"));
    }
}

}  // namespace

TEST(STKVolumeUtilities, HexVolumeAndCoordinates)
{
    test_first_element_volume_and_coordinates(
        STKCommandGenerator{}, 1.0,
        {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}});
}

TEST(STKVolumeUtilities, TetVolumeAndCoordinates)
{
    test_first_element_volume_and_coordinates(
        STKCommandGenerator{{1, 1, 1}, {0, 0, 0}, {1, 1, 1}, STKCommandElementType::Tet}, 1.0 / 6.0,
        {{0, 0, 0}, {1, 1, 0}, {0, 1, 0}, {1, 1, 1}});
}

TEST(STKVolumeUtilities, MeshVolume)
{
    {
        const STKCommandGenerator tSTKCommandGenerator{{2, 3, 4}};
        create_mesh_test_volume(tSTKCommandGenerator);
    }
    {
        const STKCommandGenerator tSTKCommandGenerator{{3, 4, 5}, {}, {3, 4, 5}, STKCommandElementType::Tet};
        create_mesh_test_volume(tSTKCommandGenerator);
    }
    {
        const STKCommandGenerator tSTKCommandGenerator{{1, 1, 1}, {}, {}, STKCommandElementType::Tet};
        create_mesh_test_volume(tSTKCommandGenerator);
    }

    {
        const STKCommandGenerator tSTKCommandGenerator{{20, 2, 3}, {}, {20, 2, 3}, STKCommandElementType::Tet};
        create_mesh_test_volume(tSTKCommandGenerator);
    }
}

TEST(STKVolumeUtilities, ExternalMeshVolume_Box_2x4x8_hex)
{
    constexpr double tGold = 2 * 4 * 8;
    read_mesh_and_test_volume("box_2x4x8_hex.cdf", tGold);
    read_mesh_and_test_volume("box_2x4x8_hex20.cdf", tGold);
}

TEST(STKVolumeUtilities, ExternalMeshVolume_Box_3x4x7_tet)
{
    constexpr double tGold = 3 * 4 * 7;
    read_mesh_and_test_volume("box_3x4x7_tet4.cdf", tGold);
    read_mesh_and_test_volume("box_3x4x7_tet10.cdf", tGold);
}

TEST(STKVolumeUtilities, ExternalMeshVolume_Box_2x4x10_hex_and_tet)
{
    read_mesh_and_test_volume("box_2x4x10_hex_and_tet.cdf", 2 * 4 * 10);
}

TEST(STKVolumeUtilities, ExternalMeshVolume_Rectangle_3x4_quad)
{
    read_mesh_and_test_volume("rectangle_3x4_quad4.cdf", 3 * 4);
}

TEST(STKVolumeUtilities, ExternalMeshVolume_Rectangle_3x4_shell)
{
    read_mesh_and_test_volume("rectangle_3x4_shell4.cdf", 3 * 4);
}

TEST(STKVolumeUtilities, ExternalMeshVolume_Rectangle_3x4_tri)
{
    read_mesh_and_test_volume("rectangle_3x4_tri3.cdf", 3 * 4);
}

TEST(STKVolumeUtilities, ExternalMeshVolume_Rectangle_4x10_tri_and_shell)
{
    read_mesh_and_test_volume("rectangle_4x10_tri_and_shell.cdf", 4 * 10);
}

TEST(STKVolumeUtilities, ElementCentroid_Hex)
{
    // unit hex
    create_single_element_mesh_test_centroid(STKCommandGenerator{{1, 1, 1}, {0, 0, 0}, {1, 1, 1}},
                                             Coordinate{0.5, 0.5, 0.5});

    // scaled hex
    constexpr Coordinate tLower{-1.0, -2.0, -4.0};
    constexpr Coordinate tUpper{6.0, 8.0, 2.0};
    constexpr Coordinate tCentroid = (tLower + tUpper) / 2.0;
    create_single_element_mesh_test_centroid(STKCommandGenerator{{1, 1, 1}, tLower, tUpper}, tCentroid);
}

TEST(STKVolumeUtilities, ElementCentroids)
{
    STKCommandGenerator tSTKCommandGenerator{{4, 1, 1}, {0, 0, 0}, {4, 1, 1}};

    const auto tBulk = generate_stk_mesh(tSTKCommandGenerator);
    const auto tCentroids = element_centroids(*tBulk);
    ASSERT_EQ(tCentroids.size(), 4u);

    test_double_equality_of_components(tCentroids[0], {0.5, 0.5, 0.5}, TEST_CONTEXT("Element centroids 0"));
    test_double_equality_of_components(tCentroids[1], {1.5, 0.5, 0.5}, TEST_CONTEXT("Element centroids 1"));
    test_double_equality_of_components(tCentroids[2], {2.5, 0.5, 0.5}, TEST_CONTEXT("Element centroids 2"));
    test_double_equality_of_components(tCentroids[3], {3.5, 0.5, 0.5}, TEST_CONTEXT("Element centroids 3"));
}

TEST(STKVolumeUtilities, AverageNodalDensity)
{
    const STKCommandGenerator tSTKCommandGenerator{{4, 4, 4}, {0, 0, 0}, {4, 4, 4}};

    const auto tBulk = generate_stk_mesh(tSTKCommandGenerator);
    const int tTotalNumberOfNodes = tSTKCommandGenerator.numberOfNodes();
    const double tTotalVolume = tSTKCommandGenerator.volume();
    const double tGold = static_cast<double>(tTotalNumberOfNodes) / tTotalVolume;
    const double tResult = average_nodal_density(*tBulk);

    EXPECT_DOUBLE_EQ(tGold, tResult);
}

}  // namespace plato::utilities::unittest
