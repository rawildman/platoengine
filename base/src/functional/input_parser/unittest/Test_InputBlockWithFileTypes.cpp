#include <gtest/gtest.h>

#include "FileList.hpp"
#include "InputBlockStruct.hpp"
#include "Test_Helpers.hpp"

// clang-format off
PLATO_INPUT_BLOCK_STRUCT(
    (Plato), TestFileTypes,
    (Plato::FileName, file_name)
    (Plato::FileList, file_list)
)
// clang-format on

TEST(ParseFileTypes, FileName)
{
    Plato::TestFileTypes tTestBlock;
    tTestBlock.file_name = Plato::FileName{"wolverine"};
    Plato::Test::test_existence_and_equality(tTestBlock.file_name, std::string{"wolverine"});
}

TEST(ParseFileTypes, ParseFileList)
{
    Plato::TestFileTypes tTestBlock;
    tTestBlock.file_list = Plato::FileList{{"storm", "cyclops"}};
    Plato::Test::test_existence_and_equality(tTestBlock.file_list, std::vector<std::string>{"storm", "cyclops"});
}
