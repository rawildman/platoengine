#include <gtest/gtest.h>

#include "Exception.hpp"
#include "GeometryFactory.hpp"
#include "InputParser.hpp"
#include "STKUtilities.hpp"

TEST(GeometryFactory, ValidBrickShapeGeometry)
{
    namespace pf = Plato::Functional;
    auto tInput = Plato::PlatoInput{};
    tInput.mBrickShapeGeometry = Plato::brick_shape_geometry{/*.mMeshName=*/Plato::FileName{"my_mesh.exo"}};
    EXPECT_NO_THROW(auto tFunction = pf::GeometryFactory::make_geometry_function(tInput));
}

TEST(GeometryFactory, InvalidBrickShapeGeometry)
{
    namespace pf = Plato::Functional;
    auto tInput = Plato::PlatoInput{};
    tInput.mBrickShapeGeometry = Plato::brick_shape_geometry{};
    EXPECT_THROW(auto tFunction = pf::GeometryFactory::make_geometry_function(tInput), pf::Exception);
}

TEST(GeometryFactory, ValidTopology)
{
    namespace pf = Plato::Functional;

    const std::string tFileName = "test.exo";
    pf::write_mesh(tFileName, pf::create_mesh("generated:3x3x4|bbox:-1,-2,-1,2,1,2"));

    auto tData = Plato::PlatoInput{};
    auto tDensityTopology = Plato::density_topology{};
    tDensityTopology.mesh_name = Plato::FileName{tFileName};
    tData.mDensityTopology = tDensityTopology;

    EXPECT_NO_THROW(auto tFunction = pf::GeometryFactory::make_geometry_function(tData));
    EXPECT_NO_THROW(auto tFunction = pf::GeometryFactory::make_initial_guess(tData));
    EXPECT_NO_THROW(auto tFunction = pf::GeometryFactory::make_bound_constraint(tData));

    std::filesystem::remove(tFileName);
}

TEST(GeometryFactory, InvalidTopologyNoMesh)
{
    namespace pf = Plato::Functional;

    auto tData = Plato::PlatoInput{};
    auto tDensityTopology = Plato::density_topology{};
    tData.mDensityTopology = tDensityTopology;

    EXPECT_THROW(auto tFunction = pf::GeometryFactory::make_geometry_function(tData), pf::Exception);
}

TEST(GeometryFactory, NoGeometry)
{
    namespace pf = Plato::Functional;

    const auto tData = Plato::PlatoInput{};

    EXPECT_THROW(auto tFunction = pf::GeometryFactory::make_geometry_function(tData), pf::Exception);
    EXPECT_THROW(auto tFunction = pf::GeometryFactory::make_initial_guess(tData), pf::Exception);
    EXPECT_THROW(auto tFunction = pf::GeometryFactory::make_bound_constraint(tData), pf::Exception);
    EXPECT_THROW(auto tFunction = pf::GeometryFactory::make_output_function(tData), pf::Exception);
}
