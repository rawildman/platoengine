#include <gtest/gtest.h>

#include <boost/mpi/communicator.hpp>

#include "plato/core/Function.hpp"
#include "plato/core/ParallelFunction.hpp"
#include "plato/core/test_utilities/Utilities.hpp"
#include "plato/test_utilities/ParallelTestWrapper.hpp"
#include "plato/test_utilities/Rosenbrock.hpp"
#include "plato/test_utilities/TwoDTestTypes.hpp"
#include "plato/test_utilities/TwoDTestTypesSerialization.hpp"

namespace plato::core::parallel_unittest
{
namespace
{
using TestVectorType = std::array<double, 2>;
}

TEST(ParallelFunction, Rosenbrock)
{
    namespace ptu = plato::test_utilities;

    const auto tA = double{1.0};
    const auto tB = double{10.0};
    const auto tRosenbrock = ptu::Rosenbrock{tA, tB};
    // Serial
    const auto tF = [tRosenbrock](const ptu::TwoDVector& aVector)
    { return tRosenbrock.f(aVector.mData[0], aVector.mData[1]); };
    const auto tDF = [tRosenbrock](const ptu::TwoDVector& aVector)
    { return tRosenbrock.df(aVector.mData[0], aVector.mData[1]); };
    const auto tSerialFunction = make_function(tF, tDF);

    // Parallel
    const auto tComm = boost::mpi::communicator{};
    const auto tParallelFunction =
        make_parallel_function(ptu::ParallelTestFunctionWrapper<double, const ptu::TwoDVector&>{tF},
                               ptu::ParallelTestFunctionWrapper<ptu::TwoDVector, const ptu::TwoDVector&>{tDF}, tComm);

    EXPECT_GT(tComm.size(), 1);

    const auto tControl = ptu::TwoDVector{1.0, -2.0};
    EXPECT_EQ(tSerialFunction.f(tControl), tParallelFunction.f(tControl));
    EXPECT_EQ(tSerialFunction.df(tControl), tParallelFunction.df(tControl));
}
}  // namespace plato::core::parallel_unittest
