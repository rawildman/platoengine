#include <gtest/gtest.h>

#include "AffirmUtilities.hpp"
#include "CriterionValidation.hpp"
#include "ObjectiveValidation.hpp"
#include "Plato_InputBlocks.hpp"

namespace
{
Plato::objective create_valid_example_objective()
{
    return Plato::objective{/*.name=*/std::string{"bike-shed"},
                            /*.active=*/true,
                            /*.app=*/Plato::CodeOptions::kCustomApp,
                            /*.shared_library_path=*/Plato::FileName{"/sweet/potato/ravioli.so"},
                            /*.number_of_processors=*/42u,
                            /*.input_files=*/Plato::FileList{{"brown.txt", "butter.txt", "sauce.txt"}},
                            /*.aggregation_weight=*/13.0,
                            /*.objective_type=*/Plato::ObjectiveTypes::kMaximize};
}

}  // namespace

TEST(ObjectiveValidation, ValidateApp)
{
    namespace pfcd = Plato::Functional::Criteria::detail;
    Plato::objective tObjective;
    EXPECT_TRUE(pfcd::validate_app(tObjective).has_value());
    tObjective.app = Plato::CodeOptions::kCustomApp;
    EXPECT_FALSE(pfcd::validate_app(tObjective).has_value());
}

TEST(ObjectiveValidation, ValidateCustomApp)
{
    namespace pfcd = Plato::Functional::Criteria::detail;
    Plato::objective tObjective;
    tObjective.app = Plato::CodeOptions::kCustomApp;
    EXPECT_TRUE(pfcd::validate_custom_app(tObjective).has_value());
    tObjective.shared_library_path = Plato::FileName{"/sweet/potato/ravioli.so"};
    EXPECT_FALSE(pfcd::validate_custom_app(tObjective).has_value());
}

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
    Plato::objective tObjective = create_valid_example_objective();
    tObjective.app = boost::none;
    namespace pfv = Plato::Functional::Validation;
    std::vector<std::string> tMessages;
    tMessages = pfv::validate<Plato::objective>(tObjective, tMessages);
    EXPECT_TRUE(tMessages.size() > 0);
    Plato::Functional::Affirmations::print_messages(tMessages);
}

TEST(ObjectiveValidation, ErrorMessagesInvalidInput)
{
    namespace pfc = Plato::Functional::Criteria;
    const Plato::objective tObjective;
    const std::vector<Plato::objective> tInput{tObjective, tObjective};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_objectives(tInput, tMessages);
    EXPECT_TRUE(tMessages.size() > 0);
    Plato::Functional::Affirmations::print_messages(tMessages);
}

TEST(ObjectiveValidation, NoErrorMessagesValidObjective)
{
    namespace pfc = Plato::Functional::Criteria;
    const auto tObjective = create_valid_example_objective();
    const std::vector<Plato::objective> tInput{tObjective, tObjective};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_objectives(tInput, tMessages);
    EXPECT_EQ(tMessages.size(), 0u);
}

TEST(ObjectiveValidation, ErrorMessagesInvalidObjectives)
{
    namespace pfc = Plato::Functional::Criteria;
    auto tObjective = create_valid_example_objective();
    tObjective.active = false;
    const std::vector<Plato::objective> tInput{tObjective, tObjective};

    std::vector<std::string> tMessages;
    tMessages = pfc::validate_objectives(tInput, tMessages);
    EXPECT_EQ(tMessages.size(), 1u);
    Plato::Functional::Affirmations::print_messages(tMessages);
}