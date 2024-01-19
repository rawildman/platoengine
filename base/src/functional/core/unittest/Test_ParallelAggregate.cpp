#include <gtest/gtest.h>
#include <mpi.h>

#include "ParallelAggregate.hpp"
#include "Rosenbrock.hpp"
#include "TwoDTestTypesSerialization.hpp"
#include "Utilities.hpp"

namespace
{
constexpr auto kNumRanks = int{2};
}  // namespace

class MPICommSpawnFixture : public ::testing::Test
{
   protected:
    void SetUp() override
    {
    }
    bool shouldRun() const { return mParentComm != MPI_COMM_NULL; }

   protected:
    MPI_Comm mParentComm;
};

TEST(Aggregate, MPISize)
{
    int tMPISize = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &tMPISize);
    EXPECT_EQ(tMPISize, kNumRanks);
}

TEST_F(MPICommSpawnFixture, ParallelAggregate)
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
        FunctionAndWeight{std::make_pair(tF, tW)});

    EXPECT_EQ(tAggregate.size(), 1);

    const auto tArg = pft::TwoDVector{2.0, -1.0};
    const double tExpectedF = kNumRanks * tW * tF.f(tArg);
    EXPECT_EQ(tAggregate.f(tArg), tExpectedF);
    const pft::TwoDVector tExpectedDF = kNumRanks * tW * tF.df(tArg);
    EXPECT_EQ(tAggregate.df(tArg), tExpectedDF);
}
