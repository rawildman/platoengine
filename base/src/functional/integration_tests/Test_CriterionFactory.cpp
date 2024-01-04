#include <gtest/gtest.h>

#include <boost/optional/optional_io.hpp>

#include "CriterionFactory.hpp"
#include "InputGeneration.hpp"
#include "ValidatedInput.hpp"

TEST(CriterionFactory, ValidObjective)
{
    namespace pfv = Plato::Functional::Validation;
    namespace pftu = Plato::Functional::TestUtilities;

    const std::string tObjectiveInput = pftu::create_valid_example_objective_string();
    const std::string tGeometryInput = pftu::create_valid_density_topology_geometry_string();
    const std::string tOptimizerInput = pftu::create_valid_example_optimization_parameters_string();
    const pfv::ValidatedInput tData = pfv::parse_and_validate(tObjectiveInput + tGeometryInput + tOptimizerInput);

    ASSERT_EQ(tData.objectives().value().size(), 1);
    EXPECT_NO_THROW(auto tFunction = Plato::Functional::CriterionFactory::make_criterion_function(
                        tData.objectives().value().front()));
}

TEST(CriterionFactory, ValidConstraint)
{
    namespace pfv = Plato::Functional::Validation;
    namespace pftu = Plato::Functional::TestUtilities;

    const std::string tConstraintInput = pftu::create_valid_example_constraint_string();
    const std::string tObjectiveInput = pftu::create_valid_example_objective_string();
    const std::string tGeometryInput = pftu::create_valid_density_topology_geometry_string();
    const std::string tOptimizerInput = pftu::create_valid_example_optimization_parameters_string();

    const pfv::ValidatedInput tData =
        pfv::parse_and_validate(tConstraintInput + tObjectiveInput + tGeometryInput + tOptimizerInput);

    ASSERT_EQ(tData.constraints().value().size(), 1);
    EXPECT_NO_THROW(auto tFunction = Plato::Functional::CriterionFactory::make_criterion_function(
                        tData.constraints().value().front()));
}

TEST(CriterionRegistration, ConvertObjectiveInput)
{
    namespace pf = Plato::Functional;
    namespace pfv = pf::Validation;
    namespace pftu = pf::TestUtilities;

    const std::string tObjectiveInput = pftu::create_valid_example_custom_app_objective_string();
    const std::string tGeometryInput = pftu::create_valid_density_topology_geometry_string();
    const std::string tOptimizerInput = pftu::create_valid_example_optimization_parameters_string();

    const pfv::ValidatedInput tData = pfv::parse_and_validate(tObjectiveInput + tGeometryInput + tOptimizerInput);
    ASSERT_EQ(tData.objectives().value().size(), 1);
    const pf::Core::ValidatedInputTypeWrapper<Plato::objective>& tValidatedObjective =
        tData.objectives().value().front();

    const pf::CriterionFactory::CriterionInput tCriterionInput =
        pf::CriterionFactory::to_criterion_input(tValidatedObjective);
    const Plato::objective& tObjective = tValidatedObjective.value();

    ASSERT_TRUE(tObjective.shared_library_path.has_value());
    EXPECT_EQ(tObjective.shared_library_path->mName, tCriterionInput.mSharedLibraryPath.mName);
    EXPECT_EQ(tObjective.number_of_processors, tCriterionInput.mNumberOfProcessors);
    ASSERT_TRUE(tObjective.input_files.has_value());
    ASSERT_EQ(tObjective.input_files->mList.size(), tCriterionInput.mInputFiles.mList.size());
    for (std::size_t tIndex = 0; tIndex < tObjective.input_files->mList.size(); ++tIndex)
    {
        EXPECT_EQ(tObjective.input_files->mList[tIndex], tCriterionInput.mInputFiles.mList[tIndex]);
    }
}

TEST(CriterionRegistration, ConvertConstraintInput)
{
    namespace pf = Plato::Functional;
    namespace pfv = pf::Validation;
    namespace pftu = pf::TestUtilities;

    const std::string tConstraintInput = pftu::create_valid_example_constraint_string();
    const std::string tObjectiveInput = pftu::create_valid_example_objective_string();
    const std::string tGeometryInput = pftu::create_valid_density_topology_geometry_string();
    const std::string tOptimizerInput = pftu::create_valid_example_optimization_parameters_string();

    const pfv::ValidatedInput tData =
        pfv::parse_and_validate(tConstraintInput + tObjectiveInput + tGeometryInput + tOptimizerInput);
    ASSERT_EQ(tData.constraints().value().size(), 1);
    const pf::Core::ValidatedInputTypeWrapper<Plato::constraint>& tValidatedConstraint =
        tData.constraints().value().front();

    const pf::CriterionFactory::CriterionInput tCriterionInput =
        pf::CriterionFactory::to_criterion_input(tValidatedConstraint);
    const Plato::constraint& tConstraint = tValidatedConstraint.value();

    EXPECT_TRUE(tCriterionInput.mSharedLibraryPath.mName.empty());
    EXPECT_EQ(tConstraint.number_of_processors, tCriterionInput.mNumberOfProcessors);
    ASSERT_TRUE(tConstraint.input_files.has_value());
    ASSERT_EQ(tConstraint.input_files->mList.size(), tCriterionInput.mInputFiles.mList.size());
    for (std::size_t tIndex = 0; tIndex < tConstraint.input_files->mList.size(); ++tIndex)
    {
        EXPECT_EQ(tConstraint.input_files->mList[tIndex], tCriterionInput.mInputFiles.mList[tIndex]);
    }
}
