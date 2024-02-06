#include <gtest/gtest.h>

#include "Exception.hpp"
#include "FilterFactory.hpp"
#include "InputBlocks.hpp"
#include "InputEnumTypes.hpp"

namespace plato::functional::filter::extension::unittest
{
TEST(FilterFactory, KernelFilterThrows)
{
    auto tDensityTopology = Plato::density_topology{};
    tDensityTopology.filter_type = Plato::FilterTypes::kKernel;
    EXPECT_THROW(auto tFunction = plato::functional::filter::library::make_filter_function(tDensityTopology),
                 plato::functional::utilities::Exception);
}
}  // namespace plato::functional::filter::extension::unittest