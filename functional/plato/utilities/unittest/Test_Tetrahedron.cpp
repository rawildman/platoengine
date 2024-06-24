#include <gtest/gtest.h>

#include <filesystem>
#include <iomanip>

#include "plato/utilities/CoordinateTestUtilities.hpp"
#include "plato/utilities/Tetrahedron.hpp"

namespace plato::utilities::unittest
{
namespace
{
Tetrahedron create_unit_tetrahedron()
{
    Coordinate tPointA{0, 0, 0};
    Coordinate tPointB{1, 0, 0};
    Coordinate tPointC{0, 1, 0};
    Coordinate tPointD{0, 0, 1};

    return Tetrahedron{tPointA, tPointB, tPointC, tPointD};
}

}  // namespace

TEST(Tetrahedron, Volume)
{
    const Tetrahedron tTet = create_unit_tetrahedron();
    const double tResult = tTet.volume();
    constexpr double tGold = 1.0 / 6.0;

    EXPECT_DOUBLE_EQ(tResult, tGold);
}

TEST(Tetrahedron, Centroid)
{
    const Tetrahedron tTet = create_unit_tetrahedron();
    const Coordinate tResult = tTet.centroid();
    const Coordinate tGold{1.0 / 4.0, 1.0 / 4.0, 1.0 / 4.0};

    test_double_equality_of_components(tResult, tGold, TEST_CONTEXT("Tetrahedron Centroid"));
}

}  // namespace plato::utilities::unittest
