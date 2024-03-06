#include <gtest/gtest.h>

#include "plato/core/Function.hpp"
#include "plato/criteria/library/CriterionRegistration.hpp"
#include "plato/input_parser/InputBlocks.hpp"
#include "plato/linear_algebra/DynamicVector.hpp"

namespace plato::criteria::library::unittest
{

TEST(CriterionRegistration, NodalSum)
{
    const std::string_view tNodalSumName =
        input_parser::kCodeOptionsTable.toString(input_parser::CodeOptions::kNodalSum).value();
    EXPECT_TRUE(plato::criteria::library::is_criterion_function_registered(tNodalSumName));
}

}  // namespace plato::criteria::library::unittest
