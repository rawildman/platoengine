#include <gtest/gtest.h>

#include "plato/process_manager/extension/LinspaceGenerator.hpp"

namespace plato::process_manager::extension::unittest
{

TEST(LinspaceGenerator, CreateLinearSpace)
{
    LinspaceGenerator tGenerator{16, 0.5, 5};
    const std::vector<double> tResult = tGenerator.steps();
    const std::vector<double> tGold = {16, 8, 4, 2, 1};
    EXPECT_EQ(tResult, tGold);
}

}  // namespace plato::process_manager::extension::unittest
