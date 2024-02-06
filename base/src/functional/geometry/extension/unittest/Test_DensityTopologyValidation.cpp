#include <gtest/gtest.h>

#include "geometry/extension/DensityTopology.hpp"
#include "utilities/Exception.hpp"
#include "geometry/library/GeometryValidation.hpp"
#include "test_utilities/InputGeneration.hpp"
namespace plato::functional::geometry::extension::unittest
{
TEST(DensityTopologyValidation, ValidateMeshName)
{
    auto tDensityTopology = plato::functional::test_utilities::create_valid_density_topology_geometry();
    EXPECT_FALSE(library::detail::validate_mesh_name(tDensityTopology).has_value());
    tDensityTopology.mesh_name = boost::none;
    EXPECT_TRUE(library::detail::validate_mesh_name(tDensityTopology).has_value());
}

TEST(DensityTopologyValidation, ValidateOutputName)
{
    auto tDensityTopology = plato::functional::test_utilities::create_valid_density_topology_geometry();
    EXPECT_FALSE(detail::validate_output_name(tDensityTopology).has_value());
    tDensityTopology.output_name = boost::none;
    EXPECT_TRUE(detail::validate_output_name(tDensityTopology).has_value());
}

TEST(DensityTopologyValidation, ValidDensityTopologyInput)
{
    auto tInput = input_parser::ParsedInput{};
    tInput.mDensityTopology = plato::functional::test_utilities::create_valid_density_topology_geometry();

    std::vector<std::string> tMessages;
    tMessages = library::validate_geometry(tInput, std::move(tMessages));
    EXPECT_TRUE(tMessages.empty());
}

TEST(DensityTopologyValidation, InvalidDensityTopologyInput)
{
    auto tInput = input_parser::ParsedInput{};
    tInput.mDensityTopology = plato::functional::test_utilities::create_valid_density_topology_geometry();
    tInput.mDensityTopology->filter_radius = 1.0;

    std::vector<std::string> tMessages;
    tMessages = library::validate_geometry(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 1);
}
}  // namespace plato::functional::geometry::extension::unittest