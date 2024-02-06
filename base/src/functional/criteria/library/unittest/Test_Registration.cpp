#include <gtest/gtest.h>

#include "CriterionRegistration.hpp"
#include "DynamicVector.hpp"
#include "Function.hpp"
#include "InputBlocks.hpp"

namespace plato::functional::criteria::library::unittest
{
namespace
{
[[nodiscard]] auto make_test_criterion_function() -> plato::functional::criteria::library::CriterionFunction
{
    return core::make_function([](const core::MeshProxy&) { return 0.0; },
                               [](const core::MeshProxy&) {
                                   return linear_algebra::DynamicVector<double>{1.0, 2.0};
                               });
}

[[maybe_unused]] static auto kTestCriterionRegistration = plato::functional::criteria::library::CriterionRegistration{
    "test", [](const plato::functional::criteria::library::CriterionInput&) { return make_test_criterion_function(); }};

}  // namespace

TEST(CriterionRegistration, PhonyCriterion)
{
    EXPECT_TRUE(plato::functional::criteria::library::is_criterion_function_registered("test"));
}

TEST(CriterionRegistration, NodalSum)
{
    const std::string_view tNodalSumName = Plato::kCodeOptionsTable.toString(Plato::CodeOptions::kNodalSum).value();
    EXPECT_TRUE(plato::functional::criteria::library::is_criterion_function_registered(tNodalSumName));
}

TEST(CriterionRegistration, CustomApp)
{
    const std::string_view tCustomAppName = Plato::kCodeOptionsTable.toString(Plato::CodeOptions::kCustomApp).value();
    EXPECT_TRUE(plato::functional::criteria::library::is_criterion_function_registered(tCustomAppName));
}
}  // namespace plato::functional::criteria::library::unittest
