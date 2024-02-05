#include <gtest/gtest.h>

#include "CriterionValidation.hpp"
#include "InputBlocks.hpp"
#include "InputGeneration.hpp"
#include "ObjectiveValidation.hpp"

namespace plato::functional::criteria::library::unittest
{
TEST(ObjectiveValidation, ValidateAggregationWeight)
{
    namespace pfcd = plato::functional::criteria::library::detail;
    Plato::objective tObjective;
    EXPECT_TRUE(pfcd::validate_aggregation_weight(tObjective).has_value());
    tObjective.aggregation_weight = 13.0;
    EXPECT_FALSE(pfcd::validate_aggregation_weight(tObjective).has_value());
    tObjective.aggregation_weight = -13.0;
    EXPECT_TRUE(pfcd::validate_aggregation_weight(tObjective).has_value());
}

TEST(ObjectiveValidation, ValidateAtLeastOneObjective)
{
    namespace pfcd = plato::functional::criteria::library::detail;
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

TEST(ObjectiveValidation, ValidateMPIRanksVsNumberOfObjectives)
{
    namespace pfcd = plato::functional::criteria::library::detail;

    // One objective and one rank
    Plato::objective tObjective;
    EXPECT_FALSE(pfcd::validate_number_of_ranks_vs_objectives({tObjective}).has_value());
}

TEST(ObjectiveValidation, ErrorMessagesInvalidObjective)
{
    Plato::objective tObjective = plato::functional::test_utilities::create_valid_example_objective();
    tObjective.app = boost::none;
    namespace pfv = Plato::Functional::Validation;
    std::vector<std::string> tMessages;
    tMessages = pfv::validate(tObjective, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 1u);
}

TEST(ObjectiveValidation, ErrorMessagesInvalidInput)
{
    namespace pfc = plato::functional::criteria::library;
    const Plato::objective tObjective;
    const std::vector<Plato::objective> tInput{tObjective, tObjective};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_objectives(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 4u);
}

TEST(ObjectiveValidation, NoErrorMessagesValidObjective)
{
    namespace pfc = plato::functional::criteria::library;
    const auto tObjective = plato::functional::test_utilities::create_valid_example_objective();
    const std::vector<Plato::objective> tInput{tObjective, tObjective};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_objectives(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 0u);
}

TEST(ObjectiveValidation, ErrorMessagesInvalidObjectives)
{
    namespace pfc = plato::functional::criteria::library;
    auto tObjective = plato::functional::test_utilities::create_valid_example_objective();
    tObjective.active = false;
    const std::vector<Plato::objective> tInput{tObjective, tObjective};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_objectives(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 1u);
}
}
