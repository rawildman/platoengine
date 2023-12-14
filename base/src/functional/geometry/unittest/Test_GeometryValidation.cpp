#include <gtest/gtest.h>

#include "GeometryValidation.hpp"

TEST(GeometryValidation, InValidPlatoInputNoGeometry)
{
    const auto tInput = Plato::PlatoInput{};
    EXPECT_TRUE(Plato::Functional::Geometry::detail::validate_only_one_geometry(tInput).has_value());
}

TEST(GeometryValidation, ValidPlatoInputOneGeometry)
{
    auto tInput = Plato::PlatoInput{};
    tInput.mBrickShapeGeometry = Plato::brick_shape_geometry{};
    EXPECT_FALSE(Plato::Functional::Geometry::detail::validate_only_one_geometry(tInput).has_value());
}

TEST(GeometryValidation, InValidPlatoInputTwoGeometry)
{
    auto tInput = Plato::PlatoInput{};
    tInput.mBrickShapeGeometry = Plato::brick_shape_geometry{};
    tInput.mDensityTopology = Plato::density_topology{};
    EXPECT_TRUE(Plato::Functional::Geometry::detail::validate_only_one_geometry(tInput).has_value());
}