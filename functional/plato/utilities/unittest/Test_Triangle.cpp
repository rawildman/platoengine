#include <gtest/gtest.h>

#include <cmath>
#include <filesystem>

#include "plato/utilities/CoordinateTestUtilities.hpp"
#include "plato/utilities/Triangle.hpp"

namespace plato::utilities::unittest
{
namespace
{
Triangle create_unit_triangle()
{
    const Coordinate tPointA{1, 0, 0};
    const Coordinate tPointB{0, 1, 0};
    const Coordinate tPointC{0, 0, 1};

    return Triangle{tPointA, tPointB, tPointC};
}
}  // namespace

TEST(Triangle, Volume)
{
    const Triangle tTri = create_unit_triangle();
    const double tResult = tTri.volume();

    const Vector3 tVectorA{tTri.p1 - tTri.p0};
    const Vector3 tVectorB{tTri.p2 - tTri.p0};
    const Vector3 tCrossBA = cross(tVectorB, tVectorA);
    const double tGold = std::sqrt(dot(tCrossBA, tCrossBA)) * 0.5;

    EXPECT_DOUBLE_EQ(tResult, tGold);
}

TEST(Triangle, Centroid)
{
    const Triangle tTri = create_unit_triangle();
    const Coordinate tResult = tTri.centroid();

    const Coordinate tGold{1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0};
    test_double_equality_of_components(tResult, tGold, TEST_CONTEXT("Triangle centroid"));
}

}  // namespace plato::utilities::unittest
