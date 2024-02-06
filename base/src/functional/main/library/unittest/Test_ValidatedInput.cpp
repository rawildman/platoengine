#include <gtest/gtest.h>

#include "utilities/Exception.hpp"
#include "input_parser/InputBlocks.hpp"
#include "test_utilities/InputGeneration.hpp"
#include "main/library/ValidatedInput.hpp"

namespace plato::functional::main::library::unittest
{
TEST(ValidatedInput, MakeValidInputWithInvalidInput)
{
    EXPECT_THROW(const auto tValidatedInput = make_validated_input(input_parser::ParsedInput{}), utilities::Exception);
}

TEST(ValidatedInput, MakeValidInputWithValidInput)
{
    EXPECT_NO_THROW(const auto tValidatedInput = make_validated_input(test_utilities::create_valid_example_input()));
}
}  // namespace plato::functional::main::library::unittest
