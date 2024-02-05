#include <gtest/gtest.h>

#include "Exception.hpp"
#include "InputBlocks.hpp"
#include "InputGeneration.hpp"
#include "ValidatedInput.hpp"

namespace plato::functional::main::library::unittest
{
TEST(ValidatedInput, MakeValidInputWithInvalidInput)
{
    namespace pf = Plato::Functional;
    EXPECT_THROW(const auto tValidatedInput = make_validated_input(Plato::PlatoInput{}), pf::Exception);
}

TEST(ValidatedInput, MakeValidInputWithValidInput)
{
    namespace pf = Plato::Functional;
    EXPECT_NO_THROW(const auto tValidatedInput =
                        make_validated_input(plato::functional::test_utilities::create_valid_example_input()));
}
}  // namespace plato::functional::main::library::unittest
