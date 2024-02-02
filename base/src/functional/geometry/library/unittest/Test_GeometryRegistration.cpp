#include <gtest/gtest.h>

#include "DynamicVector.hpp"
#include "Function.hpp"
#include "GeometryRegistration.hpp"
#include "JacobianMultiplier.hpp"
#include "MeshProxy.hpp"
#include "Plato_InputBlocks.hpp"

namespace
{
[[nodiscard]] auto make_test_geometry_function() -> Plato::Functional::GeometryFactory::FactoryTypes::Compute
{
    return Plato::Functional::make_function(
        [](const Plato::Functional::Core::DynamicVector<double>&) { return Plato::Functional::MeshProxy{}; },
        [](const Plato::Functional::Core::DynamicVector<double>&) { return Plato::Functional::JacobianMultiplier{}; });
}

[[maybe_unused]] static auto kTestGeometryRegistration = Plato::Functional::GeometryFactory::GeometryRegistration{
    "test", [](const Plato::Functional::GeometryFactory::ValidatedGeometryInput&)
    {
        return Plato::Functional::GeometryFactory::FactoryTypes{
            make_test_geometry_function(), Plato::Functional::Core::DynamicVector<double>{},
            std::make_pair(std::vector<double>{}, std::vector<double>{}),
            std::function<void(const Plato::Functional::Core::DynamicVector<double>&)>{}};
    }};
}  // namespace

TEST(GeometryRegistration, PhonyGeometry)
{
    EXPECT_TRUE(Plato::Functional::GeometryFactory::is_geometry_function_registered("test"));
}

TEST(GeometryRegistration, BrickGeometry)
{
    EXPECT_TRUE(Plato::Functional::GeometryFactory::is_geometry_function_registered("brick_shape_geometry"));
}

TEST(GeometryRegistration, DensityTopology)
{
    EXPECT_TRUE(Plato::Functional::GeometryFactory::is_geometry_function_registered("density_topology"));
}
