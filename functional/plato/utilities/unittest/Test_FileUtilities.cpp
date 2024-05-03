#include <gtest/gtest.h>

#include "plato/test_utilities/TestContext.hpp"
#include "plato/utilities/FileUtilities.hpp"

namespace plato::utilities::unittest
{
void test_file_name_and_path(const std::filesystem::path& aTestPath,
                             const std::filesystem::path& aNewPath,
                             const test_utilities::TestContext& aTestContext)
{
    EXPECT_NE(aTestPath, aNewPath) << aTestContext;
    // The parent path should be unchanged, only the filename itself should change
    EXPECT_EQ(aTestPath.parent_path(), aNewPath.parent_path()) << aTestContext;
    EXPECT_NE(aTestPath.filename(), aNewPath.filename()) << aTestContext;
}

TEST(FileUtilities, UniqueFilenameFullPath)
{
    const auto aTestPath = std::filesystem::path{"some/path/file.txt"};
    const auto aNewPath = make_filename_unique(aTestPath);
    test_file_name_and_path(aTestPath, aNewPath, TEST_CONTEXT("Filename with extension"));
}

TEST(FileUtilities, UniqueFilenameNoFilename)
{
    const auto aTestPath = std::filesystem::path{"some/path/"};
    const auto aNewPath = make_filename_unique(aTestPath);
    EXPECT_EQ(aTestPath, aNewPath);
    EXPECT_FALSE(aNewPath.has_filename());
}

TEST(FileUtilities, UniqueFilenameNoExtension)
{
    const auto aTestPath = std::filesystem::path{"some/path/file"};
    const auto aNewPath = make_filename_unique(aTestPath);
    test_file_name_and_path(aTestPath, aNewPath, TEST_CONTEXT("Filename with no extension"));
}

}  // namespace plato::utilities::unittest
