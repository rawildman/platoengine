#include <gtest/gtest.h>

#include <list>
#include <numeric>
#include <set>

#include "plato/utilities/Zip.hpp"
#include "plato/utilities/ZipIterator.hpp"

namespace plato::utilities::unittest
{
namespace
{
struct CopyCounter
{
    unsigned int count = 0;
    CopyCounter() {}
    CopyCounter(const CopyCounter& copy) : count{copy.count} { ++count; }
};
}  // namespace

TEST(Zip, AnyOf)
{
    constexpr auto tuple_1 = std::make_tuple(1, 'a');
    constexpr auto tuple_2 = std::make_tuple(2, 'a');
    constexpr auto tuple_3 = std::make_tuple(1, 'b');
    constexpr auto tuple_4 = std::make_tuple(2, 'b');

    EXPECT_TRUE(any_of_comparison(tuple_1, tuple_1, std::equal_to{}));
    EXPECT_TRUE(any_of_comparison(tuple_1, tuple_2, std::equal_to{}));
    EXPECT_TRUE(any_of_comparison(tuple_1, tuple_3, std::equal_to{}));
    EXPECT_FALSE(any_of_comparison(tuple_1, tuple_4, std::equal_to{}));
}

TEST(Zip, IteratorsEqual)
{
    const auto vector_1 = std::vector<int>{1, 2, 3};
    const auto vector_2 = std::vector<char>{'a', 'b', 'c'};
    const auto iter1 = ZipIterator{vector_1.cbegin(), vector_2.cbegin()};
    const auto iter2 = ZipIterator{std::make_tuple(vector_1.cend(), vector_2.cend())};

    EXPECT_EQ(iter1, iter1);
    EXPECT_NE(iter1, iter2);
}

TEST(Zip, ExplicitIteratorType)
{
    const auto vector_1 = std::vector<int>{1, 2, 3};
    auto vector_2 = std::vector<char>{'a', 'b', 'c'};
    using ZipIteratorType = ZipIterator<std::vector<int>::const_iterator, std::vector<char>::iterator>;
    const auto iter = ZipIteratorType{vector_1.begin(), vector_2.begin()};
    EXPECT_EQ(iter, iter);
}

TEST(Zip, DereferenceIterator)
{
    const auto vector_1 = std::vector<int>{1, 2, 3};
    const auto vector_2 = std::vector<char>{'a', 'b', 'c'};

    {
        const auto iter = ZipIterator{vector_1.cbegin(), vector_2.cbegin()};
        EXPECT_EQ(std::get<0>(*iter), 1);
        EXPECT_EQ(std::get<1>(*iter), 'a');
    }

    {
        const auto iter = ZipIterator{std::next(vector_1.cbegin()), std::next(vector_2.cbegin())};
        EXPECT_EQ(std::get<0>(*iter), 2);
        EXPECT_EQ(std::get<1>(*iter), 'b');
    }

    {
        const auto iter = ZipIterator{vector_1.crbegin(), vector_2.crbegin()};
        EXPECT_EQ(std::get<0>(*iter), 3);
        EXPECT_EQ(std::get<1>(*iter), 'c');
    }

    {
        const auto iter = ZipIterator{vector_1.cbegin(), vector_2.crbegin()};
        EXPECT_EQ(std::get<0>(*iter), 1);
        EXPECT_EQ(std::get<1>(*iter), 'c');
    }
}

TEST(Zip, DereferencingIteratorDoesNotCopy)
{
    auto vector_1 = std::vector<CopyCounter>{};
    vector_1.reserve(2);
    vector_1.emplace_back();
    vector_1.emplace_back();
    EXPECT_EQ(vector_1.front().count, 0);
    {
        const auto iter = vector_1.cbegin();
        EXPECT_EQ(iter->count, 0);
    }
    {
        const auto zip_iterator = ZipIterator{vector_1.cbegin()};
        const auto& value = *zip_iterator;
        EXPECT_EQ(std::get<0>(value).count, 0);
    }

    // Copies these on construction, so expect 1 copy
    auto vector_2 = std::vector<CopyCounter>(3, CopyCounter{});
    {
        auto zip_iterator = ZipIterator{vector_1.rbegin(), vector_2.begin()};
        auto values = *zip_iterator;
        EXPECT_EQ(std::get<0>(values).count, 0);
        EXPECT_EQ(std::get<1>(values).count, 1);
    }
    {
        auto zip_iterator = ZipIterator{vector_1.rbegin(), vector_2.begin()};
        auto [value_1, value_2] = *zip_iterator;
        EXPECT_EQ(value_1.count, 0);
        EXPECT_EQ(value_2.count, 1);
    }
}

TEST(Zip, ModifyThroughIterator)
{
    auto vector_1 = std::vector<int>{1, 2, 3};
    auto vector_2 = std::vector<char>{'a', 'b', 'c'};
    auto zip_iterator = ZipIterator{vector_1.begin(), vector_2.rbegin()};

    auto [value_1, value_2] = *zip_iterator;
    value_1 = 42;
    EXPECT_EQ(vector_1.front(), 42);
    value_2 = 'z';
    EXPECT_EQ(vector_2.back(), 'z');
}

TEST(Zip, Increment)
{
    const auto vector_1 = std::vector<int>{1, 2, 3};
    const auto vector_2 = std::vector<char>{'a', 'b', 'c'};
    auto zip_iterator = ZipIterator{vector_1.cbegin(), vector_2.crbegin()};

    {
        const auto [value_1, value_2] = *zip_iterator;
        EXPECT_EQ(value_1, 1);
        EXPECT_EQ(value_2, 'c');
    }
    ++zip_iterator;
    {
        const auto [value_1, value_2] = *zip_iterator;
        EXPECT_EQ(value_1, 2);
        EXPECT_EQ(value_2, 'b');
    }
    ++zip_iterator;
    {
        const auto [value_1, value_2] = *zip_iterator;
        EXPECT_EQ(value_1, 3);
        EXPECT_EQ(value_2, 'a');
    }
}

TEST(Zip, StdAccumulate)
{
    const auto vector_1 = std::vector<int>{1, 2, 3};
    const auto vector_2 = std::vector<int>{10, 11, 12, 13};
    auto zip_begin = ZipIterator{vector_1.cbegin(), vector_2.crbegin()};
    auto zip_end = ZipIterator{vector_1.cend(), vector_2.crend()};
    const auto result = std::accumulate(zip_begin, zip_end, 0,
                                        [](const int sum, const auto& values)
                                        { return std::get<0>(values) + std::get<1>(values) + sum; });
    EXPECT_EQ(result, 42);
}

TEST(Zip, ZipTypes)
{
    using Vector1 = std::vector<int>;
    using Vector2 = std::vector<char>;
    using TestZip = Zip<Vector1, Vector2>;

    auto vector_1 = Vector1{1, 2, 3};
    auto vector_2 = Vector2{'a', 'b', 'c'};
    // Both lvalues for arguments
    {
        auto test_zip = TestZip{vector_1, vector_2};
        auto begin_iter = test_zip.begin();
        using BeginIter = decltype(begin_iter);
        EXPECT_TRUE((std::is_same_v<BeginIter, ZipIterator<Vector1::iterator, Vector2::iterator> >));

        EXPECT_EQ(std::get<0>(*begin_iter), 1);
        EXPECT_EQ(std::get<1>(*begin_iter), 'a');
    }
    // Temporary as one argument
    {
        auto test_zip = TestZip{std::vector<int>{1, 2, 3}, vector_2};
        auto begin_iter = test_zip.begin();
        using BeginIter = decltype(begin_iter);
        EXPECT_TRUE((std::is_same_v<BeginIter, ZipIterator<Vector1::iterator, Vector2::iterator> >));

        EXPECT_EQ(std::get<0>(*begin_iter), 1);
        EXPECT_EQ(std::get<1>(*begin_iter), 'a');
    }
    // Deduced
    {
        const auto vector_3 = Vector1{1, 2, 3};
        auto test_zip = Zip{vector_1, vector_3};
        auto begin_iter = test_zip.begin();
        using BeginIter = decltype(begin_iter);
        EXPECT_TRUE((std::is_same_v<BeginIter, ZipIterator<Vector1::iterator, Vector1::const_iterator> >));
    }
}

TEST(Zip, ZipBegin)
{
    auto vector_1 = std::vector<int>{1, 2, 3};
    auto vector_2 = std::vector<char>{'a', 'b', 'c'};
    auto zip = Zip{vector_1, vector_2};
    auto begin_iter = zip.begin();
    EXPECT_EQ(std::get<0>(*begin_iter), 1);
    EXPECT_EQ(std::get<1>(*begin_iter), 'a');
}

TEST(Zip, ZipBeginTemporary)
{
    const auto alphabet = std::string{"abcdefghijklmnopqrstuvwxyz"};
    const auto digits = std::string{"1234567890"};
    auto vector_1 = std::vector<int>{1, 2};
    auto zip = Zip{vector_1, std::vector<std::string>{alphabet, digits}};
    auto begin_iter = zip.begin();
    EXPECT_EQ(std::get<0>(*begin_iter), 1);
    EXPECT_EQ(std::get<1>(*begin_iter), alphabet);
    ++begin_iter;
    EXPECT_EQ(std::get<0>(*begin_iter), 2);
    EXPECT_EQ(std::get<1>(*begin_iter), digits);
}

TEST(Zip, ZipBeginConst)
{
    const auto vector_1 = std::vector<int>{1, 2, 3};
    const auto vector_2 = std::vector<char>{'a', 'b', 'c'};
    const auto zip = Zip{vector_1, vector_2};
    auto begin_iter = zip.begin();
    EXPECT_EQ(std::get<0>(*begin_iter), 1);
    EXPECT_EQ(std::get<1>(*begin_iter), 'a');
}

TEST(Zip, ZipEnd)
{
    const auto vector_1 = std::vector<int>{1, 2, 3};
    const auto vector_2 = std::vector<char>{'a', 'b', 'c'};
    const auto zip = Zip{vector_1, vector_2};
    auto end_iter = zip.end();
    EXPECT_EQ(end_iter, (ZipIterator{vector_1.end(), vector_2.end()}));
}

TEST(Zip, VectorsSameLengthConst)
{
    const auto double_vector = std::vector<double>{1.0, 2.0, 3.0};
    const auto char_vector = std::vector<char>{'a', 'b', 'c'};

    auto result_double_vector = std::vector<double>{};
    auto result_char_vector = std::vector<char>{};
    for (const auto [value_1, value_2] : Zip{double_vector, char_vector})
    {
        result_double_vector.push_back(value_1);
        result_char_vector.push_back(value_2);
    }
    EXPECT_EQ(double_vector, result_double_vector);
    EXPECT_EQ(char_vector, result_char_vector);
}

TEST(Zip, VectorsSameLengthModifying)
{
    auto double_vector = std::vector<double>{1.0, 2.0, 3.0};
    auto int_vector = std::vector<int>{4, 5, 6};
    auto count = int{100};
    for (auto [value_1, value_2] : Zip{double_vector, int_vector})
    {
        value_1 = count;
        value_2 = 2 * count;
        ++count;
    }
    const auto expected_double_vector = std::vector<double>{100.0, 101.0, 102.0};
    const auto expected_int_vector = std::vector<int>{200, 202, 204};
    EXPECT_EQ(double_vector, expected_double_vector);
    EXPECT_EQ(int_vector, expected_int_vector);
}

TEST(Zip, VectorsDifferentLengthTemporary)
{
    auto double_vector = std::vector<double>{1.0, 2.0, 3.0};
    auto result_double_vector = std::vector<double>{};
    auto result_int_vector = std::vector<int>{};
    for (const auto [value_1, value_2] : Zip{double_vector, std::vector<int>{4, 5, 6, 7}})
    {
        result_double_vector.push_back(value_1);
        result_int_vector.push_back(value_2);
    }
    EXPECT_EQ(double_vector, result_double_vector);
    EXPECT_EQ((std::vector<int>{4, 5, 6}), result_int_vector);
}

TEST(Zip, ListAndSet)
{
    const auto int_set = std::set<int>{1, 2, 3, 4, 4, 4};
    const auto char_list = std::list<char>{'a', 'b', 'c', 'd', 'e'};
    auto result_ints = std::vector<int>{};
    auto result_chars = std::vector<char>{};
    for (const auto [int_value, char_value] : Zip{int_set, char_list})
    {
        result_ints.push_back(int_value);
        result_chars.push_back(char_value);
    }
    const auto expected_ints = std::vector<int>{1, 2, 3, 4};
    const auto expected_chars = std::vector<char>{'a', 'b', 'c', 'd'};
    EXPECT_EQ(result_ints, expected_ints);
    EXPECT_EQ(result_chars, expected_chars);
}

TEST(Zip, NoCopiesForLValues)
{
    auto vector_1 = std::vector<CopyCounter>{};
    vector_1.reserve(2);
    vector_1.emplace_back();
    vector_1.emplace_back();
    auto copy_count = int{0};
    for (const auto [value_1] : Zip{vector_1})
    {
        copy_count += value_1.count;
    }
    EXPECT_EQ(copy_count, 0);
}

TEST(Zip, CopiesForRValues)
{
    auto copy_count = int{0};
    for (const auto [value_1] : Zip{std::vector<CopyCounter>(3)})
    {
        copy_count += value_1.count;
    }
    EXPECT_EQ(copy_count, 3);
}

}  // namespace plato::utilities::unittest
