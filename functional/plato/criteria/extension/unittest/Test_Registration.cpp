#include <gtest/gtest.h>

#include "plato/core/Function.hpp"
#include "plato/criteria/extension/NodalSumObjective.hpp"
#include "plato/criteria/extension/SharedLibCriterion.hpp"
#include "plato/criteria/extension/VolumeCriterion.hpp"
#include "plato/criteria/library/CriterionRegistration.hpp"
#include "plato/input_parser/InputBlocks.hpp"
#include "plato/linear_algebra/DynamicVector.hpp"

namespace plato::criteria::extension::unittest
{

TEST(CriterionRegistration, NodalSum)
{
    EXPECT_TRUE(library::is_criterion_function_registered(NodalSumObjective::kAppName));
}

TEST(CriterionRegistration, Volume)
{
    EXPECT_TRUE(library::is_criterion_function_registered(VolumeCriterion::kVolumeAppName));
}

TEST(CriterionRegistration, VolumeFraction)
{
    EXPECT_TRUE(library::is_criterion_function_registered(VolumeCriterion::kVolumeFractionAppName));
}

TEST(CriterionRegistration, CustomApp)
{
    EXPECT_TRUE(plato::criteria::library::is_criterion_function_registered(SharedLibCriterion::kAppName));
}

TEST(CriterionRegistration, ParallelCustomApp)
{
    EXPECT_TRUE(plato::criteria::library::is_parallel_criterion_function_registered(SharedLibCriterion::kAppName));
}

}  // namespace plato::criteria::extension::unittest
