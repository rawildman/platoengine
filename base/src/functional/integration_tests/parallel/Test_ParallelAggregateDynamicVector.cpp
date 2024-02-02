#include <gtest/gtest.h>

#include <boost/mpi/communicator.hpp>

#include "DynamicVector.hpp"
#include "DynamicVectorSerialization.hpp"
#include "DynamicVectorRosenbrockFunction.hpp"
#include "ParallelAggregate.hpp"
#include "Rosenbrock.hpp"

namespace
{
constexpr auto kNumRanks = int{4};
}

TEST(ParallelAggregateDynamicVector, MPISize)
{
    auto tComm = boost::mpi::communicator{};
    EXPECT_EQ(tComm.size(), kNumRanks);
}

TEST(ParallelAggregateDynamicVector, ParallelAggregateTwoRosenbrockObjectives)
{
    namespace pf = Plato::Functional;
    namespace pfc = pf::Core;
    namespace pft = pf::Test;

    // This assumes this test is running in parallel w/ `kNumRanks` number of ranks.
    // Each rank constructs a `ParallelAggregate` object with a single function, so that
    // the resulting aggregation should be the evaluation of one function times the number
    // of ranks.
    const auto tRosenbrockFunction = pft::make_rosenbrock_dynamic_vector_function(pft::Rosenbrock{});
    using RosenbrockF = std::decay_t<decltype(tRosenbrockFunction)>;
    constexpr auto tWeight = double{0.5};

    using FunctionAndWeight = std::vector<std::pair<RosenbrockF, double>>;
    const auto tAggregate =
        pf::ParallelAggregate<double, pfc::DynamicVector<double>, const pfc::DynamicVector<double>&>(
            FunctionAndWeight{std::make_pair(tRosenbrockFunction, tWeight)}, boost::mpi::communicator{});

    const auto tControl = pfc::DynamicVector{1.0, -2.0};
    const double tExpectedF = kNumRanks * tWeight * tRosenbrockFunction.f(tControl);
    const double tComputedF = tAggregate.f(tControl);
    EXPECT_EQ(tComputedF, tExpectedF);
    const pfc::DynamicVector<double> tExpectedDF = kNumRanks * tWeight * tRosenbrockFunction.df(tControl);
    const pfc::DynamicVector<double> tComputedDF = tAggregate.df(tControl);
    EXPECT_EQ(tComputedDF, tExpectedDF);
}
