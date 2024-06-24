#include <gtest/gtest.h>

#include <cmath>
#include <filesystem>
#include <numeric>
#include <stk_mesh/base/BulkData.hpp>
#include <stk_mesh/base/Comm.hpp>
#include <stk_util/parallel/Parallel.hpp>
#include <string_view>

#include "plato/utilities/CoordinateTestUtilities.hpp"
#include "plato/utilities/STKCommandGenerator.hpp"
#include "plato/utilities/STKUtilities.hpp"
#include "plato/utilities/STKVolumeUtilities.hpp"

namespace plato::utilities::unittest
{
TEST(STKGenerateMesh, Box)
{
    ASSERT_EQ(stk::parallel_machine_size(MPI_COMM_WORLD), 1);

    constexpr std::string_view fileName = "test.exo";
    const STKCommandGenerator tSTKCommandGenerator{{3, 3, 4}, {-1, -2, -1}, {2, 1, 2}, STKCommandElementType::Hex};
    auto bulk = generate_stk_mesh(tSTKCommandGenerator);
    write_mesh(fileName, bulk);

    EXPECT_EQ(tSTKCommandGenerator.numberOfNodes(), read_mesh_node_size(fileName));
    EXPECT_EQ(tSTKCommandGenerator.numberOfElements(), element_size(fileName));

    EXPECT_TRUE(std::filesystem::exists(fileName));
    EXPECT_TRUE(std::filesystem::remove(fileName));
}

TEST(STKUtilities, NumberOfNodesAndElementsFromBulk)
{
    const STKCommandGenerator tSTKCommandGenerator{{2, 2, 2}};
    const auto tMesh = generate_stk_mesh(tSTKCommandGenerator);
    ASSERT_TRUE(tMesh);
    EXPECT_EQ(node_size(*tMesh), tSTKCommandGenerator.numberOfNodes());
    EXPECT_EQ(element_size(*tMesh), tSTKCommandGenerator.numberOfElements());
}

TEST(STKUtilities, SpatialDimensions)
{
    const STKCommandGenerator tSTKCommandGenerator{{2, 2, 2}};
    const auto tMesh = generate_stk_mesh(tSTKCommandGenerator);
    ASSERT_TRUE(tMesh);
    EXPECT_EQ(spatial_dimensions(*tMesh), 3u);
}

TEST(STKUtilities, ReadCoordinates)
{
    const STKCommandGenerator tSTKCommandGenerator;
    const std::vector<double> gold = {0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1};
    const auto tMesh = generate_stk_mesh(tSTKCommandGenerator);
    ASSERT_TRUE(tMesh);
    const std::vector<double> res = flattened_nodal_coordinates(*tMesh);
    EXPECT_EQ(gold, res);
    EXPECT_EQ(node_size(*tMesh), tSTKCommandGenerator.numberOfNodes());
}

TEST(STKUtilities, ReadCoordinatesCoordinate)
{
    const STKCommandGenerator tSTKCommandGenerator;
    const std::vector<Coordinate> tGold = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0},
                                           {0, 0, 1}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1}};
    const auto tMesh = generate_stk_mesh(tSTKCommandGenerator);
    ASSERT_TRUE(tMesh);
    const std::vector<Coordinate> tResult = nodal_coordinates(*tMesh);
    ASSERT_EQ(tGold.size(), tResult.size());
    for (unsigned int tIndex = 0; tIndex < tGold.size(); ++tIndex)
    {
        test_double_equality_of_components(tResult[tIndex], tGold[tIndex], TEST_CONTEXT("Read nodal coordinates"));
    }

    EXPECT_EQ(node_size(*tMesh), tSTKCommandGenerator.numberOfNodes());
}

TEST(STKUtilities, WriteDensityField)
{
    constexpr std::string_view tInputFileName = "brick.exo";
    write_mesh(tInputFileName, generate_stk_mesh(STKCommandGenerator{}));
    const std::vector<double> data = {1, 2, 3, 4, 5, 6, 7, 8};
    constexpr std::string_view tOutputFileName = "brick-out.exo";
    write_mesh_density(tInputFileName, data, tOutputFileName);
    auto res = read_mesh_density(tOutputFileName);

    EXPECT_EQ(data, res);

    EXPECT_TRUE(std::filesystem::remove(tInputFileName));
    EXPECT_TRUE(std::filesystem::remove(tOutputFileName));
}

TEST(STKUtilities, WriteElementDensityField)
{
    constexpr std::string_view tInputFileName = "brick.exo";
    const STKCommandGenerator tSTKCommandGenerator{{2, 2, 2}};
    write_mesh(tInputFileName, generate_stk_mesh(tSTKCommandGenerator));
    const std::vector<double> data = {1, 2, 3, 4, 5, 6, 7, 8};
    constexpr std::string_view tOutputFileName = "brick-out.exo";
    write_element_density(tInputFileName, data, tOutputFileName);
    auto res = read_element_density(tOutputFileName);

    EXPECT_EQ(data, res);

    EXPECT_TRUE(std::filesystem::remove(tInputFileName));
    EXPECT_TRUE(std::filesystem::remove(tOutputFileName));
}

}  // namespace plato::utilities::unittest
