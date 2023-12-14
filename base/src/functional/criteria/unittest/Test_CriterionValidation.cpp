#include <gtest/gtest.h>

#include "AffirmUtilities.hpp"
#include "ObjectiveValidation.hpp"
#include "Plato_InputBlocks.hpp"

TEST(CriterionValidation, ValidateApp)
{
    namespace pfcd = Plato::Functional::Criteria::detail;
    Plato::objective tObjective;
    EXPECT_TRUE(pfcd::validate_app(tObjective).has_value());
    tObjective.app = Plato::CodeOptions::kCustomApp;
    EXPECT_FALSE(pfcd::validate_app(tObjective).has_value());
}

TEST(CriterionValidation, ValidateCustomApp)
{
    namespace pfcd = Plato::Functional::Criteria::detail;
    Plato::constraint tConstraint;
    tConstraint.app = Plato::CodeOptions::kCustomApp;
    EXPECT_TRUE(pfcd::validate_custom_app(tConstraint).has_value());
    tConstraint.shared_library_path = Plato::FileName{"/sweet/potato/ravioli.so"};
    EXPECT_FALSE(pfcd::validate_custom_app(tConstraint).has_value());
}
