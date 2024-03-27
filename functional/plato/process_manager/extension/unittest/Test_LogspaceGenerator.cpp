#include <gtest/gtest.h>

#include "plato/process_manager/extension/LogspaceGenerator.hpp"

namespace plato::process_manager::extension::unittest
{
TEST(LogspaceGenerator, CreateLogSpace)
{
    const LogspaceGenerator tGenerator{16, 0.5, 5};
    const std::vector<double> tResult = tGenerator.steps();
    const std::vector<double> tGold = {16, 8, 4, 2, 1};
    EXPECT_EQ(tResult, tGold);
}

TEST(LogspaceGenerator, ZeroSteps)
{
    const LogspaceGenerator tGenerator{16, 0.5, 0};
    EXPECT_TRUE(tGenerator.steps().empty());
}

TEST(LogspaceGenerator, OneStep)
{
    const LogspaceGenerator tGenerator{16, 0.5, 1};
    const std::vector<double> tGold = {16};
    EXPECT_EQ(tGenerator.steps(), tGold);
}

}  // namespace plato::process_manager::extension::unittest
