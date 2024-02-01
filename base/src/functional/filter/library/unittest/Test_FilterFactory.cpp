#include <gtest/gtest.h>

#include "Exception.hpp"
#include "FilterFactory.hpp"
#include "InputParser.hpp"

TEST(FilterFactory, KernelFilterThrows)
{
    auto tDensityTopology = Plato::density_topology{};
    tDensityTopology.filter_type = Plato::FilterTypes::kKernel;
    EXPECT_THROW(auto tFunction = Plato::Functional::FilterFactory::make_filter_function(tDensityTopology),
                 Plato::Functional::Exception);
}