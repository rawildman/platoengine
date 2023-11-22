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

    const std::filesystem::path tMeshFileName = "test.exo";
    const std::filesystem::path tOutFileName = "test_out.exo";
    pf::write_mesh(tMeshFileName, pf::create_mesh("generated:3x3x4|bbox:-1,-2,-1,2,1,2"));

    auto tInput = Plato::PlatoInput{};
    auto tDensityTopology = Plato::density_topology{};
    tDensityTopology.mesh_name = Plato::FileName{tMeshFileName.string()};
    tDensityTopology.output_name = Plato::FileName{tOutFileName.string()};
    tDensityTopology.filter_type = Plato::FilterTypes::kIdentity;
    tInput.mDensityTopology = tDensityTopology;

    EXPECT_NO_THROW(auto tData = pf::GeometryFactory::make_geometry_data(tInput));

    std::filesystem::remove(tMeshFileName);
}

TEST(GeometryFactory, InvalidTopologyNoMesh)
{
    namespace pf = Plato::Functional;

    auto tInput = Plato::PlatoInput{};
    auto tDensityTopology = Plato::density_topology{};
    tInput.mDensityTopology = tDensityTopology;

    EXPECT_THROW(auto tData = pf::GeometryFactory::make_geometry_data(tInput), pf::Exception);
}

TEST(GeometryFactory, NoGeometry)
{
    namespace pf = Plato::Functional;

    const auto tInput = Plato::PlatoInput{};

    EXPECT_THROW(auto tData = pf::GeometryFactory::make_geometry_data(tInput), pf::Exception);
}
