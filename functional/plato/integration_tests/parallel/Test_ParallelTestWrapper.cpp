#include <gtest/gtest.h>

#include <boost/mpi/communicator.hpp>

#include "plato/integration_tests/utilities/DynamicVectorRosenbrockFunction.hpp"
#include "plato/integration_tests/utilities/ParallelTestWrapper.hpp"

namespace plato::integration_tests::parallel
{
namespace
{
constexpr auto kNumRanks = int{4};
}

TEST(ParallelTestWrapper, MPISize)
{
    auto tComm = boost::mpi::communicator{};
    EXPECT_EQ(tComm.size(), kNumRanks);
}

TEST(ParallelTestWrapper, MatchesSerial)
{
    const auto tRosenbrockFunction = utilities::make_rosenbrock_dynamic_vector_function(test_utilities::Rosenbrock{});
    const auto tParallelRosenbrock = utilities::ParallelTestWrapper{tRosenbrockFunction, boost::mpi::communicator{}};

    const auto tControl = linear_algebra::DynamicVector{1.0, -2.0};
    EXPECT_EQ(tParallelRosenbrock.f(tControl), tRosenbrockFunction.f(tControl));
    EXPECT_EQ(tParallelRosenbrock.df(tControl), tRosenbrockFunction.df(tControl));
}
}  // namespace plato::integration_tests::parallel
