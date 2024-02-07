#include <gtest/gtest.h>

#include "filter/library/FilterFactory.hpp"
#include "input_parser/InputBlocks.hpp"
#include "input_parser/InputEnumTypes.hpp"
#include "utilities/Exception.hpp"

namespace plato::functional::filter::extension::unittest
{
TEST(FilterFactory, KernelFilterThrows)
{
    auto tDensityTopology = input_parser::density_topology{};
    tDensityTopology.filter_type = input_parser::FilterTypes::kKernel;
    EXPECT_THROW(auto tFunction = plato::functional::filter::library::make_filter_function(tDensityTopology),
                 plato::functional::utilities::Exception);
}
}  // namespace plato::functional::filter::extension::unittest