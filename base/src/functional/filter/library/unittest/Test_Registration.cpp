#include <gtest/gtest.h>

#include "filter/library/FilterJacobian.hpp"
#include "filter/library/FilterRegistration.hpp"
#include "core/Function.hpp"
#include "core/MeshProxy.hpp"

namespace input_parser
{
struct density_topology;
}

namespace plato::functional::filter::library::unittest
{
namespace
{
[[nodiscard]] auto make_test_filter_function() -> FilterFunction
{
    return core::make_function([](const core::MeshProxy&) { return core::MeshProxy{}; },
                               [](const core::MeshProxy&) { return FilterJacobian{}; });
}

[[maybe_unused]] static auto kTestFilterRegistration =
    FilterRegistration{"test", [](const input_parser::density_topology&) { return make_test_filter_function(); }};
}  // namespace

TEST(FilterRegistration, PhonyFilter) { EXPECT_TRUE(is_filter_function_registered("test")); }

TEST(FilterRegistration, Identity) { EXPECT_TRUE(is_filter_function_registered("identity")); }
}  // namespace plato::functional::filter::library::unittest
