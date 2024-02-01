#include <gtest/gtest.h>

#include "Exception.hpp"
#include "FilterFactory.hpp"
#include "InputParser.hpp"

TEST(FilterFactory, HelmholtzFilterThrows)
{
    // For the Helmholtz filter, the filter may be loaded depending on whether
    // or not the PA shared library is available. This checks if we can load it,
    // and if not, checks that we get the right exception type.
    namespace pf = Plato::Functional;

    auto tDensityTopology = Plato::density_topology{};
    tDensityTopology.filter_type = Plato::FilterTypes::kHelmholtz;
    bool tCreationSuccessful = false;
    bool tCorrectException = false;
    try
    {
        const pf::FilterFactory::FilterFunction tFunction = pf::FilterFactory::make_filter_function(tDensityTopology);
        tCreationSuccessful = true;
    }
    catch (const pf::Exception&)
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
