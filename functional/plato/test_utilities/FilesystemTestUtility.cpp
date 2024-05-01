#include "plato/test_utilities/FilesystemTestUtility.hpp"

#include <gtest/gtest.h>

#include <filesystem>

namespace plato::test_utilities
{

void test_for_existence_and_remove(const std::vector<std::filesystem::path>& aFilesToCheck)
{
    for (const auto& tFileName : aFilesToCheck)
    {
        EXPECT_TRUE(std::filesystem::exists(tFileName));
        EXPECT_TRUE(std::filesystem::remove(tFileName));
    }
}
}  // namespace plato::test_utilities
