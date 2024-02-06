#include <gtest/gtest.h>

#include "geometry/extension/BrickShapeGeometry.hpp"
#include "utilities/Exception.hpp"
#include "geometry/library/GeometryValidation.hpp"
#include "test_utilities/InputGeneration.hpp"
namespace plato::functional::geometry::extension::unittest
{
TEST(BrickShapeGeometryValidation, BrickShapeMeshName)
{
    auto tBrickShapeGeometry = plato::functional::test_utilities::create_valid_brick_shape_geometry();
    EXPECT_FALSE(library::detail::validate_mesh_name(tBrickShapeGeometry).has_value());
    EXPECT_TRUE(library::detail::validate_mesh_name(input_parser::brick_shape_geometry{}).has_value());
}
}  // namespace plato::functional::geometry::extension::unittest