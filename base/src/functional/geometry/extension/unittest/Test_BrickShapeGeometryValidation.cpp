#include <gtest/gtest.h>

#include "BrickShapeGeometry.hpp"
#include "Exception.hpp"
#include "GeometryValidation.hpp"
#include "InputGeneration.hpp"
namespace plato::functional::geometry::extension::unittest
{
TEST(BrickShapeGeometryValidation, BrickShapeMeshName)
{
    auto tBrickShapeGeometry = Plato::Functional::TestUtilities::create_valid_brick_shape_geometry();
    EXPECT_FALSE(library::detail::validate_mesh_name(tBrickShapeGeometry).has_value());
    EXPECT_TRUE(library::detail::validate_mesh_name(Plato::brick_shape_geometry{}).has_value());
}
}  // namespace plato::functional::geometry::extension::unittest