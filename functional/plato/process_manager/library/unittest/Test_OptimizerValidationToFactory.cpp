#include <gtest/gtest.h>

#include "plato/input_parser/InputParser.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"
#include "plato/rol_integration/OptimizerFactory.hpp"
#include "plato/test_utilities/InputGeneration.hpp"

namespace plato::process_manager::library::unittest
{
namespace
{
void checkParameterList(
    const core::ValidatedInputTypeWrapper<input_parser::optimization_parameters>& aOptimizationParameters)
{
    const ROL::ParameterList tParlist = plato::rol_integration::rol_parameter_list(aOptimizationParameters);
    EXPECT_EQ(tParlist.sublist("Status Test").get<int>("Iteration Limit"),
              aOptimizationParameters.rawInput().max_iterations.value());
    EXPECT_EQ(tParlist.sublist("Status Test").get<double>("Gradient Tolerance"),
              aOptimizationParameters.rawInput().gradient_tolerance.value());
    EXPECT_EQ(tParlist.sublist("Status Test").get<double>("Step Tolerance"),
              aOptimizationParameters.rawInput().step_tolerance.value());
}
}  // namespace
TEST(OptimizerFactory, ParlistGenerationFromInput)
{
    const std::string tInput = plato::test_utilities::create_valid_brick_shape_geometry_string() +
                               plato::test_utilities::create_valid_example_objective_string() +
                               R"(
                                  begin optimization_parameters
                                    step_tolerance 10
                                    gradient_tolerance 100.0
                                    max_iterations 10
                                  end
                              )";

    const ValidatedInput tData{make_validated_input(input_parser::parse_input(tInput))};

    checkParameterList(tData.optimizationParameters());

    const auto tProcessManagerData = tData.processManagers();
    ASSERT_EQ(tProcessManagerData.rawInput().size(), 1);
    using ValidatedOptimizationParameters = core::ValidatedInputTypeWrapper<input_parser::optimization_parameters>;
    ASSERT_TRUE(std::holds_alternative<ValidatedOptimizationParameters>(tProcessManagerData.rawInput().front()));
    checkParameterList(std::get<ValidatedOptimizationParameters>(tProcessManagerData.rawInput().front()));
}

TEST(OptimizerFactory, ParlistGenerationFromFile)
{
    const std::string kFileName = "test.xml";

    ROL::ParameterList tParameterListToWrite;
    tParameterListToWrite.sublist("Status Test").set<int>("Iteration Limit", 42);
    tParameterListToWrite.sublist("Status Test").set<double>("Gradient Tolerance", 0.5);
    tParameterListToWrite.sublist("Status Test").set<double>("Step Tolerance", 0.25);
    Teuchos::writeParameterListToXmlFile(tParameterListToWrite, kFileName);

    const std::string tInput = plato::test_utilities::create_valid_brick_shape_geometry_string() +
                               plato::test_utilities::create_valid_example_objective_string() +
                               "begin optimization_parameters"
                               " input_file_name" +
                               kFileName + " step_tolerance 10" + " end";

    const ValidatedInput tData{make_validated_input(input_parser::parse_input(tInput))};
    ROL::ParameterList tParameterListFromDisk =
        plato::rol_integration::rol_parameter_list(tData.optimizationParameters());

    EXPECT_EQ(tParameterListFromDisk.sublist("Status Test").get<int>("Iteration Limit"),
              tParameterListToWrite.sublist("Status Test").get<int>("Iteration Limit"));
    EXPECT_EQ(tParameterListFromDisk.sublist("Status Test").get<double>("Gradient Tolerance"),
              tParameterListToWrite.sublist("Status Test").get<double>("Gradient Tolerance"));
    EXPECT_EQ(tParameterListFromDisk.sublist("Status Test").get<double>("Step Tolerance"),
              tParameterListToWrite.sublist("Status Test").get<double>("Step Tolerance"));

    std::filesystem::remove(kFileName);
}

}  // namespace plato::process_manager::library::unittest
