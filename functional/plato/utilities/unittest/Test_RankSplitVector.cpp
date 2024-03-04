#include <gtest/gtest.h>

#include "plato/utilities/RankSplitVector.hpp"

namespace plato::utilities::unittest
{
namespace
{
constexpr auto kMPISize = int{3};

void split_vector_and_check(const std::vector<int>& aVector, int aRankNamedTypeNumber, const std::vector<int>& tGold)
{
    const std::vector<int> tDistributedValues =
        rank_split_vector(aVector, RankNamedType{aRankNamedTypeNumber}, SizeNamedType{kMPISize});
    EXPECT_EQ(tDistributedValues, tGold);
}

}  // namespace

TEST(RankSplitVector, NumElementsPerRank)
{
    {
        const auto [tQuotient, tRemainder] = detail::num_elements_per_rank(10, 2);
        EXPECT_EQ(tQuotient, 5);
        EXPECT_EQ(tRemainder, 0);
    }
    {
        const auto [tQuotient, tRemainder] = detail::num_elements_per_rank(10, 3);
        EXPECT_EQ(tQuotient, 3);
        EXPECT_EQ(tRemainder, 1);
    }
}

TEST(RankSplitVector, DividesEvenly)
{
    const auto tValues = std::vector<int>{1, 2, 3, 4, 5, 6};
    split_vector_and_check(tValues, 0, {1, 2});
    split_vector_and_check(tValues, 1, {3, 4});
    split_vector_and_check(tValues, 2, {5, 6});
}

TEST(RankSplitVector, DividesUnevenlyOneRemaining)
{
    const auto tValues = std::vector<int>{1, 2, 3, 4, 5, 6, 7};
    split_vector_and_check(tValues, 0, {1, 2, 7});
    split_vector_and_check(tValues, 1, {3, 4});
    split_vector_and_check(tValues, 2, {5, 6});
}

TEST(RankSplitVector, DividesUnevenlyTwoRemaining)
{
    const auto tValues = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8};
    split_vector_and_check(tValues, 0, {1, 2, 7});
    split_vector_and_check(tValues, 1, {3, 4, 8});
    split_vector_and_check(tValues, 2, {5, 6});
}

TEST(RankSplitVector, MoreRanksThanElements)
{
    const auto tValues = std::vector<int>{1, 2};
    split_vector_and_check(tValues, 0, {1});
    split_vector_and_check(tValues, 1, {2});
    split_vector_and_check(tValues, 2, {});
}
}  // namespace plato::utilities::unittest