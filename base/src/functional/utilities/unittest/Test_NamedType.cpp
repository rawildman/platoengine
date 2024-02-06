#include <gtest/gtest.h>

#include "NamedType.hpp"

namespace
{
struct Input
{
    double mValue;
    std::string mName;
};
using ValidatedInput = plato::functional::utilities::NamedType<Input, struct ValidatedInputTag>;
}  // namespace

TEST(NamedType, Creation)
{
    Input tInput{3.0, "invalid name"};
    ValidatedInput tValid{tInput};
    EXPECT_EQ(tValid.mValue.mValue, tInput.mValue);
    EXPECT_EQ(tValid.mValue.mName, tInput.mName);
}
