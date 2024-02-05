#include <gtest/gtest.h>

#include "FilterJacobian.hpp"
#include "FilterRegistration.hpp"
#include "Function.hpp"
#include "MeshProxy.hpp"

namespace Plato
{
struct density_topology;
}

namespace plato::functional::filter::library::unittest
{

namespace
{
[[nodiscard]] auto make_test_filter_function() -> FilterFunction
{
    return Plato::Functional::make_function([](const Plato::Functional::MeshProxy&)
                                            { return Plato::Functional::MeshProxy{}; },
                                            [](const Plato::Functional::MeshProxy&) { return FilterJacobian{}; });
}

[[maybe_unused]] static auto kTestFilterRegistration =
    FilterRegistration{"test", [](const Plato::density_topology&) { return make_test_filter_function(); }};
}  // namespace

TEST(FilterRegistration, PhonyFilter) { EXPECT_TRUE(is_filter_function_registered("test")); }

TEST(FilterRegistration, Identity) { EXPECT_TRUE(is_filter_function_registered("identity")); }
}  // namespace plato::functional::filter::library::unittest
