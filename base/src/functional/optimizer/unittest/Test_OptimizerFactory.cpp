#include <gtest/gtest.h>

#include "InputParser.hpp"
#include "OptimizerFactory.hpp"

namespace Plato::Functional::Test
{
TEST(OptimizerFactory, ParlistGenerationFromInput)
{
    const std::string tInput =
        R"(
          begin optimization_parameters
            step_tolerance 10
            gradient_tolerance 100.0
          end
       )";

    const Plato::PlatoInput tData = Plato::Functional::parse_input(tInput);
    ROL::ParameterList tParlist = rol_parameter_list(tData.mOptimizationParameters);

    constexpr int kDefaultIterationLimit = 10;
    EXPECT_EQ(tParlist.sublist("Status Test").get<int>("Iteration Limit"), kDefaultIterationLimit);
    EXPECT_EQ(tParlist.sublist("Status Test").get<double>("Gradient Tolerance"),
              tData.mOptimizationParameters.gradient_tolerance.value());
    EXPECT_EQ(tParlist.sublist("Status Test").get<double>("Step Tolerance"),
              tData.mOptimizationParameters.step_tolerance.value());
}

TEST(OptimizerFactory, ParlistGenerationFromFile)
{
    const std::string kFileName = "test.xml";

    ROL::ParameterList tParameterListToWrite;
    tParameterListToWrite.sublist("Status Test").set<int>("Iteration Limit", 42);
    tParameterListToWrite.sublist("Status Test").set<double>("Gradient Tolerance", 0.5);
    tParameterListToWrite.sublist("Status Test").set<double>("Step Tolerance", 0.25);
    Teuchos::writeParameterListToXmlFile(tParameterListToWrite, kFileName);

    const std::string tInput =
        "begin optimization_parameters"
        " input_file_name" +
        kFileName + " step_tolerance 10" + " end";

    const Plato::PlatoInput tData = Plato::Functional::parse_input(tInput);
    ROL::ParameterList tParameterListFromDisk = rol_parameter_list(tData.mOptimizationParameters);

    EXPECT_EQ(tParameterListFromDisk.sublist("Status Test").get<int>("Iteration Limit"),
              tParameterListToWrite.sublist("Status Test").get<int>("Iteration Limit"));
    EXPECT_EQ(tParameterListFromDisk.sublist("Status Test").get<double>("Gradient Tolerance"),
              tParameterListToWrite.sublist("Status Test").get<double>("Gradient Tolerance"));
    EXPECT_EQ(tParameterListFromDisk.sublist("Status Test").get<double>("Step Tolerance"),
              tParameterListToWrite.sublist("Status Test").get<double>("Step Tolerance"));

    std::filesystem::remove(kFileName);
}

}  // namespace Plato::Functional::Test
