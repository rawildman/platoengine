#ifndef PLATO_CORE_UNITTEST_TESTHELPERS
#define PLATO_CORE_UNITTEST_TESTHELPERS

#include <gtest/gtest.h>

#include "plato/core/Aggregate.hpp"
#include "plato/core/Function.hpp"

namespace plato::core::unittest
{

namespace
{
namespace pft = plato::test_utilities;
using RosenbrockF = std::decay_t<decltype(test_utilities::make_rosenbrock_function(pft::Rosenbrock{}))>;
using FunctionAndWeight = std::vector<std::pair<RosenbrockF, double>>;
}  // namespace

template <typename Return>
void run_rosenbrock_aggregator_test(std::function<Return(const FunctionAndWeight&)> aGenerateCommand)
{
    const std::pair<double, double> tRosenbrockParams{2.0, 3.0};
    const std::pair<double, double> tWeights{.25, .5};
    const auto tF1 = test_utilities::make_rosenbrock_function(pft::Rosenbrock{});
    const auto tF2 =
        test_utilities::make_rosenbrock_function(pft::Rosenbrock{tRosenbrockParams.first, tRosenbrockParams.second});

    const auto tFunctionAndWeight =
        FunctionAndWeight{std::make_pair(tF1, tWeights.first), std::make_pair(tF2, tWeights.second)};
    const auto tAggregate = aGenerateCommand(tFunctionAndWeight);

    for (auto aTestPoint : {pft::TwoDVector{0.0, 1.0}, pft::TwoDVector{1.0, 1.0}, pft::TwoDVector{-1.0, 2.0}})
    {
        EXPECT_EQ(tAggregate.f(aTestPoint), tWeights.first * tF1.f(aTestPoint) + tWeights.second * tF2.f(aTestPoint));
        EXPECT_EQ(tAggregate.df(aTestPoint),
                  tWeights.first * tF1.df(aTestPoint) + tWeights.second * tF2.df(aTestPoint));
    }
}

}  // namespace plato::core::unittest

#endif