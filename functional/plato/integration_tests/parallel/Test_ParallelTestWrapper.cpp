#include <gtest/gtest.h>

#include <boost/mpi/communicator.hpp>

#include "plato/integration_tests/utilities/DynamicVectorRosenbrockFunction.hpp"
#include "plato/test_utilities/ParallelTestWrapper.hpp"

namespace plato::integration_tests::parallel
{
TEST(ParallelTestWrapper, MatchesSerial)
{
    const auto tRosenbrockFunction = utilities::make_rosenbrock_dynamic_vector_function(test_utilities::Rosenbrock{});
    const auto tParallelRosenbrock = test_utilities::ParallelTestWrapper{tRosenbrockFunction};

    const auto tComm = boost::mpi::communicator{};
    EXPECT_GT(tComm.size(), 1);
    const auto tControl = linear_algebra::DynamicVector{1.0, -2.0};
    EXPECT_EQ(tParallelRosenbrock.f(tControl, boost::mpi::communicator{}), tRosenbrockFunction.f(tControl));
    EXPECT_EQ(tParallelRosenbrock.df(tControl, boost::mpi::communicator{}), tRosenbrockFunction.df(tControl));
}
}  // namespace plato::integration_tests::parallel
