#include <gtest/gtest.h>

#include "BrickShapeGeometry.hpp"
#include "Exception.hpp"
#include "GeometryValidation.hpp"
#include "InputGeneration.hpp"

TEST(BrickShapeGeometryValidation, BrickShapeMeshName)
{
    namespace pf = Plato::Functional;
    auto tBrickShapeGeometry = pf::TestUtilities::create_valid_brick_shape_geometry();
    EXPECT_FALSE(pf::Geometry::detail::validate_mesh_name(tBrickShapeGeometry).has_value());
    EXPECT_TRUE(pf::Geometry::detail::validate_mesh_name(Plato::brick_shape_geometry{}).has_value());
}
