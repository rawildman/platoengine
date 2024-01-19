#include <gtest/gtest.h>

#include <boost/mpi/communicator.hpp>

#include "RankSplitVector.hpp"

namespace
{
constexpr auto kExpectedMPISize = int{3};
}  // namespace

TEST(RankSplitVector, NumElementsPerRank)
{
    namespace pfu = Plato::Functional::Utilities;
    {
        const auto [tQuotient, tRemainder] = pfu::detail::num_elements_per_rank(10, 2);
        EXPECT_EQ(tQuotient, 5);
        EXPECT_EQ(tRemainder, 0);
    }  // namespace Plato::Functional::Utilities;
    {
        const auto [tQuotient, tRemainder] = pfu::detail::num_elements_per_rank(10, 3);
        EXPECT_EQ(tQuotient, 3);
        EXPECT_EQ(tRemainder, 1);
    }
}

TEST(RankSplitVector, DividesEvenly)
{
    namespace pf = Plato::Functional;
    auto tCommWorld = boost::mpi::communicator{};
    ASSERT_EQ(tCommWorld.size(), kExpectedMPISize);

    const auto tValues = std::vector<int>{1, 2, 3, 4, 5, 6};
    const std::vector<int> tDistributedValues = pf::Utilities::rank_split_vector(tValues, boost::mpi::communicator{});
    if (tCommWorld.rank() == 0)
    {
        EXPECT_EQ(tDistributedValues, (std::vector<int>{1, 2}));
    }
    else if (tCommWorld.rank() == 1)
    {
        EXPECT_EQ(tDistributedValues, (std::vector<int>{3, 4}));
    }
    else
    {
        EXPECT_EQ(tDistributedValues, (std::vector<int>{5, 6}));
    }
}

TEST(RankSplitVector, DividesUnevenlyOneRemaining)
{
    namespace pf = Plato::Functional;
    auto tCommWorld = boost::mpi::communicator{};
    ASSERT_EQ(tCommWorld.size(), kExpectedMPISize);

    const auto tValues = std::vector<int>{1, 2, 3, 4, 5, 6, 7};
    const std::vector<int> tDistributedValues = pf::Utilities::rank_split_vector(tValues, boost::mpi::communicator{});
    if (tCommWorld.rank() == 0)
    {
        EXPECT_EQ(tDistributedValues, (std::vector<int>{1, 2, 7}));
    }
    else if (tCommWorld.rank() == 1)
    {
        EXPECT_EQ(tDistributedValues, (std::vector<int>{3, 4}));
    }
    else
    {
        EXPECT_EQ(tDistributedValues, (std::vector<int>{5, 6}));
    }
}

TEST(RankSplitVector, DividesUnevenlyTwoRemaining)
{
    namespace pf = Plato::Functional;
    auto tCommWorld = boost::mpi::communicator{};
    ASSERT_EQ(tCommWorld.size(), kExpectedMPISize);

    const auto tValues = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8};
    const std::vector<int> tDistributedValues = pf::Utilities::rank_split_vector(tValues, boost::mpi::communicator{});
    if (tCommWorld.rank() == 0)
    {
        EXPECT_EQ(tDistributedValues, (std::vector<int>{1, 2, 7}));
    }
    else if (tCommWorld.rank() == 1)
    {
        EXPECT_EQ(tDistributedValues, (std::vector<int>{3, 4, 8}));
    }
    else
    {
        EXPECT_EQ(tDistributedValues, (std::vector<int>{5, 6}));
    }
}

TEST(RankSplitVector, MoreRanksThanElements)
{
    namespace pf = Plato::Functional;
    auto tCommWorld = boost::mpi::communicator{};
    ASSERT_EQ(tCommWorld.size(), kExpectedMPISize);

    const auto tValues = std::vector<int>{1, 2};
    const std::vector<int> tDistributedValues = pf::Utilities::rank_split_vector(tValues, boost::mpi::communicator{});
    if (tCommWorld.rank() == 0)
    {
        EXPECT_EQ(tDistributedValues, (std::vector<int>{1}));
    }
    else if (tCommWorld.rank() == 1)
    {
        EXPECT_EQ(tDistributedValues, (std::vector<int>{2}));
    }
    else
    {
        EXPECT_TRUE(tDistributedValues.empty());
    }
}
