#include <gtest/gtest.h>

#include "CriterionValidation.hpp"
#include "Plato_InputBlocks.hpp"

namespace
{
template <typename Criteria>
void check_validation_app_and_custom_app()
{
    namespace pfcd = Plato::Functional::Criteria::detail;
    Criteria tCriteria;
    EXPECT_TRUE(pfcd::validate_app(tCriteria).has_value());
    tCriteria.app = Plato::CodeOptions::kCustomApp;
    EXPECT_FALSE(pfcd::validate_app(tCriteria).has_value());
    EXPECT_TRUE(pfcd::validate_custom_app(tCriteria).has_value());
    tCriteria.shared_library_path = Plato::FileName{"/sweet/potato/ravioli.so"};
    EXPECT_FALSE(pfcd::validate_custom_app(tCriteria).has_value());
}

}  // namespace

TEST(CriterionValidation, CheckValidationAppAndCustomAppOnObjective)
{
    check_validation_app_and_custom_app<Plato::objective>();
}

TEST(CriterionValidation, CheckValidationAppAndCustomAppOnConstraint)
{
    check_validation_app_and_custom_app<Plato::constraint>();
}
