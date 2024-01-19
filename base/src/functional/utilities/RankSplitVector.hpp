#ifndef PLATO_FUNCTIONAL_RANKDISTRIBUTEDVECTOR
#define PLATO_FUNCTIONAL_RANKDISTRIBUTEDVECTOR

#include <array>
#include <boost/mpi/communicator.hpp>
#include <type_traits>

namespace Plato::Functional::Utilities
{
/// @brief Splits the elements of @a aVector among available ranks on MPI_COMM_WORLD.
///
/// This attempts to distribute the elements as evenly as possible. If the number of elements is
/// evenly divisible by the number of ranks, then the first `N` elements are distributed to
/// rank 0, the next `N` to rank 1, and so on, where `N` is `aVector.size / num_ranks`.
///
/// If the number of elements is not divisible by the number of ranks, then the remainder are
/// distributed one at a time to each rank until all are exhausted, so that no rank will have
/// more than `ceil(aVector.size() / num_ranks)` elements.
///
/// @note No communication is performed, it is assumed that @a aVector is the same on all ranks.
template <typename T>
std::vector<T> rank_split_vector(const std::vector<T>& aVector, const boost::mpi::communicator& aComm);

namespace detail
{
/// @brief Divides @a aNumElements by @a aNumRanks and returns the quotient in the first element and
///  the remainder in the second.
template <typename T, typename U>
auto num_elements_per_rank(const T aNumElements, const U aNumRanks) -> std::array<std::common_type_t<T, U>, 2>
{
    static_assert(std::is_integral_v<T>, "aNumElements must have an integer type.");
    static_assert(std::is_integral_v<U>, "aNumRanks must have an integer type.");
    return {aNumElements / aNumRanks, aNumElements % aNumRanks};
}

template <typename T>
bool assign_remainder_element_to_rank(const boost::mpi::communicator& aComm, const T aRemainder)
{
    static_assert(std::is_integral_v<T>, "aRemainder must have an integer type.");
    return static_cast<T>(aComm.rank()) < aRemainder;
}
}  // namespace detail

template <typename T>
std::vector<T> rank_split_vector(const std::vector<T>& aVector, const boost::mpi::communicator& aComm)
{
    auto tDistributedVector = std::vector<T>{};
    const auto [tNumElementsPerRank, tRemainder] = detail::num_elements_per_rank(aVector.size(), aComm.size());
    const auto tFirstIndex = aComm.rank() * tNumElementsPerRank;
    const auto tLastIndex = (aComm.rank() + 1) * tNumElementsPerRank;
    tDistributedVector.reserve(tNumElementsPerRank);
    std::copy(std::next(aVector.cbegin(), tFirstIndex), std::next(aVector.cbegin(), tLastIndex),
              std::back_inserter(tDistributedVector));
    if (detail::assign_remainder_element_to_rank(aComm, tRemainder))
    {
        const int tNumDistributed = tNumElementsPerRank * aComm.size();
        const int tRemainderForRankIndex = tNumDistributed + aComm.rank();
        tDistributedVector.push_back(aVector.at(tRemainderForRankIndex));
    }
    return tDistributedVector;
}

}  // namespace Plato::Functional::Utilities

#endif
