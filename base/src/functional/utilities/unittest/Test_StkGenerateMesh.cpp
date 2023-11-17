#include <gtest/gtest.h>

#include <cmath>
#include <filesystem>
#include <numeric>
#include <stk_mesh/base/BulkData.hpp>
#include <stk_mesh/base/Comm.hpp>
#include <stk_util/parallel/Parallel.hpp>
#include <string_view>

#include "STKUtilities.hpp"

TEST(STKGenerateMesh, Box)
{
    namespace pf = Plato::Functional;
    ASSERT_EQ(stk::parallel_machine_size(MPI_COMM_WORLD), 1);

    constexpr std::string_view fileName = "test.exo";
    auto bulk = pf::create_mesh("generated:3x3x4|bbox:-1,-2,-1,2,1,2");
    pf::write_mesh(fileName, bulk);

    constexpr unsigned expectedNumNodes = 80;
    EXPECT_EQ(expectedNumNodes, pf::read_mesh_node_size(fileName));

    constexpr unsigned expectedNumElements = 3 * 3 * 4;
    EXPECT_EQ(expectedNumElements, pf::element_size(fileName));

    EXPECT_TRUE(std::filesystem::exists(fileName));
    EXPECT_TRUE(std::filesystem::remove(fileName));
}

TEST(STKUtilities, NumberOfNodesAndElementsFromBulk)
{
    namespace pf = Plato::Functional;
    EXPECT_EQ(pf::node_size(pf::create_mesh("generated:2x2x2|bbox:0,0,0,1,1,1")), 27u);
    EXPECT_EQ(pf::element_size(pf::create_mesh("generated:2x2x2|bbox:0,0,0,1,1,1")), 8u);
}

TEST(STKUtilities, SpatialDimensions)
{
    namespace pf = Plato::Functional;
    EXPECT_EQ(pf::spatial_dimensions(pf::create_mesh("generated:2x2x2|bbox:0,0,0,1,1,1")), 3u);
}

TEST(STKUtilities, ReadCoordinates)
{
    namespace pf = Plato::Functional;
    const std::vector<double> gold = {0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1};
    const auto res = pf::nodal_coordinates(pf::create_mesh("generated:1x1x1|bbox:0,0,0,1,1,1"));

    EXPECT_EQ(gold, res);
    EXPECT_EQ(pf::node_size(pf::create_mesh("generated:1x1x1|bbox:0,0,0,1,1,1")), 8u);
}

TEST(STKUtilities, WriteDensityField)
{
    namespace pf = Plato::Functional;
    constexpr std::string_view tInputFileName = "brick.exo";
    pf::write_mesh(tInputFileName, pf::create_mesh("generated:1x1x1"));
    const std::vector<double> data = {1, 2, 3, 4, 5, 6, 7, 8};
    constexpr std::string_view tOutputFileName = "brick-out.exo";
    pf::write_mesh_density(tInputFileName, data, tOutputFileName);
    auto res = pf::read_mesh_density(tOutputFileName);

    EXPECT_EQ(data, res);

    EXPECT_TRUE(std::filesystem::remove(tInputFileName));
    EXPECT_TRUE(std::filesystem::remove(tOutputFileName));
}
