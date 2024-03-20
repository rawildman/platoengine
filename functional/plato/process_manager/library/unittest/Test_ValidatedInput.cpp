#include <gtest/gtest.h>

#include "plato/input_parser/InputBlocks.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"
#include "plato/test_utilities/InputGeneration.hpp"
#include "plato/utilities/Exception.hpp"

namespace plato::process_manager::library::unittest
{
TEST(ValidatedInput, MakeValidInputWithInvalidInput)
{
    EXPECT_THROW(const auto tValidatedInput = make_validated_input(input_parser::ParsedInput{}), utilities::Exception);
}

TEST(ValidatedInput, MakeValidInputWithValidInput)
{
    EXPECT_NO_THROW(const auto tValidatedInput = make_validated_input(test_utilities::create_valid_example_input()));
}
}  // namespace plato::process_manager::library::unittest
