#include <gtest/gtest.h>

#include "Exception.hpp"
#include "FilterFactory.hpp"
#include "InputParser.hpp"

TEST(FilterFactory, ValidIdentityFilter)
{
    auto tDensityTopology = Plato::density_topology{};
    tDensityTopology.filter_type = Plato::FilterTypes::kIdentity;
    EXPECT_NO_THROW(auto tFunction = Plato::Functional::FilterFactory::make_filter_function(tDensityTopology));
}

TEST(FilterFactory, KernelFilterThrows)
{
    auto tDensityTopology = Plato::density_topology{};
    tDensityTopology.filter_type = Plato::FilterTypes::kKernel;
    EXPECT_THROW(auto tFunction = Plato::Functional::FilterFactory::make_filter_function(tDensityTopology),
                 Plato::Functional::Exception);
}

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
