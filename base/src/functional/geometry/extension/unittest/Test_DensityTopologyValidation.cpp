#include <gtest/gtest.h>

#include "DensityTopology.hpp"
#include "Exception.hpp"
#include "GeometryValidation.hpp"
#include "InputGeneration.hpp"

TEST(DensityTopologyValidation, ValidateMeshName)
{
    namespace pf = Plato::Functional;
    auto tDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();
    EXPECT_FALSE(pf::Geometry::detail::validate_mesh_name(tDensityTopology).has_value());
    tDensityTopology.mesh_name = boost::none;
    EXPECT_TRUE(pf::Geometry::detail::validate_mesh_name(tDensityTopology).has_value());
}

TEST(DensityTopologyValidation, ValidateOutputName)
{
    namespace pf = Plato::Functional;
    namespace pfd = Plato::Functional::detail;
    auto tDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();
    EXPECT_FALSE(pfd::validate_output_name(tDensityTopology).has_value());
    tDensityTopology.output_name = boost::none;
    EXPECT_TRUE(pfd::validate_output_name(tDensityTopology).has_value());
}

TEST(DensityTopologyValidation, ValidDensityTopologyInput)
{
    namespace pf = Plato::Functional;
    auto tInput = Plato::PlatoInput{};
    tInput.mDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();

    std::vector<std::string> tMessages;
    tMessages = pf::Geometry::validate_geometry(tInput, std::move(tMessages));
    EXPECT_TRUE(tMessages.empty());
}

TEST(DensityTopologyValidation, InvalidDensityTopologyInput)
{
    namespace pf = Plato::Functional;
    auto tInput = Plato::PlatoInput{};
    tInput.mDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();
    tInput.mDensityTopology->filter_radius = 1.0;

    std::vector<std::string> tMessages;
    tMessages = pf::Geometry::validate_geometry(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 1);
}
