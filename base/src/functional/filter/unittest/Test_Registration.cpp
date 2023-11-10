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

static auto kTestFilterRegistration = Plato::Functional::FilterFactory::Registration{
   "test",
   [](const Plato::density_topology&){ return make_test_filter_function();}
};
}  // namespace

TEST(FilterRegistration, PhonyFilter) 
{ 
   namespace pf = Plato::Functional;
   // Test that the test filter was registered
   EXPECT_NO_THROW(pf::FilterFactory::detail::registered_functions().at("test"));
}

TEST(FilterRegistration, Identity)
{
   namespace pf = Plato::Functional;
   EXPECT_NO_THROW(pf::FilterFactory::detail::registered_functions().at("identity"));
}
