#include <gtest/gtest.h>

#include "AffirmUtilities.hpp"
#include "CriterionValidation.hpp"
#include "InputGeneration.hpp"
#include "ObjectiveValidation.hpp"
#include "Plato_InputBlocks.hpp"

TEST(ObjectiveValidation, ValidateAggregationWeight)
{
    namespace pfcd = Plato::Functional::Criteria::detail;
    Plato::objective tObjective;
    EXPECT_TRUE(pfcd::validate_aggregation_weight(tObjective).has_value());
    tObjective.aggregation_weight = 13.0;
    EXPECT_FALSE(pfcd::validate_aggregation_weight(tObjective).has_value());
}

TEST(ObjectiveValidation, ValidateAtLeastOneObjective)
{
    namespace pfcd = Plato::Functional::Criteria::detail;
    EXPECT_TRUE(pfcd::validate_at_least_one_objective({}).has_value());
    Plato::objective tObjective;
    EXPECT_FALSE(pfcd::validate_at_least_one_objective({tObjective}).has_value());
    tObjective.active = false;
    EXPECT_TRUE(pfcd::validate_at_least_one_objective({tObjective}).has_value());
    EXPECT_TRUE(pfcd::validate_at_least_one_objective({tObjective, tObjective}).has_value());

    Plato::objective tObjectiveTwo;
    tObjectiveTwo.active = true;
    EXPECT_FALSE(pfcd::validate_at_least_one_objective({tObjective, tObjectiveTwo}).has_value());
    EXPECT_FALSE(pfcd::validate_at_least_one_objective({tObjectiveTwo, tObjectiveTwo}).has_value());
}

TEST(ObjectiveValidation, ErrorMessagesInvalidObjective)
{
    Plato::objective tObjective = Plato::Functional::TestUtilities::create_valid_example_objective();
    tObjective.app = boost::none;
    namespace pfv = Plato::Functional::Validation;
    std::vector<std::string> tMessages;
    tMessages = pfv::validate<Plato::objective>(tObjective, std::move(tMessages));
    EXPECT_TRUE(tMessages.size() > 0);
    Plato::Functional::Validation::print_messages(tMessages);
}

TEST(ObjectiveValidation, ErrorMessagesInvalidInput)
{
    namespace pfc = Plato::Functional::Criteria;
    const Plato::objective tObjective;
    const std::vector<Plato::objective> tInput{tObjective, tObjective};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_objectives(tInput, std::move(tMessages));
    EXPECT_TRUE(tMessages.size() > 0);
    Plato::Functional::Validation::print_messages(tMessages);
}

TEST(ObjectiveValidation, NoErrorMessagesValidObjective)
{
    namespace pfc = Plato::Functional::Criteria;
    const auto tObjective = Plato::Functional::TestUtilities::create_valid_example_objective();
    const std::vector<Plato::objective> tInput{tObjective, tObjective};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_objectives(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 0u);
}

TEST(ObjectiveValidation, ErrorMessagesInvalidObjectives)
{
    namespace pfc = Plato::Functional::Criteria;
    auto tObjective = Plato::Functional::TestUtilities::create_valid_example_objective();
    tObjective.active = false;
    const std::vector<Plato::objective> tInput{tObjective, tObjective};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_objectives(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 1u);
    Plato::Functional::Validation::print_messages(tMessages);
}