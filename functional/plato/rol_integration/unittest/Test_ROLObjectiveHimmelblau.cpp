#include <gtest/gtest.h>

#include <ROL_StdVector.hpp>
#include <memory>

#include "plato/core/Aggregate.hpp"
#include "plato/rol_integration/ROLObjectiveFunction.hpp"
#include "plato/rol_integration/unittest/DynamicVectorHimmelblauFunction.hpp"
#include "plato/test_utilities/Himmelblau.hpp"

namespace plato::rol_integration::unittest
{
TEST(ROLObjectiveFunction, HimmelblauObjectiveValueAndGradient)
{
    namespace pft = plato::test_utilities;

    auto tObjective = ROLObjectiveFunction{make_himmelblau_dynamic_vector_function(pft::Himmelblau{})};
    const auto tControl = ROL::StdVector<double>{1.0, 1.0};
    double tTolerance;
    EXPECT_EQ(tObjective.value(tControl, tTolerance), 0.0);

    auto tGradient = ROL::StdVector<double>{0.0, 0.0};
    tObjective.gradient(tGradient, tControl, tTolerance);
    EXPECT_EQ(tGradient[0], 0.0);
    EXPECT_EQ(tGradient[1], 0.0);
}

TEST(ROLObjectiveFunction, AggregateTwoHimmelblauObjectives)
{
    namespace pft = plato::test_utilities;

    auto tHimmelblauFunction = make_himmelblau_dynamic_vector_function(pft::Himmelblau{});
    using HimmelblauF = std::decay_t<decltype(tHimmelblauFunction)>;
    auto tObjective = ROLObjectiveFunction{core::make_aggregate_function(
        std::vector<std::pair<HimmelblauF, double>>{std::make_pair(tHimmelblauFunction, 1.0)})};
    const auto tControl = ROL::StdVector<double>{1.0, 1.0};
    double tTolerance;
    EXPECT_EQ(tObjective.value(tControl, tTolerance), 0.0);

    auto tGradient = ROL::StdVector<double>{0.0, 0.0};
    tObjective.gradient(tGradient, tControl, tTolerance);
    EXPECT_EQ(tGradient[0], 0.0);
    EXPECT_EQ(tGradient[1], 0.0);
}
}  // namespace plato::rol_integration::unittest
