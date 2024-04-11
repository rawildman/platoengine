#ifndef PLATO_CORE_PARALLELFUNCTION
#define PLATO_CORE_PARALLELFUNCTION

#include <boost/mpi/communicator.hpp>

#include "plato/core/Function.hpp"

namespace plato::core
{
/// @brief Wraps a function and its derivative that require a communicator in a Function.
///
/// The purpose of this function is to create a Function that runs in parallel with a specific
/// communicator. The signatures of @a aFun and @a aDFun should have two arguments, the first
/// the argument to the function as in Function, and the second a communicator.
template <typename F, typename DF>
auto make_parallel_function(F aFun, DF aDFun, const boost::mpi::communicator aComm);

namespace detail
{
template <typename F, typename R, typename Arg>
struct ArgType<R (F::*)(Arg, boost::mpi::communicator)>
{
    using type = Arg;
};

template <typename F, typename R, typename Arg>
struct ArgType<R (F::*)(Arg, boost::mpi::communicator) const>
{
    using type = Arg;
};
}  // namespace detail

template <typename F, typename DF>
auto make_parallel_function(F aFun, DF aDFun, const boost::mpi::communicator aComm)
{
    using ArgF = typename detail::ArgType<decltype(&F::operator())>::type;
    using ArgDF = typename detail::ArgType<decltype(&DF::operator())>::type;
    return make_function([tComm = aComm, tFun = std::move(aFun)](ArgF aArg) { return tFun(aArg, tComm); },
                         [tComm = aComm, tDFun = std::move(aDFun)](ArgDF aArg) { return tDFun(aArg, tComm); });
}

}  // namespace plato::core

#endif
