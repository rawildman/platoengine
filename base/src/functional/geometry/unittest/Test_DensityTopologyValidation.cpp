#include <gtest/gtest.h>

#include "DensityTopology.hpp"
#include "Exception.hpp"
#include "GeometryValidation.hpp"
#include "InputGeneration.hpp"
#include "InputParser.hpp"

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
    namespace pfvdd = Plato::Functional::Validation::DensityTopology::detail;
    auto tDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();
    EXPECT_FALSE(pfvdd::validate_output_name(tDensityTopology).has_value());
    tDensityTopology.output_name = boost::none;
    EXPECT_TRUE(pfvdd::validate_output_name(tDensityTopology).has_value());
}

TEST(DensityTopologyValidation, CheckFilterValuesIdentity)
{
    namespace pf = Plato::Functional;
    namespace pfvdd = Plato::Functional::Validation::DensityTopology::detail;
    auto tDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();

    EXPECT_FALSE(pfvdd::check_filter_values<Plato::FilterTypes::kIdentity>(tDensityTopology).has_value());
    tDensityTopology.filter_radius = 1;
    EXPECT_TRUE(pfvdd::check_filter_values<Plato::FilterTypes::kIdentity>(tDensityTopology).has_value());
    tDensityTopology.boundary_sticking_penalty = 1;
    EXPECT_TRUE(pfvdd::check_filter_values<Plato::FilterTypes::kIdentity>(tDensityTopology).has_value());
    tDensityTopology.filter_radius = boost::none;
    EXPECT_TRUE(pfvdd::check_filter_values<Plato::FilterTypes::kIdentity>(tDensityTopology).has_value());
    tDensityTopology.boundary_sticking_penalty = boost::none;
    EXPECT_FALSE(pfvdd::check_filter_values<Plato::FilterTypes::kIdentity>(tDensityTopology).has_value());
}

TEST(DensityTopologyValidation, CheckFilterValuesHelmholtz)
{
    namespace pf = Plato::Functional;
    namespace pfvdd = Plato::Functional::Validation::DensityTopology::detail;
    auto tDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();
    tDensityTopology.filter_type = Plato::FilterTypes::kHelmholtz;
    tDensityTopology.filter_radius = 1;
    tDensityTopology.boundary_sticking_penalty = 1;
    EXPECT_FALSE(pfvdd::check_filter_values<Plato::FilterTypes::kHelmholtz>(tDensityTopology).has_value());  // valid

    tDensityTopology.filter_radius = boost::none;
    EXPECT_TRUE(pfvdd::check_filter_values<Plato::FilterTypes::kHelmholtz>(tDensityTopology).has_value());
    tDensityTopology.boundary_sticking_penalty = boost::none;
    EXPECT_TRUE(pfvdd::check_filter_values<Plato::FilterTypes::kHelmholtz>(tDensityTopology).has_value());
    tDensityTopology.filter_radius = 1;
    EXPECT_TRUE(pfvdd::check_filter_values<Plato::FilterTypes::kHelmholtz>(tDensityTopology).has_value());
    tDensityTopology.boundary_sticking_penalty = 1;
    EXPECT_FALSE(pfvdd::check_filter_values<Plato::FilterTypes::kHelmholtz>(tDensityTopology).has_value());

    tDensityTopology.boundary_sticking_penalty = -1;
    EXPECT_TRUE(pfvdd::check_filter_values<Plato::FilterTypes::kHelmholtz>(tDensityTopology).has_value());
    tDensityTopology.boundary_sticking_penalty = 1;
    EXPECT_FALSE(pfvdd::check_filter_values<Plato::FilterTypes::kHelmholtz>(tDensityTopology).has_value());

    tDensityTopology.filter_radius = -1;
    EXPECT_TRUE(pfvdd::check_filter_values<Plato::FilterTypes::kHelmholtz>(tDensityTopology).has_value());
}

TEST(DensityTopologyValidation, ValidateFilterIdentity)
{
    namespace pf = Plato::Functional;
    namespace pfvdd = Plato::Functional::Validation::DensityTopology::detail;
    auto tDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();
    EXPECT_FALSE(pfvdd::validate_filter(tDensityTopology).has_value());
    tDensityTopology.filter_type = boost::none;
    EXPECT_TRUE(pfvdd::validate_filter(tDensityTopology).has_value());
    tDensityTopology.filter_type = Plato::FilterTypes::kIdentity;
    EXPECT_FALSE(pfvdd::validate_filter(tDensityTopology).has_value());
    tDensityTopology.filter_radius = 1;
    EXPECT_TRUE(pfvdd::validate_filter(tDensityTopology).has_value());  // Identity filter has no radius, this is an error
    tDensityTopology.filter_radius = boost::none;
    EXPECT_FALSE(pfvdd::validate_filter(tDensityTopology).has_value());
    tDensityTopology.boundary_sticking_penalty = 1;
    EXPECT_TRUE(pfvdd::validate_filter(tDensityTopology).has_value());  // Identity filter has no sticking penalty, this is an error
}

TEST(DensityTopologyValidation, ValidateFilterHelmholtz)
{
    namespace pf = Plato::Functional;
    namespace pfvdd = Plato::Functional::Validation::DensityTopology::detail;
    auto tDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();
    tDensityTopology.filter_radius = 1;
    tDensityTopology.boundary_sticking_penalty = 1;
    tDensityTopology.filter_type = boost::none;
    EXPECT_TRUE(pfvdd::validate_filter(tDensityTopology).has_value());  // missing filter type
    tDensityTopology.filter_type = Plato::FilterTypes::kHelmholtz;
    EXPECT_FALSE(pfvdd::validate_filter(tDensityTopology).has_value());  // valid
    tDensityTopology.filter_radius = boost::none;
    EXPECT_TRUE(pfvdd::validate_filter(tDensityTopology).has_value());  // helmholtz filter must have radius
    tDensityTopology.filter_radius = 1;
    EXPECT_FALSE(pfvdd::validate_filter(tDensityTopology).has_value());  // valid
    tDensityTopology.boundary_sticking_penalty = boost::none;
    EXPECT_TRUE(pfvdd::validate_filter(tDensityTopology).has_value());  // helmholtz filter must have sticking penalty
}