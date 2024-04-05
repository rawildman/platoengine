#ifndef PLATO_INTEGRATION_TESTS_UTILITIES_PARALLELTESTWRAPPER
#define PLATO_INTEGRATION_TESTS_UTILITIES_PARALLELTESTWRAPPER

#include <boost/mpi.hpp>
#include <boost/mpi/communicator.hpp>

#include "plato/core/Function.hpp"

namespace plato::integration_tests::utilities
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
    ParallelTestWrapper(core::Function<R, dR, Arg> aFunction, boost::mpi::communicator aComm);
    R f(const Arg& aArg) const;
    dR df(const Arg& aArg) const;

   private:
    core::Function<R, dR, Arg> mFunction;
    boost::mpi::communicator mComm;
};

namespace detail
{
double rank_weight(const boost::mpi::communicator aComm) { return aComm.rank() == 0 ? 1.0 : 0.0; }
}  // namespace detail

template <typename R, typename dR, typename Arg>
ParallelTestWrapper<R, dR, Arg>::ParallelTestWrapper(core::Function<R, dR, Arg> aFunction,
                                                     boost::mpi::communicator aComm)
    : mFunction{std::move(aFunction)}, mComm(std::move(aComm))
{
}

template <typename R, typename dR, typename Arg>
R ParallelTestWrapper<R, dR, Arg>::f(const Arg& aArg) const
{
    auto tResult = mFunction.f(aArg) * detail::rank_weight(mComm);
    boost::mpi::all_reduce(mComm, boost::mpi::inplace(tResult), std::plus<R>());
    return tResult;
}

template <typename R, typename dR, typename Arg>
dR ParallelTestWrapper<R, dR, Arg>::df(const Arg& aArg) const
{
    auto tResult = mFunction.df(aArg) * detail::rank_weight(mComm);
    boost::mpi::all_reduce(mComm, boost::mpi::inplace(tResult), std::plus<dR>());
    return tResult;
}
}  // namespace plato::integration_tests::utilities

#endif