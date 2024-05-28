#include <gtest/gtest.h>

#include "plato/criteria/extension/SharedLibCriterion.hpp"

namespace plato::criteria::extension::unittest
{
TEST(SharedLibCriterion, Validation)
{
    auto tCriteria = input_parser::objective{};

    EXPECT_FALSE(detail::validate_custom_app(tCriteria).has_value());
    tCriteria.app = input_parser::AppName{std::string{SharedLibCriterion::kAppName}};
    // Specified custom app, but no shared library given
    EXPECT_TRUE(detail::validate_custom_app(tCriteria).has_value());
    tCriteria.shared_library_path = input_parser::FileName{"/sweet/potato/ravioli.so"};
    EXPECT_FALSE(detail::validate_custom_app(tCriteria).has_value());
}

}  // namespace plato::criteria::extension::unittest
