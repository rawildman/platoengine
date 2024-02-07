#include <gtest/gtest.h>

#include "core/ValidationRegistration.hpp"
#include "filter/extension/HelmholtzFilter.hpp"
#include "filter/extension/IdentityFilter.hpp"
#include "filter/library/FilterValidation.hpp"
#include "test_utilities/InputGeneration.hpp"

namespace plato::functional::filter::extension::unittest
{
TEST(FilterValidation, CheckFilterValuesIdentity)
{
    auto tDensityTopology = plato::functional::test_utilities::create_valid_density_topology_geometry();

    EXPECT_FALSE(validate_identity_filter(tDensityTopology).has_value());
    tDensityTopology.filter_radius = 1;
    EXPECT_TRUE(validate_identity_filter(tDensityTopology).has_value());
    tDensityTopology.boundary_sticking_penalty = 1;
    EXPECT_TRUE(validate_identity_filter(tDensityTopology).has_value());
    tDensityTopology.filter_radius = boost::none;
    EXPECT_TRUE(validate_identity_filter(tDensityTopology).has_value());
    tDensityTopology.boundary_sticking_penalty = boost::none;
    EXPECT_FALSE(validate_identity_filter(tDensityTopology).has_value());

    // Check in registered function list
    tDensityTopology.filter_radius = 1;  // Make invalid
    auto tErrorMessages = std::vector<std::string>{};
    tErrorMessages = core::validate(tDensityTopology, std::move(tErrorMessages));
    EXPECT_EQ(tErrorMessages.size(), 1);
}

TEST(FilterValidation, CheckFilterValuesHelmholtzRadius)
{
    auto tDensityTopology = plato::functional::test_utilities::create_valid_density_topology_geometry();
    tDensityTopology.filter_type = input_parser::FilterTypes::kHelmholtz;
    tDensityTopology.filter_radius = 1;
    tDensityTopology.boundary_sticking_penalty = 1;
    EXPECT_FALSE(validate_helmholtz_filter_radius(tDensityTopology).has_value());  // valid

    tDensityTopology.filter_radius = boost::none;
    EXPECT_TRUE(validate_helmholtz_filter_radius(tDensityTopology).has_value());
    tDensityTopology.filter_radius = -1;
    EXPECT_TRUE(validate_helmholtz_filter_radius(tDensityTopology).has_value());

    // Check in registered function list
    tDensityTopology.filter_radius = boost::none;  // Make invalid
    auto tErrorMessages = std::vector<std::string>{};
    tErrorMessages = core::validate(tDensityTopology, std::move(tErrorMessages));
    EXPECT_EQ(tErrorMessages.size(), 1);
}

TEST(FilterValidation, CheckFilterValuesHelmholtzBoundaryStickingPenalty)
{
    auto tDensityTopology = plato::functional::test_utilities::create_valid_density_topology_geometry();
    tDensityTopology.filter_type = input_parser::FilterTypes::kHelmholtz;
    tDensityTopology.filter_radius = 1;
    tDensityTopology.boundary_sticking_penalty = 1;
    EXPECT_FALSE(validate_helmholtz_filter_boundary_sticking_penalty(tDensityTopology).has_value());  // valid
    tDensityTopology.boundary_sticking_penalty = boost::none;
    EXPECT_FALSE(validate_helmholtz_filter_boundary_sticking_penalty(tDensityTopology).has_value());  // valid, optional
    tDensityTopology.boundary_sticking_penalty = -1;
    EXPECT_TRUE(validate_helmholtz_filter_boundary_sticking_penalty(tDensityTopology).has_value());  // invalid

    // Check in registered function list
    tDensityTopology.boundary_sticking_penalty = -1;  // Make invalid
    auto tErrorMessages = std::vector<std::string>{};
    tErrorMessages = core::validate(tDensityTopology, std::move(tErrorMessages));
    EXPECT_EQ(tErrorMessages.size(), 1);
}

}  // namespace plato::functional::filter::extension::unittest
