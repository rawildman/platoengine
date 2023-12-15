#include <gtest/gtest.h>

#include "DensityTopology.hpp"
#include "Exception.hpp"
#include "InputGeneration.hpp"
#include "InputParser.hpp"

TEST(DensityTopologyValidation, ValidTopology)
{
    namespace pf = Plato::Functional;
    const auto tDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();
    EXPECT_FALSE(pf::detail::validate_mesh_name(tDensityTopology).has_value());
}

TEST(DensityTopologyValidation, InvalidTopologyNoMesh)
{
    namespace pf = Plato::Functional;
    const auto tDensityTopology = Plato::density_topology{};
    EXPECT_TRUE(pf::detail::validate_mesh_name(tDensityTopology).has_value());
}
