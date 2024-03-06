#include <gtest/gtest.h>

#include "plato/core/Aggregate.hpp"
#include "plato/core/test_utilities/Utilities.hpp"
#include "plato/core/unittest/TestHelpers.hpp"
#include "plato/test_utilities/Rosenbrock.hpp"

namespace plato::core::unittest
{

TEST(Aggregate, DirectConstruction)
{
    namespace pft = plato::test_utilities;
    run_rosenbrock_aggregator_test(
        [](const detail::FunctionAndWeight& tFunctionAndWeight)
        { return Aggregate<double, pft::TwoDVector, const pft::TwoDVector&>(tFunctionAndWeight); });
}

TEST(Aggregate, UsingMakeFunction)
{
    run_rosenbrock_aggregator_test([](const detail::FunctionAndWeight& tFunctionAndWeight)
                                   { return make_aggregate_function(tFunctionAndWeight); });
}

TEST(Aggregate, OneFunction)
{
    namespace pft = plato::test_utilities;

    const auto tF1 = test_utilities::make_rosenbrock_function(pft::Rosenbrock{});
    const double tW1 = 1.0;
    using RosenbrockF = std::decay_t<decltype(tF1)>;
    using FunctionAndWeight = std::vector<std::pair<RosenbrockF, double>>;
    const auto tAggregate =
        Aggregate<double, pft::TwoDVector, const pft::TwoDVector&>(FunctionAndWeight{std::make_pair(tF1, tW1)});
    {
        const auto tArg = pft::TwoDVector{1.0, 1.0};
        EXPECT_EQ(tAggregate.f(tArg), 0.0);
        const pft::TwoDVector tDfdx = tAggregate.df(tArg);
        EXPECT_EQ(tDfdx(0), 0.0);
        EXPECT_EQ(tDfdx(1), 0.0);
    }
}
}  // namespace plato::core::unittest
