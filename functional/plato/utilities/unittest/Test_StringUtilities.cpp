#include <gtest/gtest.h>

#include "plato/utilities/StringUtilities.hpp"

namespace plato::utilities::unittest
{
TEST(Utilities, ConcatenateVector)
{
    const auto tStrings = std::vector<std::string>{"thing", "one", "thing", "two"};
    const auto tResult = concatenate_vector(tStrings);
    EXPECT_EQ(tResult, "thing one thing two");
}

TEST(Utilities, ConcatenateVectorCustomDelimiter)
{
    const auto tStrings = std::vector<std::string>{"a", "b", "c"};
    const auto tResult = concatenate_vector(tStrings, ", ");
    EXPECT_EQ(tResult, "a, b, c");
}
TEST(Utilities, OneElementVector)
{
    const auto tStrings = std::vector<std::string>{"onestring"};
    const auto tResult = concatenate_vector(tStrings);
    EXPECT_EQ(tResult, tStrings.front());
}

TEST(Utilities, EmptyVector)
{
    const auto tStrings = std::vector<std::string>{};
    const auto tResult = concatenate_vector(tStrings);
    EXPECT_TRUE(tResult.empty());
}

TEST(Utilities, ConcatenateStringLValues)
{
    const auto str1 = std::string{"one"};
    const auto str2 = std::string{"fish"};
    const auto result = concatenate(str1, str2);
    EXPECT_EQ(result, "onefish");
}

TEST(Utilities, ConcatenateStringRValues)
{
    auto str1 = std::string{"two"};
    const auto result = concatenate(std::move(str1), std::string{"fish"});
    EXPECT_EQ(result, "twofish");
}

TEST(Utilities, ConcatenateStringCharStar)
{
    const auto str1 = "red";
    const auto result = concatenate(str1, "fish");
    EXPECT_EQ(result, "redfish");
}

TEST(Utilities, ConcatenateStringView)
{
    const auto str1 = std::string_view{"blue"};
    const auto result = concatenate(str1, std::string_view{"fish"});
    EXPECT_EQ(result, "bluefish");
}

TEST(Utilities, ConcatenateStringsAndNumbers)
{
    const auto one = int{1};
    const auto fish = "fish";
    const auto two = unsigned{2};
    const auto result = concatenate(one, fish, two, fish);
    EXPECT_EQ(result, "1fish2fish");
}

TEST(Utilities, ConcatenateStringsChar)
{
    const auto f = char{'f'};
    const auto i = char{'i'};
    const auto result = concatenate(f, i, 's', 'h');
    EXPECT_EQ(result, "fish");
}

}  // namespace plato::utilities::unittest