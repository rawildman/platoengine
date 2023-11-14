#include <gtest/gtest.h>

#include <boost/optional/optional_io.hpp>

#include "CriterionFactory.hpp"
#include "InputParser.hpp"

TEST(CriterionFactory, ValidObjective)
{
    constexpr std::string_view tInput =
        R"(
          begin objective test
            active true
            app nodal_sum
            number_of_processors 4
            input_files test-input.inp
            aggregation_weight 42.0
            objective_type minimize
          end
       )";

    const Plato::PlatoInput tData = Plato::Functional::parse_input(tInput);

    EXPECT_EQ(tData.mObjectives.size(), 1);
    EXPECT_NO_THROW(auto tFunction =
                        Plato::Functional::CriterionFactory::make_criterion_function(tData.mObjectives.front()));
}

TEST(CriterionFactory, ValidConstraint)
{
    constexpr std::string_view tInput =
        R"(
          begin constraint test
            active true
            app nodal_sum
            number_of_processors 4
            input_files test-input.inp
            equal_to 13
            is_linear true
          end
       )";

    const Plato::PlatoInput tData = Plato::Functional::parse_input(tInput);

    ASSERT_EQ(tData.mConstraints.size(), 1);
    EXPECT_NO_THROW(auto tFunction =
                        Plato::Functional::CriterionFactory::make_criterion_function(tData.mConstraints.front()));
}

TEST(CriterionRegistration, ConvertObjectiveInput)
{
    namespace pf = Plato::Functional;

    const auto tObjective = Plato::objective{/*.name=*/std::string{"bike-shed"},
                                             /*.active=*/true,
                                             /*.app=*/Plato::CodeOptions::kCustomApp,
                                             /*.shared_library_path=*/Plato::FileName{"/sweet/potato/ravioli.so"},
                                             /*.number_of_processors=*/42u,
                                             /*.input_files=*/Plato::FileList{{"brown.txt", "butter.txt", "sauce.txt"}},
                                             /*.aggregation_weight=*/13.0,
                                             /*.objective_type=*/Plato::ObjectiveTypes::kMaximize};

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

    const auto tConstraint =
        Plato::constraint{/*.name=*/std::string{"bleu-bike-shed"},
                          /*.active=*/true,
                          /*.app=*/Plato::CodeOptions::kCustomApp,
                          /*.shared_library_path=*/Plato::FileName{"/sweet/potato/ravioli.so"},
                          /*.number_of_processors=*/42u,
                          /*.input_files=*/Plato::FileList{{"brown.txt", "butter.txt", "sauce.txt"}},
                          /*.equal_to=*/boost::none,
                          /*.less_than=*/boost::none,
                          /*.greater_than=*/boost::none,
                          /*.is_linear=*/boost::none};

    const pf::CriterionFactory::CriterionInput tCriterionInput = pf::CriterionFactory::to_criterion_input(tConstraint);

    EXPECT_EQ(tConstraint.shared_library_path->mName, tCriterionInput.mSharedLibraryPath.mName);
    EXPECT_EQ(tConstraint.number_of_processors, tCriterionInput.mNumberOfProcessors);
    ASSERT_EQ(tConstraint.input_files->mList.size(), tCriterionInput.mInputFiles.mList.size());
    for (std::size_t tIndex = 0; tIndex < tConstraint.input_files->mList.size(); ++tIndex)
    {
        EXPECT_EQ(tConstraint.input_files->mList[tIndex], tCriterionInput.mInputFiles.mList[tIndex]);
    }
}
