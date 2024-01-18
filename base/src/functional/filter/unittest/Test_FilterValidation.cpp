#include <gtest/gtest.h>

#include "FilterValidation.hpp"
#include "HelmholtzFilter.hpp"
#include "IdentityFilter.hpp"
#include "InputGeneration.hpp"
#include "ValidationRegistration.hpp"

TEST(FilterValidation, TypeExists)
{
    namespace pf = Plato::Functional;
    namespace pff = pf::Filter;
    auto tDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();

    EXPECT_FALSE(pff::validate_filter_type(tDensityTopology).has_value());
    tDensityTopology.filter_type = boost::none;
    EXPECT_TRUE(pff::validate_filter_type(tDensityTopology).has_value());

    // Check in registered function list
    auto tErrorMessages = std::vector<std::string>{};
    tErrorMessages = Plato::Functional::Validation::validate(tDensityTopology, std::move(tErrorMessages));
    EXPECT_EQ(tErrorMessages.size(), 1);
}

TEST(FilterValidation, CheckFilterValuesIdentity)
{
    namespace pf = Plato::Functional;
    auto tDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();

    EXPECT_FALSE(pf::validate_identity_filter(tDensityTopology).has_value());
    tDensityTopology.filter_radius = 1;
    EXPECT_TRUE(pf::validate_identity_filter(tDensityTopology).has_value());
    tDensityTopology.boundary_sticking_penalty = 1;
    EXPECT_TRUE(pf::validate_identity_filter(tDensityTopology).has_value());
    tDensityTopology.filter_radius = boost::none;
    EXPECT_TRUE(pf::validate_identity_filter(tDensityTopology).has_value());
    tDensityTopology.boundary_sticking_penalty = boost::none;
    EXPECT_FALSE(pf::validate_identity_filter(tDensityTopology).has_value());

    // Check in registered function list
    tDensityTopology.filter_radius = 1;  // Make invalid
    auto tErrorMessages = std::vector<std::string>{};
    tErrorMessages = Plato::Functional::Validation::validate(tDensityTopology, std::move(tErrorMessages));
    EXPECT_EQ(tErrorMessages.size(), 1);
}

TEST(FilterValidation, CheckFilterValuesHelmholtzRadius)
{
    namespace pf = Plato::Functional;
    auto tDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();
    tDensityTopology.filter_type = Plato::FilterTypes::kHelmholtz;
    tDensityTopology.filter_radius = 1;
    tDensityTopology.boundary_sticking_penalty = 1;
    EXPECT_FALSE(pf::validate_helmholtz_filter_radius(tDensityTopology).has_value());  // valid

    tDensityTopology.filter_radius = boost::none;
    EXPECT_TRUE(pf::validate_helmholtz_filter_radius(tDensityTopology).has_value());
    tDensityTopology.filter_radius = -1;
    EXPECT_TRUE(pf::validate_helmholtz_filter_radius(tDensityTopology).has_value());

    // Check in registered function list
    tDensityTopology.filter_radius = boost::none;  // Make invalid
    auto tErrorMessages = std::vector<std::string>{};
    tErrorMessages = Plato::Functional::Validation::validate(tDensityTopology, std::move(tErrorMessages));
    EXPECT_EQ(tErrorMessages.size(), 1);
}

TEST(FilterValidation, CheckFilterValuesHelmholtzBoundaryStickingPenalty)
{
    namespace pf = Plato::Functional;
    auto tDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();
    tDensityTopology.filter_type = Plato::FilterTypes::kHelmholtz;
    tDensityTopology.filter_radius = 1;
    tDensityTopology.boundary_sticking_penalty = 1;
    EXPECT_FALSE(pf::validate_helmholtz_filter_boundary_sticking_penalty(tDensityTopology).has_value());  // valid
    tDensityTopology.boundary_sticking_penalty = boost::none;
    EXPECT_FALSE(
        pf::validate_helmholtz_filter_boundary_sticking_penalty(tDensityTopology).has_value());  // valid, optional
    tDensityTopology.boundary_sticking_penalty = -1;
    EXPECT_TRUE(pf::validate_helmholtz_filter_boundary_sticking_penalty(tDensityTopology).has_value());  // invalid

    // Check in registered function list
    tDensityTopology.boundary_sticking_penalty = -1;  // Make invalid
    auto tErrorMessages = std::vector<std::string>{};
    tErrorMessages = Plato::Functional::Validation::validate(tDensityTopology, std::move(tErrorMessages));
    EXPECT_EQ(tErrorMessages.size(), 1);
}
