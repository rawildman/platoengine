#include <gtest/gtest.h>

#include "FilterValidation.hpp"
#include "HelmholtzFilter.hpp"
#include "IdentityFilter.hpp"
#include "InputGeneration.hpp"
#include "ValidationRegistration.hpp"

namespace plato::functional::filter::library::unittest
{

TEST(FilterValidation, TypeExists)
{
    auto tDensityTopology = plato::functional::test_utilities::create_valid_density_topology_geometry();

    EXPECT_FALSE(validate_filter_type(tDensityTopology).has_value());
    tDensityTopology.filter_type = boost::none;
    EXPECT_TRUE(validate_filter_type(tDensityTopology).has_value());

    // Check in registered function list
    auto tErrorMessages = std::vector<std::string>{};
    tErrorMessages = core::validate(tDensityTopology, std::move(tErrorMessages));
    EXPECT_EQ(tErrorMessages.size(), 1);
}

}  // namespace plato::functional::filter::library::unittest