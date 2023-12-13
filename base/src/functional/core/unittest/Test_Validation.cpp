#include <gtest/gtest.h>

#include <exception>
#include <iostream>

#include "ValidationRegistration.hpp"
namespace
{
std::optional<std::string> test_fun(const Plato::PlatoInput&) { return std::nullopt; }
std::optional<std::string> test_fun2(const Plato::PlatoInput&) { return std::nullopt; }
std::optional<std::string> test_fun_obj(const Plato::objective&) { return "cruel objective"; }

[[maybe_unused]] static auto kValidationRegistration = Plato::Functional::Validation::Registration<Plato::PlatoInput>{

    [](const Plato::PlatoInput& aInput) { return test_fun(aInput); },
    [](const Plato::PlatoInput& aInput) { return test_fun2(aInput); }};

[[maybe_unused]] static auto kValidationRegistrationObjective =
    Plato::Functional::Validation::Registration<Plato::objective>{[](const Plato::objective& aInput)
                                                                  { return test_fun_obj(aInput); }};

}  // namespace

TEST(Validation, RegistrationGeneral)
{
    Plato::PlatoInput tInput;
    auto tRegisteredFunctions = Plato::Functional::Validation::detail::registered_functions<Plato::PlatoInput>();
    EXPECT_EQ(tRegisteredFunctions.size(), 2u);

    EXPECT_FALSE(tRegisteredFunctions[0](tInput).has_value());
    EXPECT_FALSE(tRegisteredFunctions[1](tInput).has_value());
}

TEST(Validation, RegistrationObjective)
{
    Plato::objective tInput;
    auto tRegisteredFunctions = Plato::Functional::Validation::detail::registered_functions<Plato::objective>();
    EXPECT_EQ(tRegisteredFunctions.size(), 1u);
    EXPECT_TRUE(tRegisteredFunctions[0](tInput).has_value());
}
