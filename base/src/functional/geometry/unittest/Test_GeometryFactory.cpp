#include <gtest/gtest.h>

#include "Exception.hpp"
#include "GeometryFactory.hpp"
#include "InputGeneration.hpp"
#include "InputParser.hpp"
#include "STKUtilities.hpp"

TEST(GeometryFactory, ValidBrickShapeGeometry)
{
    namespace pf = Plato::Functional;
    auto tInput = Plato::PlatoInput{};
    tInput.mBrickShapeGeometry = Plato::brick_shape_geometry{/*.mMeshName=*/Plato::FileName{"my_mesh.exo"}};
    EXPECT_NO_THROW(auto tData = pf::GeometryFactory::make_geometry_data(tInput));
}

TEST(GeometryFactory, InvalidBrickShapeGeometry)
{
    namespace pf = Plato::Functional;
    auto tInput = Plato::PlatoInput{};
    tInput.mBrickShapeGeometry = Plato::brick_shape_geometry{};
    EXPECT_THROW(auto tData = pf::GeometryFactory::make_geometry_data(tInput), pf::Exception);
}

TEST(GeometryFactory, ValidTopology)
{
    namespace pf = Plato::Functional;

    auto tInput = Plato::PlatoInput{};
    auto tDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();
    tInput.mDensityTopology = tDensityTopology;

    const std::filesystem::path tMeshFileName{tDensityTopology.mesh_name.value().mName};
    pf::write_mesh(tMeshFileName, pf::create_mesh("generated:3x3x4|bbox:-1,-2,-1,2,1,2"));

    EXPECT_NO_THROW(auto tData = pf::GeometryFactory::make_geometry_data(tInput));

    std::filesystem::remove(tMeshFileName);
}

TEST(GeometryFactory, UnknownGeometry)
{
    namespace pf = Plato::Functional;

    const auto tInput = Plato::PlatoInput{};
    EXPECT_THROW(auto tData = pf::GeometryFactory::make_geometry_data(tInput), pf::Exception);
}
