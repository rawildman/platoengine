#include <gtest/gtest.h>

#include <exception>
#include <iostream>

#include "ValidationRegistration.hpp"
namespace
{
std::optional<std::string> test_fun(const Plato::PlatoInput&) { return std::nullopt; }
std::optional<std::string> test_fun2(const Plato::PlatoInput&) { return "error message cruel objective"; }
std::optional<std::string> test_fun_obj(const Plato::objective&) { return "error message cruel objective"; }
std::optional<std::string> test_fun_obj2(const Plato::objective&) { return std::nullopt; }

[[maybe_unused]] static auto kValidationRegistration = Plato::Functional::Validation::Registration<Plato::PlatoInput>{

    [](const Plato::PlatoInput& aInput) { return test_fun(aInput); },
    [](const Plato::PlatoInput& aInput) { return test_fun2(aInput); }};

[[maybe_unused]] static auto kValidationRegistrationObjective =
    Plato::Functional::Validation::Registration<Plato::objective>{[](const Plato::objective& aInput)
                                                                  { return test_fun_obj(aInput); }};

[[maybe_unused]] static auto kValidationRegistrationObjectiveSecondIntentionalSplitForCTOR =
    Plato::Functional::Validation::Registration<Plato::objective>{[](const Plato::objective& aInput)
                                                                  { return test_fun_obj2(aInput); }};

}  // namespace

TEST(Validation, RegistrationGeneral)
{
    Plato::PlatoInput tInput;
    auto tRegisteredFunctions = Plato::Functional::Validation::detail::registered_functions<Plato::PlatoInput>();
    EXPECT_EQ(tRegisteredFunctions.size(), 2u);
}

TEST(Validation, RegistrationObjective)
{
    Plato::objective tInput;
    auto tRegisteredFunctions = Plato::Functional::Validation::detail::registered_functions<Plato::objective>();
    EXPECT_EQ(tRegisteredFunctions.size(), 2u);
}

TEST(Validation, ValidateInputObjective)
{
    Plato::objective tInput;
    std::vector<std::string> tMessages;
    tMessages = Plato::Functional::Validation::validate<Plato::objective>(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 1u);
}

TEST(Validation, ValidateInputGeneral)
{
    Plato::PlatoInput tInput;
    std::vector<std::string> tMessages;
    tMessages = Plato::Functional::Validation::validate<Plato::PlatoInput>(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 1u);
}

TEST(Validation, ValidateInputObjectiveList_EmptyNoneDefined)
{
    Plato::objective tInput;
    std::vector<Plato::objective> tInputList{tInput, tInput};
    std::vector<std::string> tMessages;
    tMessages =
        Plato::Functional::Validation::validate<std::vector<Plato::objective>>(tInputList, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 0u);
}