#include <gtest/gtest.h>

#include "plato/core/Function.hpp"
#include "plato/criteria/extension/NodalSumObjective.hpp"
#include "plato/criteria/extension/VolumeCriterion.hpp"
#include "plato/criteria/library/CriterionRegistration.hpp"
#include "plato/input_parser/InputBlocks.hpp"
#include "plato/linear_algebra/DynamicVector.hpp"

namespace plato::criteria::extension::unittest
{

TEST(CriterionRegistration, NodalSum)
{
    EXPECT_TRUE(library::is_criterion_function_registered(
        library::builtin_criterion_registration_name(NodalSumObjective::kAppName)));
}

TEST(CriterionRegistration, Volume)
{
    EXPECT_TRUE(library::is_criterion_function_registered(
        library::builtin_criterion_registration_name(VolumeCriterion::kVolumeAppName)));
}

TEST(CriterionRegistration, VolumeFraction)
{
    EXPECT_TRUE(library::is_criterion_function_registered(
        library::builtin_criterion_registration_name(VolumeCriterion::kVolumeFractionAppName)));
}

}  // namespace plato::criteria::extension::unittest
