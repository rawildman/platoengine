#include <gtest/gtest.h>

#include "DensityTopology.hpp"
#include "Exception.hpp"
#include "GeometryValidation.hpp"
#include "InputGeneration.hpp"
#include "InputParser.hpp"

TEST(DensityTopologyValidation, ValidTopology)
{
    namespace pf = Plato::Functional;
    const auto tDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();
    EXPECT_FALSE(pf::Geometry::detail::validate_mesh_name(tDensityTopology).has_value());
}

TEST(DensityTopologyValidation, InvalidTopologyNoMeshName)
{
    namespace pf = Plato::Functional;
    auto tDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();
    tDensityTopology.mesh_name = boost::none;
    EXPECT_TRUE(pf::Geometry::detail::validate_mesh_name(tDensityTopology).has_value());
}

TEST(DensityTopologyValidation, InvalidTopologyNoOutputMeshName)
{
    namespace pf = Plato::Functional;
    auto tDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();
    EXPECT_FALSE(pf::Validation::DensityTopology::detail::validate_output_name(tDensityTopology).has_value());
    tDensityTopology.output_name = boost::none;
    EXPECT_TRUE(pf::Validation::DensityTopology::detail::validate_output_name(tDensityTopology).has_value());
}