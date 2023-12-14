#include <gtest/gtest.h>

#include <boost/optional/optional_io.hpp>

#include "CriterionFactory.hpp"
#include "InputGeneration.hpp"
#include "InputParser.hpp"

TEST(CriterionFactory, ValidObjective)
{
    const std::string tInput = Plato::Functional::TestUtilities::create_valid_example_objective_string();
    const Plato::PlatoInput tData = Plato::Functional::parse_input(tInput);

    EXPECT_EQ(tData.mObjectives.size(), 1);
    EXPECT_NO_THROW(auto tFunction =
                        Plato::Functional::CriterionFactory::make_criterion_function(tData.mObjectives.front()));
}

TEST(CriterionFactory, ValidConstraint)
{
    const std::string tInput = Plato::Functional::TestUtilities::create_valid_example_constraint_string();
    const Plato::PlatoInput tData = Plato::Functional::parse_input(tInput);

    ASSERT_EQ(tData.mConstraints.size(), 1);
    EXPECT_NO_THROW(auto tFunction =
                        Plato::Functional::CriterionFactory::make_criterion_function(tData.mConstraints.front()));
}

TEST(CriterionRegistration, ConvertObjectiveInput)
{
    namespace pf = Plato::Functional;
    const auto tObjective = Plato::Functional::TestUtilities::create_valid_example_objective();

    const pf::CriterionFactory::CriterionInput tCriterionInput = pf::CriterionFactory::to_criterion_input(tObjective);

    EXPECT_EQ(tObjective.shared_library_path->mName, tCriterionInput.mSharedLibraryPath.mName);
    EXPECT_EQ(tObjective.number_of_processors, tCriterionInput.mNumberOfProcessors);
    ASSERT_EQ(tObjective.input_files->mList.size(), tCriterionInput.mInputFiles.mList.size());
    for (std::size_t tIndex = 0; tIndex < tObjective.input_files->mList.size(); ++tIndex)
    {
        EXPECT_EQ(tObjective.input_files->mList[tIndex], tCriterionInput.mInputFiles.mList[tIndex]);
    }
}

TEST(CriterionRegistration, ConvertConstraintInput)
{
    namespace pf = Plato::Functional;

    const auto tConstraint = Plato::Functional::TestUtilities::create_valid_example_constraint();
    const pf::CriterionFactory::CriterionInput tCriterionInput = pf::CriterionFactory::to_criterion_input(tConstraint);

    EXPECT_EQ(tConstraint.shared_library_path->mName, tCriterionInput.mSharedLibraryPath.mName);
    EXPECT_EQ(tConstraint.number_of_processors, tCriterionInput.mNumberOfProcessors);
    ASSERT_EQ(tConstraint.input_files->mList.size(), tCriterionInput.mInputFiles.mList.size());
    for (std::size_t tIndex = 0; tIndex < tConstraint.input_files->mList.size(); ++tIndex)
    {
        EXPECT_EQ(tConstraint.input_files->mList[tIndex], tCriterionInput.mInputFiles.mList[tIndex]);
    }
}
