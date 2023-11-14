#include <gtest/gtest.h>

#include "FilterJacobian.hpp"
#include "Function.hpp"
#include "MeshProxy.hpp"
#include "FilterRegistration.hpp"

namespace Plato
{
struct density_topology;
}
namespace
{
[[nodiscard]] auto make_test_filter_function() -> Plato::Functional::FilterFactory::FilterFunction
{
    return Plato::Functional::make_function(
        [](const Plato::Functional::MeshProxy&) { return Plato::Functional::MeshProxy{}; },
        [](const Plato::Functional::MeshProxy&) { return Plato::Functional::FilterJacobian{}; });
}

static auto kTestFilterRegistration = Plato::Functional::FilterFactory::FilterRegistration{
   "test",
   [](const Plato::density_topology&){ return make_test_filter_function();}
};
}  // namespace

TEST(FilterRegistration, PhonyFilter) 
{ 
   EXPECT_TRUE(Plato::Functional::FilterFactory::is_filter_function_registered("test"));
}

TEST(FilterRegistration, Identity)
{
   EXPECT_TRUE(Plato::Functional::FilterFactory::is_filter_function_registered("identity"));
}
