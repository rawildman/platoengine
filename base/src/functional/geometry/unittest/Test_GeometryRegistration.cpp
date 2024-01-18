#include <gtest/gtest.h>

#include <ROL_StdVector.hpp>

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
        [](const ROL::StdVector<double>&) { return Plato::Functional::MeshProxy{}; },
        [](const ROL::StdVector<double>&) { return Plato::Functional::JacobianMultiplier{}; });
}

[[maybe_unused]] static auto kTestGeometryRegistration = Plato::Functional::GeometryFactory::GeometryRegistration{
    "test", [](const Plato::Functional::GeometryFactory::ValidatedGeometryInput&)
    {
        return Plato::Functional::GeometryFactory::FactoryTypes{
            make_test_geometry_function(), nullptr, std::make_pair(std::vector<double>{}, std::vector<double>{}),
            std::function<void(const ROL::StdVector<double>&)>{}};
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
