#include <gtest/gtest.h>

#include "../OptimizationProblem.hpp"
#include "Exception.hpp"

TEST(PlatoProblem, InvalidInput)
{
    namespace pf = Plato::Functional;
    const std::string tInput;
    EXPECT_THROW(const pf::ValidatedInput tData = pf::parse_and_validate(""), pf::Exception);
}