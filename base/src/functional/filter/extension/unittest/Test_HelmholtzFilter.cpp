#include <gtest/gtest.h>

#include "Exception.hpp"
#include "FilterFactory.hpp"
#include "InputBlocks.hpp"

namespace plato::functional::filter::extension::unittest
{
TEST(FilterFactory, HelmholtzFilterThrows)
{
    // For the Helmholtz filter, the filter may be loaded depending on whether
    // or not the PA shared library is available. This checks if we can load it,
    // and if not, checks that we get the right exception type.
    namespace pff = plato::functional::filter;

    auto tDensityTopology = Plato::density_topology{};
    tDensityTopology.filter_type = Plato::FilterTypes::kHelmholtz;
    bool tCreationSuccessful = false;
    bool tCorrectException = false;
    try
    {
        const pff::library::FilterFunction tFunction = pff::library::make_filter_function(tDensityTopology);
        tCreationSuccessful = true;
    }
    catch (const plato::functional::utilities::Exception&)
    {
        tCorrectException = true;
    }

    if (tCreationSuccessful)
    {
        EXPECT_FALSE(tCorrectException);
    }
    else
    {
        EXPECT_TRUE(tCorrectException);
    }
}
}  // namespace plato::functional::filter::extension::unittest