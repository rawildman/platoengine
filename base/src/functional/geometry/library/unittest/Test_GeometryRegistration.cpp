#include <gtest/gtest.h>

#include "DynamicVector.hpp"
#include "Exception.hpp"
#include "Function.hpp"
#include "GeometryRegistration.hpp"
#include "InputBlocks.hpp"
#include "JacobianMultiplier.hpp"
#include "MeshProxy.hpp"
namespace plato::functional::geometry::library::unittest
{
namespace
{
[[nodiscard]] auto make_test_geometry_function() -> FactoryTypes::Compute
{
    return Plato::Functional::make_function(
        [](const linear_algebra::DynamicVector<double>&) { return Plato::Functional::MeshProxy{}; },
        [](const linear_algebra::DynamicVector<double>&) { return linear_algebra::JacobianMultiplier{}; });
}

[[maybe_unused]] static auto kTestGeometryRegistration = GeometryRegistration{
    "test", [](const ValidatedGeometryInput&)
    {
        return FactoryTypes{make_test_geometry_function(), linear_algebra::DynamicVector<double>{},
                            std::make_pair(std::vector<double>{}, std::vector<double>{}),
                            std::function<void(const linear_algebra::DynamicVector<double>&)>{}};
    }};
}  // namespace

TEST(GeometryRegistration, PhonyGeometry) { EXPECT_TRUE(is_geometry_function_registered("test")); }

TEST(GeometryRegistration, BrickGeometry) { EXPECT_TRUE(is_geometry_function_registered("brick_shape_geometry")); }

TEST(GeometryRegistration, DensityTopology) { EXPECT_TRUE(is_geometry_function_registered("density_topology")); }

TEST(GeometryRegistrationUtilities, GeometryInputAllEmpty)
{
    EXPECT_THROW(auto tGeometryInput = first_geometry_input(Plato::PlatoInput{}), Plato::Functional::Exception);
}
}  // namespace plato::functional::geometry::library::unittest