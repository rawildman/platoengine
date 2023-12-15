#include <gtest/gtest.h>

#include "BrickShapeGeometry.hpp"
#include "Exception.hpp"
#include "InputGeneration.hpp"
#include "InputParser.hpp"

TEST(BrickShapeGeometryValidation, ValidBrickShapeMeshName)
{
    namespace pf = Plato::Functional;
    const auto tBrickShapeGeometry = pf::TestUtilities::create_valid_brick_shape_geometry();
    EXPECT_FALSE(pf::Validation::BrickShapeGeometry::detail::validate_mesh_name(tBrickShapeGeometry).has_value());
}

TEST(BrickShapeGeometryValidation, InvalidBrickShape)
{
    namespace pf = Plato::Functional;
    const auto tBrickShapeGeometry = Plato::brick_shape_geometry{};
    EXPECT_TRUE(pf::Validation::BrickShapeGeometry::detail::validate_mesh_name(tBrickShapeGeometry).has_value());
}
