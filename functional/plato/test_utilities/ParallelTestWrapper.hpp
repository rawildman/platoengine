#ifndef PLATO_TEST_UTILITIES_PARALLELTESTWRAPPER
#define PLATO_TEST_UTILITIES_PARALLELTESTWRAPPER

#include <boost/mpi.hpp>
#include <boost/mpi/communicator.hpp>

#include "plato/core/Function.hpp"

namespace plato::test_utilities
{
/// @brief The purpose of this class is to facilitate parallel testing by wrapping an
///  existing serial objective and adding some useless parallelism.
///
/// Specifically, it is constructed with an existing Function and a communicator, and
/// for both f and its derivative, only rank zero computes the result, which is communicated
/// to the other ranks.
template <typename R, typename dR, typename Arg>
class ParallelTestWrapper
{
   public:
    ParallelTestWrapper(core::Function<R, dR, Arg> aFunction);
    R f(const Arg& aArg, boost::mpi::communicator aComm) const;
    dR df(const Arg& aArg, boost::mpi::communicator aComm) const;

   private:
    core::Function<R, dR, Arg> mFunction;
};

namespace detail
{
double rank_weight(const boost::mpi::communicator aComm) { return aComm.rank() == 0 ? 1.0 : 0.0; }
}  // namespace detail

template <typename R, typename dR, typename Arg>
ParallelTestWrapper<R, dR, Arg>::ParallelTestWrapper(core::Function<R, dR, Arg> aFunction)
    : mFunction{std::move(aFunction)}
{
}

template <typename R, typename dR, typename Arg>
R ParallelTestWrapper<R, dR, Arg>::f(const Arg& aArg, boost::mpi::communicator aComm) const
{
    auto tResult = mFunction.f(aArg) * detail::rank_weight(aComm);
    boost::mpi::all_reduce(aComm, boost::mpi::inplace(tResult), std::plus<R>());
    return tResult;
}

template <typename R, typename dR, typename Arg>
dR ParallelTestWrapper<R, dR, Arg>::df(const Arg& aArg, boost::mpi::communicator aComm) const
{
    auto tResult = detail::rank_weight(aComm) * mFunction.df(aArg);
    boost::mpi::all_reduce(aComm, boost::mpi::inplace(tResult), std::plus<dR>());
    return tResult;
}
}  // namespace plato::test_utilities

#endif