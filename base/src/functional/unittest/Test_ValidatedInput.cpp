#include <gtest/gtest.h>

#include "Exception.hpp"
#include "InputGeneration.hpp"
#include "Plato_InputBlocks.hpp"
#include "ValidatedInput.hpp"

TEST(ValidatedInput, MakeValidInputWithInvalidInput)
{
    namespace pfc = Plato::Functional::Core;
    EXPECT_THROW(const auto tValidatedInput = pfc::make_validated_input(Plato::PlatoInput{}), Plato::Functional::Exception);
}

TEST(ValidatedInput, MakeValidInputWithValidInput)
{
    namespace pf = Plato::Functional;
    EXPECT_NO_THROW(const auto tValidatedInput = pf::Core::make_validated_input(pf::TestUtilities::create_valid_example_input()));
}