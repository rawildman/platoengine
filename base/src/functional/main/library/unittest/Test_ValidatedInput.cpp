#include <gtest/gtest.h>

#include "Exception.hpp"
#include "InputBlocks.hpp"
#include "InputGeneration.hpp"
#include "ValidatedInput.hpp"

namespace plato::functional::main::library::unittest
{
TEST(ValidatedInput, MakeValidInputWithInvalidInput)
{
    EXPECT_THROW(const auto tValidatedInput = make_validated_input(input_parser::PlatoInput{}), utilities::Exception);
}

TEST(ValidatedInput, MakeValidInputWithValidInput)
{
    EXPECT_NO_THROW(const auto tValidatedInput = make_validated_input(test_utilities::create_valid_example_input()));
}
}  // namespace plato::functional::main::library::unittest
