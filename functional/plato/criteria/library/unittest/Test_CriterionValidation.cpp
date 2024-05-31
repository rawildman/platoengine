#include <gtest/gtest.h>

#include "plato/criteria/library/CriterionValidation.hpp"
#include "plato/input_parser/InputBlocks.hpp"

namespace plato::criteria::library::unittest
{
namespace
{
template <typename Criteria>
void check_validation_app_and_custom_app()
{
    Criteria tCriteria;
    EXPECT_TRUE(detail::validate_app_is_registered(tCriteria).has_value());

    tCriteria.app = input_parser::AppName{"definitely not an app"};
    EXPECT_TRUE(detail::validate_app_is_registered(tCriteria).has_value());

    tCriteria.app = input_parser::AppName{"custom_app"};
    EXPECT_FALSE(detail::validate_app_is_registered(tCriteria).has_value());

    EXPECT_FALSE(detail::validate_number_of_processors(tCriteria).has_value());
    tCriteria.number_of_processors = 0;
    EXPECT_TRUE(detail::validate_number_of_processors(tCriteria).has_value());
    tCriteria.number_of_processors = 1;
    EXPECT_FALSE(detail::validate_number_of_processors(tCriteria).has_value());
}

}  // namespace

TEST(CriterionValidation, CheckValidationAppAndCustomAppOnObjective)
{
    check_validation_app_and_custom_app<input_parser::objective>();
}

TEST(CriterionValidation, CheckValidationAppAndCustomAppOnConstraint)
{
    check_validation_app_and_custom_app<input_parser::constraint>();
}
}  // namespace plato::criteria::library::unittest
