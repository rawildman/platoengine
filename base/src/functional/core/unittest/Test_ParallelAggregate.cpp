#include <gtest/gtest.h>

#include <boost/mpi/communicator.hpp>

#include "ParallelAggregate.hpp"
#include "Rosenbrock.hpp"
#include "TwoDTestTypesSerialization.hpp"
#include "Utilities.hpp"

namespace
{
constexpr auto kNumRanks = int{2};
}  // namespace

TEST(ParallelAggregate, MPISize)
{
    auto tComm = boost::mpi::communicator{};
    EXPECT_EQ(tComm.size(), kNumRanks);
}

TEST(ParallelAggregate, EvaluateSame)
{
    namespace pf = Plato::Functional;
    namespace pft = Plato::Functional::Test;

    // This assumes this test is running in parallel w/ `kNumRanks` number of ranks.
    // Each rank constructs a `ParallelAggregate` object with a single function, so that
    // the resulting aggregation should be the evaluation of one function times the number
    // of ranks.
    const auto tF = make_rosenbrock_function(pft::Rosenbrock{});
    const double tW = 0.5;
    using RosenbrockF = std::decay_t<decltype(tF)>;
    using FunctionAndWeight = std::vector<std::pair<RosenbrockF, double>>;
    const auto tAggregate = pf::ParallelAggregate<double, pft::TwoDVector, const pft::TwoDVector&>(
        FunctionAndWeight{std::make_pair(tF, tW)}, boost::mpi::communicator{});

    EXPECT_EQ(tAggregate.size(), 1);

    const auto tArg = pft::TwoDVector{2.0, -1.0};
    const double tExpectedF = kNumRanks * tW * tF.f(tArg);
    EXPECT_EQ(tAggregate.f(tArg), tExpectedF);
    const pft::TwoDVector tExpectedDF = kNumRanks * tW * tF.df(tArg);
    EXPECT_EQ(tAggregate.df(tArg), tExpectedDF);
}

TEST(ParallelAggregate, EvaluateDifferent)
{
    namespace pf = Plato::Functional;
    namespace pft = Plato::Functional::Test;

    const auto tCommunicator = boost::mpi::communicator{};
    constexpr double tA = 2.0;
    constexpr double tB = 50.0;
    const auto tFNonDefault = make_rosenbrock_function(pft::Rosenbrock{tA, tB});
    const auto tFDefault = make_rosenbrock_function(pft::Rosenbrock{});

    using RosenbrockF = std::decay_t<decltype(tFNonDefault)>;
    using FunctionAndWeight = std::vector<std::pair<RosenbrockF, double>>;
    constexpr auto tW = double{0.5};
    const auto tFunctionAndWeight = tCommunicator.rank() == 0 ? FunctionAndWeight{std::make_pair(tFDefault, tW)}
                                                              : FunctionAndWeight{std::make_pair(tFNonDefault, tW)};
    const auto tAggregate =
        pf::ParallelAggregate<double, pft::TwoDVector, const pft::TwoDVector&>(tFunctionAndWeight, tCommunicator);

    EXPECT_EQ(tAggregate.size(), 1);

    const auto tArg = pft::TwoDVector{2.0, -1.0};
    const double tExpectedF = tW * (tFNonDefault.f(tArg) + tFDefault.f(tArg));
    EXPECT_EQ(tAggregate.f(tArg), tExpectedF);
    const pft::TwoDVector tExpectedDF = tW * (tFNonDefault.df(tArg) + tFDefault.df(tArg));
    EXPECT_EQ(tAggregate.df(tArg), tExpectedDF);
}
