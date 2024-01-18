#include <gtest/gtest.h>

#include "CriterionRegistration.hpp"
#include "DynamicVector.hpp"
#include "Function.hpp"
#include "Plato_InputBlocks.hpp"

namespace
{
[[nodiscard]] auto make_test_criterion_function() -> Plato::Functional::CriterionFactory::CriterionFunction
{
    return Plato::Functional::make_function([](const Plato::Functional::MeshProxy&) { return 0.0; },
                                            [](const Plato::Functional::MeshProxy&) {
                                                return Plato::Functional::Core::DynamicVector<double>{1.0, 2.0};
                                            });
}

[[maybe_unused]] static auto kTestCriterionRegistration = Plato::Functional::CriterionFactory::CriterionRegistration{
    "test", [](const Plato::Functional::CriterionFactory::CriterionInput&) { return make_test_criterion_function(); }};

}  // namespace

TEST(CriterionRegistration, PhonyCriterion)
{
    EXPECT_TRUE(Plato::Functional::CriterionFactory::is_criterion_function_registered("test"));
}

TEST(CriterionRegistration, NodalSum)
{
    const std::string_view tNodalSumName = Plato::kCodeOptionsTable.toString(Plato::CodeOptions::kNodalSum).value();
    EXPECT_TRUE(Plato::Functional::CriterionFactory::is_criterion_function_registered(tNodalSumName));
}

TEST(CriterionRegistration, CustomApp)
{
    const std::string_view tCustomAppName = Plato::kCodeOptionsTable.toString(Plato::CodeOptions::kCustomApp).value();
    EXPECT_TRUE(Plato::Functional::CriterionFactory::is_criterion_function_registered(tCustomAppName));
}
