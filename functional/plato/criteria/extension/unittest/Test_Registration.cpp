#include <gtest/gtest.h>

#include "plato/core/Function.hpp"
#include "plato/criteria/library/CriterionRegistration.hpp"
#include "plato/input_parser/InputBlocks.hpp"
#include "plato/linear_algebra/DynamicVector.hpp"

namespace plato::criteria::extension::unittest
{

TEST(CriterionRegistration, NodalSum)
{
    const std::string tNodalSumName =
        input_parser::kCodeOptionsTable.toString(input_parser::CodeOptions::kNodalSum).value();
    EXPECT_TRUE(library::is_criterion_function_registered(tNodalSumName));
}

TEST(CriterionRegistration, VolumeConstraint)
{
    const std::string tVolumeConstraintName =
        input_parser::kCodeOptionsTable.toString(input_parser::CodeOptions::kVolume).value();
    EXPECT_TRUE(library::is_criterion_function_registered(tVolumeConstraintName));
}

TEST(CriterionRegistration, VolumeFractionConstraint)
{
    const std::string tVolumeFractionConstraintName =
        input_parser::kCodeOptionsTable.toString(input_parser::CodeOptions::kVolumeFraction).value();
    EXPECT_TRUE(library::is_criterion_function_registered(tVolumeFractionConstraintName));
}

}  // namespace plato::criteria::extension::unittest
