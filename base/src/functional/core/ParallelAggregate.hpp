#ifndef PLATO_FUNCTIONAL_PARALLELAGGREGATE
#define PLATO_FUNCTIONAL_PARALLELAGGREGATE

#include <boost/mpi.hpp>

#include "Aggregate.hpp"
#include "Function.hpp"
#include "ROLHelpers.hpp"

namespace Plato::Functional
{
/// @brief Implements a weighted sum of Function objects computed via MPI parallelization.
///
/// Requires that the types @a R and @a dR implement addition, as well as multiplication with a scalar.
/// Further requires that types @a R and @a dR can be communicated using boost mpi. Built-in types are
/// handled with native MPI functions, but standard library types and user-defined types must be
/// serialized with boost serialization.
/// @sa make_aggregate_function
template <typename R, typename dR, typename Arg>
class ParallelAggregate
{
   public:
    using AggregateFunction = Function<R, dR, Arg>;

    explicit ParallelAggregate(std::vector<std::pair<AggregateFunction, double>> aFunctionsAndWeights);

    /// @brief Computes the weighted sum of functions owned by this object and
    ///  then performs a blocking all reduce operation on the result.
    [[nodiscard]] R f(const Arg& aArg) const;
    /// @brief Computes the weighted sum of the derivatives of the functions owned by
    ///  this object and then performs a blocking all reduce operation on the result.
    [[nodiscard]] dR df(const Arg& aArg) const;

    /// @return The number of Function objects used on construction.
    [[nodiscard]] std::size_t size() const;

   private:
    Aggregate<R, dR, Arg> mAggregateFunction;
};

template <typename R, typename dR, typename Arg>
ParallelAggregate<R, dR, Arg>::ParallelAggregate(std::vector<std::pair<AggregateFunction, double>> aFunctionsAndWeights)
    : mAggregateFunction{std::move(aFunctionsAndWeights)}
{
}

template <typename R, typename dR, typename Arg>
R ParallelAggregate<R, dR, Arg>::f(const Arg& aArg) const
{
    const auto tCommWorld = boost::mpi::communicator{};
    R tResult = mAggregateFunction.f(aArg);
    boost::mpi::all_reduce(tCommWorld, boost::mpi::inplace(tResult), std::plus<R>());
    return tResult;
}

template <typename R, typename dR, typename Arg>
dR ParallelAggregate<R, dR, Arg>::df(const Arg& aArg) const
{
    const auto tCommWorld = boost::mpi::communicator{};
    dR tResult = mAggregateFunction.df(aArg);
    boost::mpi::all_reduce(tCommWorld, boost::mpi::inplace(tResult), std::plus<dR>());
    return tResult;
}

template <typename R, typename dR, typename Arg>
std::size_t ParallelAggregate<R, dR, Arg>::size() const
{
    return mAggregateFunction.size();
}

}  // namespace Plato::Functional

#endif
