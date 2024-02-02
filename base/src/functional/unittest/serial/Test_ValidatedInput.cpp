#include <gtest/gtest.h>

#include "Exception.hpp"
#include "InputGeneration.hpp"
#include "InputBlocks.hpp"
#include "ValidatedInput.hpp"

TEST(ValidatedInput, MakeValidInputWithInvalidInput)
{
    namespace pf = Plato::Functional;
    namespace pfv = Plato::Functional::Validation;
    EXPECT_THROW(const auto tValidatedInput = pfv::make_validated_input(Plato::PlatoInput{}), pf::Exception);
}

TEST(ValidatedInput, MakeValidInputWithValidInput)
{
    namespace pf = Plato::Functional;
    namespace pfv = Plato::Functional::Validation;
    EXPECT_NO_THROW(const auto tValidatedInput = pfv::make_validated_input(pf::TestUtilities::create_valid_example_input()));
}