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

}  // namespace plato::utilities::unittest