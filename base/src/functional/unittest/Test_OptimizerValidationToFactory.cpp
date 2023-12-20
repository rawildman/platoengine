#include <gtest/gtest.h>

#include "InputGeneration.hpp"
#include "InputParser.hpp"
#include "OptimizerFactory.hpp"
#include "ValidatedInput.hpp"

namespace Plato::Functional::Test
{
TEST(OptimizerFactory, ParlistGenerationFromInput)
{
    const std::string tInput = TestUtilities::create_valid_brick_shape_geometry_string() +
                               TestUtilities::create_valid_example_objective_string() +
                               R"(
                                  begin optimization_parameters
                                    step_tolerance 10
                                    gradient_tolerance 100.0
                                    max_iterations 10
                                  end
                              )";

    const Validation::ValidatedInput tData{Validation::make_validated_input(parse_input(tInput))};

    const auto tOPData = tData.optimizationParameters();
    ROL::ParameterList tParlist = rol_parameter_list(tOPData);

    constexpr int kDefaultIterationLimit = 10;
    EXPECT_EQ(tParlist.sublist("Status Test").get<int>("Iteration Limit"), kDefaultIterationLimit);
    EXPECT_EQ(tParlist.sublist("Status Test").get<double>("Gradient Tolerance"),
              tOPData.value().gradient_tolerance.value());
    EXPECT_EQ(tParlist.sublist("Status Test").get<double>("Step Tolerance"),
              tOPData.value().step_tolerance.value());
}

TEST(OptimizerFactory, ParlistGenerationFromFile)
{
    const std::string kFileName = "test.xml";

    ROL::ParameterList tParameterListToWrite;
    tParameterListToWrite.sublist("Status Test").set<int>("Iteration Limit", 42);
    tParameterListToWrite.sublist("Status Test").set<double>("Gradient Tolerance", 0.5);
    tParameterListToWrite.sublist("Status Test").set<double>("Step Tolerance", 0.25);
    Teuchos::writeParameterListToXmlFile(tParameterListToWrite, kFileName);

    const std::string tInput = TestUtilities::create_valid_brick_shape_geometry_string() +
                               TestUtilities::create_valid_example_objective_string() +
                               "begin optimization_parameters"
                               " input_file_name" +
                               kFileName + " step_tolerance 10" + " end";

    const Validation::ValidatedInput tData{Validation::make_validated_input(parse_input(tInput))};
    ROL::ParameterList tParameterListFromDisk = rol_parameter_list(tData.optimizationParameters());

    EXPECT_EQ(tParameterListFromDisk.sublist("Status Test").get<int>("Iteration Limit"),
              tParameterListToWrite.sublist("Status Test").get<int>("Iteration Limit"));
    EXPECT_EQ(tParameterListFromDisk.sublist("Status Test").get<double>("Gradient Tolerance"),
              tParameterListToWrite.sublist("Status Test").get<double>("Gradient Tolerance"));
    EXPECT_EQ(tParameterListFromDisk.sublist("Status Test").get<double>("Step Tolerance"),
              tParameterListToWrite.sublist("Status Test").get<double>("Step Tolerance"));

    std::filesystem::remove(kFileName);
}

}  // namespace Plato::Functional::Test
