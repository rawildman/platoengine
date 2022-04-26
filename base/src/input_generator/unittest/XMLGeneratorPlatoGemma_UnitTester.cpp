 /*
 * XMLGeneratorPlatoGemma_UnitTester.cpp
 *
 *  Created on: Feb 28, 2022
 */

#include <gtest/gtest.h>

#include "pugixml.hpp"
#include <fstream>

#include "XMLGenerator_UnitTester_Tools.hpp"

#include "XMLGeneratorStage.hpp"
#include "XMLGeneratorUtilities.hpp"
#include "XMLGeneratorDataStruct.hpp"
#include "XMLGeneratorFlaskUtilities.hpp"
#include "XMLGeneratorGemmaUtilities.hpp"
#include "XMLGeneratorParserUtilities.hpp"
#include "XMLGeneratorServiceUtilities.hpp"
#include "XMLGeneratorCriteriaUtilities.hpp"
#include "XMLGeneratorScenarioUtilities.hpp"
#include "XMLGeneratorOperationsFileUtilities.hpp"
#include "XMLGeneratorOperationsDataStructures.hpp"
#include "XMLGeneratorGemmaMatchedPowerBalanceUtilities.hpp"

#include "XMLGeneratorGemmaProblem.hpp"
#include "XMLGeneratorOperation.hpp"

namespace PlatoTestXMLGenerator
{

TEST(PlatoTestXMLGenerator, WriteGemmaInputDeckMatchedPowerBalance)
{
    XMLGen::InputData tInputMetaData;

    // define optimization parameters 
    XMLGen::OptimizationParameters tOptParams;
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    tOptParams.descriptors(tDescriptors);
    tOptParams.append("concurrent_evaluations", "2");
    tInputMetaData.set(tOptParams);

    // define scenario
    XMLGen::Scenario tScenario;
    tScenario.append("id", "1");
    tScenario.append("material", "1");
    tScenario.append("frequency_min", "10");
    tScenario.append("frequency_max", "100");
    tScenario.append("frequency_step", "5");
    tScenario.append("cavity_radius", "0.1016");
    tScenario.append("cavity_height", "0.1018");
    tScenario.append("physics", "electromagnetics");
    tInputMetaData.append(tScenario);

    // define material
    XMLGen::Material tMaterial;
    tMaterial.id("1");
    tMaterial.property("conductivity", "1e6");
    tInputMetaData.append(tMaterial);

    XMLGen::XMLGeneratorGemmaProblem tGemmaProblem(tInputMetaData);
    ASSERT_NO_THROW(tGemmaProblem.create_matched_power_balance_input_deck(tInputMetaData));

    auto tReadData = XMLGen::read_data_from_file("gemma_matched_power_balance_input_deck.yaml.template");
    std::string tGoldString = std::string("%YAML1.1---Gemma-dynamic:Global:Description:HigginscylinderSolutiontype:powerbalance") + 
        std::string("Powerbalance:Algorithm:matchedboundRadius:0.1016Height:0.1018Conductivity:1e6Slotlength:{slot_length}") + 
        std::string("Slotwidth:{slot_width}Slotdepth:{slot_depth}Startfrequencyrange:10Endfrequencyrange:100Frequencyintervalsize:5...");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
    Plato::system("rm -rf gemma_matched_power_balance_input_deck.yaml.template");
}

TEST(PlatoTestXMLGenerator, WriteGemmaInputDeckMatchedPowerBalanceToSubdirectories)
{
    XMLGen::InputData tInputMetaData;

    // define optimization parameters 
    XMLGen::OptimizationParameters tOptParams;
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    tOptParams.descriptors(tDescriptors);
    tOptParams.append("concurrent_evaluations", "2");
    tInputMetaData.set(tOptParams);

    // define scenario
    XMLGen::Scenario tScenario;
    tScenario.append("id", "1");
    tScenario.append("material", "1");
    tScenario.append("frequency_min", "50");
    tScenario.append("frequency_max", "300");
    tScenario.append("frequency_step", "7");
    tScenario.append("cavity_radius", "0.1016");
    tScenario.append("cavity_height", "0.1018");
    tScenario.append("physics", "electromagnetics");
    tInputMetaData.append(tScenario);

    // define material
    XMLGen::Material tMaterial;
    tMaterial.id("1");
    tMaterial.property("conductivity", "1e6");
    tInputMetaData.append(tMaterial);

    XMLGen::XMLGeneratorGemmaProblem tGemmaProblem(tInputMetaData);
    ASSERT_NO_THROW(tGemmaProblem.create_evaluation_subdirectories_and_gemma_input(tInputMetaData));

    auto tReadData = XMLGen::read_data_from_file("evaluations_0/gemma_matched_power_balance_input_deck.yaml.template");
    std::string tGoldString = std::string("%YAML1.1---Gemma-dynamic:Global:Description:HigginscylinderSolutiontype:powerbalance") + 
        std::string("Powerbalance:Algorithm:matchedboundRadius:0.1016Height:0.1018Conductivity:1e6Slotlength:{slot_length}") + 
        std::string("Slotwidth:{slot_width}Slotdepth:{slot_depth}Startfrequencyrange:50Endfrequencyrange:300Frequencyintervalsize:7...");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());

    tReadData = XMLGen::read_data_from_file("evaluations_1/gemma_matched_power_balance_input_deck.yaml.template");
    tGoldString = std::string("%YAML1.1---Gemma-dynamic:Global:Description:HigginscylinderSolutiontype:powerbalance") + 
        std::string("Powerbalance:Algorithm:matchedboundRadius:0.1016Height:0.1018Conductivity:1e6Slotlength:{slot_length}") + 
        std::string("Slotwidth:{slot_width}Slotdepth:{slot_depth}Startfrequencyrange:50Endfrequencyrange:300Frequencyintervalsize:7...");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());

    Plato::system("rm -rf evaluations_0");
    Plato::system("rm -rf evaluations_1");
}

TEST(PlatoTestXMLGenerator, WriteGemmaPlatoMainOperationsFile)
{
    XMLGen::InputData tInputMetaData;

    // define criteria
    XMLGen::Criterion tCriterion;
    tCriterion.id("2");
    tCriterion.type("system_call");
    tCriterion.append("data_group", "1");
    tCriterion.append("data_extraction_operation", "max");
    tCriterion.append("data_file", "matched_power_balance.dat");
    tInputMetaData.append(tCriterion);

    // define objective 
    XMLGen::Objective tObjective;
    tObjective.criteriaIDs.push_back("2");
    tInputMetaData.objective = tObjective;

    // define optimization parameters 
    XMLGen::OptimizationParameters tOptParams;
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    tOptParams.descriptors(tDescriptors);
    tOptParams.append("concurrent_evaluations", "2");
    tInputMetaData.set(tOptParams);

    // define services
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tServiceOne.append("type", "system_call");
    tServiceOne.append("number_processors", "1");
    tInputMetaData.append(tServiceOne);
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "platomain");
    tServiceTwo.append("id", "2");
    tServiceTwo.append("type", "plato_app");
    tServiceTwo.append("number_processors", "2");
    tInputMetaData.append(tServiceTwo);

    XMLGen::XMLGeneratorGemmaProblem tGemmaProblem(tInputMetaData);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tGemmaProblem.write_plato_main(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // APREPRO OPERATIONS
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {"Function",
        "Name", 
        "Command",
        "ChDir", 
        "OnChange",
        "Argument",
        "Argument",
        "Argument",
        "AppendInput",
        "Option",
        "Option",
        "Option",
        "Input"};
    std::vector<std::string> tValues = {"SystemCall", 
        "aprepro_0", 
        "aprepro", 
        "evaluations_0",
        "true",
        "-q",
        "gemma_matched_power_balance_input_deck.yaml.template",
        "gemma_matched_power_balance_input_deck.yaml",
        "true",
        "slot_length=",
        "slot_width=",
        "slot_depth=",
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    auto tInput = tOperation.child("Input");
    ASSERT_FALSE(tInput.empty());
    PlatoTestXMLGenerator::test_children({"ArgumentName", "Layout", "Size"}, {"parameters", "scalar", "3"}, tInput);
    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_FALSE(tOperation.empty());
    tValues = {"SystemCall", 
        "aprepro_1", 
        "aprepro", 
        "evaluations_1",
        "true",
        "-q",
        "gemma_matched_power_balance_input_deck.yaml.template",
        "gemma_matched_power_balance_input_deck.yaml",
        "true",
        "slot_length=",
        "slot_width=",
        "slot_depth=",
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tInput = tOperation.child("Input");
    ASSERT_FALSE(tInput.empty());
    PlatoTestXMLGenerator::test_children({"ArgumentName", "Layout", "Size"}, {"parameters", "scalar", "3"}, tInput);
    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    // GEMMA CALL OPERATIONS
    tOperation = tOperation.next_sibling("Operation");
    ASSERT_FALSE(tOperation.empty());
    tKeys = {"Function",
        "Name", 
        "Command", 
        "ChDir",
        "OnChange",
        "NumRanks",
        "Argument",
        "AppendInput"};
    tValues = {"SystemCallMPI", 
        "gemma_0", 
        "gemma", 
        "evaluations_0",
        "true",
        "1",
        "gemma_matched_power_balance_input_deck.yaml",
        "false"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_FALSE(tOperation.empty());
    tValues = {"SystemCallMPI", 
        "gemma_1", 
        "gemma", 
        "evaluations_1",
        "true",
        "1",
        "gemma_matched_power_balance_input_deck.yaml",
        "false"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    // WAIT OPERATIONS
    tOperation = tOperation.next_sibling("Operation");
    ASSERT_FALSE(tOperation.empty());
    tKeys = {"Function",
        "Name", 
        "Command", 
        "ChDir",
        "OnChange"};
    tValues = {"SystemCall", 
        "wait_0", 
        "while lsof -u $USER | grep ./matched_power_balance.dat; do sleep 1; done", 
        "evaluations_0",
        "false"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_FALSE(tOperation.empty());
    tValues = {"SystemCall", 
        "wait_1", 
        "while lsof -u $USER | grep ./matched_power_balance.dat; do sleep 1; done", 
        "evaluations_1",
        "false"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);
    
    // HARVEST DATA OPERATIONS
    tOperation = tOperation.next_sibling("Operation");
    ASSERT_FALSE(tOperation.empty());
    tKeys = {"Function",
        "Name", 
        "File", 
        "Operation",
        "Column",
        "Output"};
    tValues = {"HarvestDataFromFile", 
        "harvest_data_0", 
        "./evaluations_0/matched_power_balance.dat",
        "max",
        "1",
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    auto tOutput = tOperation.child("Output");
    ASSERT_FALSE(tOutput.empty());
    PlatoTestXMLGenerator::test_children({"ArgumentName", "Layout", "Size"}, {"criterion value", "scalar", "1"}, tOutput);
    tOutput = tOutput.next_sibling("Output");
    ASSERT_TRUE(tOutput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_FALSE(tOperation.empty());
    tValues = {"HarvestDataFromFile", 
        "harvest_data_1", 
        "./evaluations_1/matched_power_balance.dat",
        "max",
        "1",
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOutput = tOperation.child("Output");
    ASSERT_FALSE(tOutput.empty());
    PlatoTestXMLGenerator::test_children({"ArgumentName", "Layout", "Size"}, {"criterion value", "scalar", "1"}, tOutput);
    tOutput = tOutput.next_sibling("Output");
    ASSERT_TRUE(tOutput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteGemmaPlatoInterfaceFile)
{
    XMLGen::InputData tInputMetaData;

    // define criteria
    XMLGen::Criterion tCriterion;
    tCriterion.id("2");
    tCriterion.type("system_call");
    tCriterion.append("data_group", "1");
    tCriterion.append("data_extraction_operation", "max");
    tCriterion.append("data_file", "matched_power_balance.dat");
    tInputMetaData.append(tCriterion);

    // define objective 
    XMLGen::Objective tObjective;
    tObjective.criteriaIDs.push_back("2");
    tInputMetaData.objective = tObjective;

    // define optimization parameters 
    XMLGen::OptimizationParameters tOptParams;
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    tOptParams.descriptors(tDescriptors);
    tOptParams.append("concurrent_evaluations", "2");
    tOptParams.append("verbose", "true");
    tInputMetaData.set(tOptParams);

    // define services
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tServiceOne.append("type", "system_call");
    tServiceOne.append("number_processors", "1");
    tInputMetaData.append(tServiceOne);
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "platomain");
    tServiceTwo.append("id", "2");
    tServiceTwo.append("type", "plato_app");
    tServiceTwo.append("number_processors", "2");
    tInputMetaData.append(tServiceTwo);

    XMLGen::XMLGeneratorGemmaProblem tGemmaProblem(tInputMetaData);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tGemmaProblem.write_interface(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // INCLUDE DEFINES
    auto tInclude = tDocument.child("include");
    ASSERT_FALSE(tInclude.empty());
    PlatoTestXMLGenerator::test_attributes({"filename"}, {"defines.xml"}, tInclude);
    tInclude = tInclude.next_sibling("include");
    ASSERT_TRUE(tInclude.empty());

    // CONSOLE
    auto tConsole = tDocument.child("Console");
    ASSERT_FALSE(tConsole.empty());
    ASSERT_STREQ("Console", tConsole.name());
    auto tChild = tConsole.child("Enabled");
    ASSERT_STREQ("false", tChild.child_value());
    tChild = tConsole.child("Verbose");
    ASSERT_STREQ("true", tChild.child_value());

    // PERFORMERS
    auto tPerformer = tDocument.child("Performer");
    ASSERT_FALSE(tPerformer.empty());
    PlatoTestXMLGenerator::test_children({"Name", "Code", "PerformerID"}, {"platomain_1", "platomain", "0"}, tPerformer);

    auto tPerformerForNode = tPerformer.next_sibling("For");
    ASSERT_FALSE(tPerformerForNode.empty());
    PlatoTestXMLGenerator::test_attributes({"var", "in"}, {"E", "Performers"}, tPerformerForNode);
    auto tInnerPerformer = tPerformerForNode.child("Performer");
    ASSERT_FALSE(tInnerPerformer.empty());
    PlatoTestXMLGenerator::test_children({"Name", "Code", "PerformerID"}, {"plato_services_{E}", "plato_services", "{E+1}"}, tInnerPerformer);

    tInnerPerformer = tInnerPerformer.next_sibling("Performer");
    ASSERT_TRUE(tInnerPerformer.empty());

    tPerformer = tPerformer.next_sibling("Performer");
    ASSERT_TRUE(tPerformer.empty());

    // SHARED DATA
    auto tSharedDataForNode = tPerformerForNode.next_sibling("For");
    ASSERT_FALSE(tSharedDataForNode.empty());
    auto tInnerSharedData = tSharedDataForNode.child("SharedData");
    ASSERT_FALSE(tInnerSharedData.empty());
    std::vector<std::string> tKeys = {"Name",
        "Type", 
        "Layout",
        "Size", 
        "OwnerName",
        "UserName",
        "UserName"};
    std::vector<std::string> tValues = {"design_parameters_{E}", 
        "Scalar", 
        "Global", 
        "3",
        "platomain_1",
        "platomain_1",
        "plato_services_{E}"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInnerSharedData);
    tInnerSharedData = tInnerSharedData.next_sibling("SharedData");
    ASSERT_TRUE(tInnerSharedData.empty());

    tSharedDataForNode = tSharedDataForNode.next_sibling("For");
    ASSERT_FALSE(tSharedDataForNode.empty());
    tInnerSharedData = tSharedDataForNode.child("SharedData");
    ASSERT_FALSE(tInnerSharedData.empty());
    tKeys = {"Name",
        "Type", 
        "Layout",
        "Size", 
        "OwnerName",
        "UserName"};
    tValues = {"criterion_X_service_X_scenario_X_{E}", 
        "Scalar", 
        "Global", 
        "1",
        "plato_services_{E}",
        "platomain_1"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInnerSharedData);
    tInnerSharedData = tInnerSharedData.next_sibling("SharedData");
    ASSERT_TRUE(tInnerSharedData.empty());

    // INITIALIZE STAGE
    auto tStage = tDocument.child("Stage");
    ASSERT_FALSE(tStage.empty());
    ASSERT_STREQ("Stage", tStage.name());
    auto tName = tStage.child("Name");
    ASSERT_STREQ("Initialize Input", tName.child_value());
    auto tForNode = tStage.child("For");
    PlatoTestXMLGenerator::test_attributes({"var", "in"}, {"E", "Parameters"}, tForNode);
    auto tInputs = tForNode.child("Input");
    ASSERT_FALSE(tInputs.empty());
    ASSERT_STREQ("Input", tInputs.name());
    PlatoTestXMLGenerator::test_children({"SharedDataName"}, {"design_parameters_{E}"}, tInputs);

    auto tOuterOperation = tStage.child("Operation");
    ASSERT_FALSE(tOuterOperation.empty());
    tForNode = tOuterOperation.child("For");
    ASSERT_FALSE(tForNode.empty());
    PlatoTestXMLGenerator::test_attributes({"var", "in"}, {"E", "Parameters"}, tForNode);
    auto tOperation = tForNode.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    tKeys = {"Name", "PerformerName", "Input"};
    tValues = {"aprepro_{E}", "plato_services_{E}", ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);
    auto tOpInputs = tOperation.child("Input");
    ASSERT_FALSE(tOpInputs.empty());
    PlatoTestXMLGenerator::test_children({"ArgumentName", "SharedDataName"}, {"parameters", "design_parameters_{E}"}, tOpInputs);

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());

    tOuterOperation = tOuterOperation.next_sibling("Operation");
    ASSERT_TRUE(tOuterOperation.empty());

    // CRITERION 0 STAGE
    tStage = tStage.next_sibling("Stage");
    ASSERT_FALSE(tStage.empty());
    ASSERT_STREQ("Stage", tStage.name());
    tName = tStage.child("Name");
    ASSERT_STREQ("Compute Criterion 0 Value", tName.child_value());

    tOuterOperation = tStage.child("Operation");
    ASSERT_FALSE(tOuterOperation.empty());
    tForNode = tOuterOperation.child("For");
    ASSERT_FALSE(tForNode.empty());
    PlatoTestXMLGenerator::test_attributes({"var", "in"}, {"E", "Parameters"}, tForNode);
    tOperation = tForNode.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    tKeys = {"Name", "PerformerName"};
    tValues = {"gemma_{E}", "plato_services_{E}"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());

    tOuterOperation = tOuterOperation.next_sibling("Operation");
    ASSERT_FALSE(tOuterOperation.empty());
    tForNode = tOuterOperation.child("For");
    ASSERT_FALSE(tForNode.empty());
    PlatoTestXMLGenerator::test_attributes({"var", "in"}, {"E", "Parameters"}, tForNode);
    tOperation = tForNode.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    tKeys = {"Name", "PerformerName"};
    tValues = {"wait_{E}", "plato_services_{E}"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());

    tOuterOperation = tOuterOperation.next_sibling("Operation");
    ASSERT_FALSE(tOuterOperation.empty());
    tForNode = tOuterOperation.child("For");
    ASSERT_FALSE(tForNode.empty());
    PlatoTestXMLGenerator::test_attributes({"var", "in"}, {"E", "Parameters"}, tForNode);
    tOperation = tForNode.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    tKeys = {"Name", "PerformerName", "Output"};
    tValues = {"harvest_data_{E}", "plato_services_{E}", ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);
    auto tOpOutputs = tOperation.child("Output");
    ASSERT_FALSE(tOpOutputs.empty());
    PlatoTestXMLGenerator::test_children({"ArgumentName", "SharedDataName"}, {"criterion value", "criterion_X_service_X_scenario_X_{E}"}, tOpOutputs);

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());

    tForNode = tOuterOperation.next_sibling("For");
    ASSERT_FALSE(tForNode.empty());
    PlatoTestXMLGenerator::test_attributes({"var", "in"}, {"E", "Parameters"}, tForNode);
    auto tOutputs = tForNode.child("Output");
    ASSERT_FALSE(tOutputs.empty());
    PlatoTestXMLGenerator::test_children({"SharedDataName"}, {"criterion_X_service_X_scenario_X_{E}"}, tOutputs);

    tForNode = tForNode.next_sibling("For");
    ASSERT_TRUE(tForNode.empty());

    tStage = tStage.next_sibling("Stage");
    ASSERT_TRUE(tStage.empty());

    // DAKOTA DRIVER
    auto tDriver = tDocument.child("DakotaDriver");
    ASSERT_FALSE(tDriver.empty());
    ASSERT_STREQ("DakotaDriver", tDriver.name());
    tStage = tDriver.child("Stage");
    ASSERT_FALSE(tStage.empty());
    tKeys = {"StageTag", "StageName", "For"};
    tValues = {"initialize", "Initialize Input", ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tStage);
    tForNode = tStage.child("For");
    ASSERT_FALSE(tForNode.empty());
    PlatoTestXMLGenerator::test_attributes({"var", "in"}, {"E", "Parameters"}, tForNode);
    tInputs = tForNode.child("Input");
    ASSERT_FALSE(tInputs.empty());
    tKeys = {"Tag", "SharedDataName"};
    tValues = {"continuous", "design_parameters_{E}"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInputs);

    tInputs = tInputs.next_sibling("Input");
    ASSERT_TRUE(tInputs.empty());

    tForNode = tForNode.next_sibling("For");
    ASSERT_TRUE(tForNode.empty());

    tStage = tStage.next_sibling("Stage");
    ASSERT_FALSE(tStage.empty());
    tKeys = {"StageTag", "StageName", "For"};
    tValues = {"criterion_value_0", "Compute Criterion 0 Value", ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tStage);
    tForNode = tStage.child("For");
    ASSERT_FALSE(tForNode.empty());
    PlatoTestXMLGenerator::test_attributes({"var", "in"}, {"E", "Parameters"}, tForNode);
    tOutputs = tForNode.child("Output");
    ASSERT_FALSE(tOutputs.empty());
    tKeys = {"SharedDataName"};
    tValues = {"criterion_X_service_X_scenario_X_{E}"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOutputs);

    tOutputs = tOutputs.next_sibling("Output");
    ASSERT_TRUE(tOutputs.empty());

    tForNode = tForNode.next_sibling("For");
    ASSERT_TRUE(tForNode.empty());

    tStage = tStage.next_sibling("Stage");
    ASSERT_TRUE(tStage.empty());
}

TEST(PlatoTestXMLGenerator, WriteGemmaMPIRunFile)
{
    XMLGen::InputData tInputMetaData;

    // define criteria
    XMLGen::Criterion tCriterion;
    tCriterion.id("2");
    tCriterion.type("system_call");
    tCriterion.append("data_group", "1");
    tCriterion.append("data_extraction_operation", "max");
    tCriterion.append("data_file", "matched_power_balance.dat");
    tInputMetaData.append(tCriterion);

    // define objective 
    XMLGen::Objective tObjective;
    tObjective.criteriaIDs.push_back("2");
    tInputMetaData.objective = tObjective;

    // define optimization parameters 
    XMLGen::OptimizationParameters tOptParams;
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    tOptParams.descriptors(tDescriptors);
    tOptParams.append("concurrent_evaluations", "2");
    tInputMetaData.set(tOptParams);

    // define services
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tServiceOne.append("type", "system_call");
    tServiceOne.append("number_processors", "13");
    tInputMetaData.append(tServiceOne);
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "platomain");
    tServiceTwo.append("id", "2");
    tServiceTwo.append("type", "plato_app");
    tServiceTwo.append("number_processors", "2");
    tInputMetaData.append(tServiceTwo);

    XMLGen::XMLGeneratorGemmaProblem tGemmaProblem(tInputMetaData);

    tGemmaProblem.write_mpisource("mpifile");

    std::string tLine;
    std::ifstream tInFile("mpifile");

    EXPECT_NO_THROW(std::getline(tInFile,tLine));
    ASSERT_EQ("mpirun --oversubscribe -np 1 -x PLATO_PERFORMER_ID=0 \\",tLine);
    EXPECT_NO_THROW(std::getline(tInFile,tLine));
    ASSERT_EQ("-x PLATO_INTERFACE_FILE=interface.xml \\",tLine);
    EXPECT_NO_THROW(std::getline(tInFile,tLine));
    ASSERT_EQ("-x PLATO_APP_FILE=plato_main_operations.xml \\",tLine);
    EXPECT_NO_THROW(std::getline(tInFile,tLine));
    ASSERT_EQ("PlatoMain plato_main_input_deck.xml \\",tLine);
    
    EXPECT_NO_THROW(std::getline(tInFile,tLine));
    ASSERT_EQ(": -np 13 -x PLATO_PERFORMER_ID=1 \\",tLine);
    EXPECT_NO_THROW(std::getline(tInFile,tLine));
    ASSERT_EQ("-x PLATO_INTERFACE_FILE=interface.xml \\",tLine);
    EXPECT_NO_THROW(std::getline(tInFile,tLine));
    ASSERT_EQ("-x PLATO_APP_FILE=plato_main_operations.xml \\",tLine);
    EXPECT_NO_THROW(std::getline(tInFile,tLine));
    ASSERT_EQ("PlatoEngineServices plato_main_input_deck.xml \\",tLine);
    
    EXPECT_NO_THROW(std::getline(tInFile,tLine));
    ASSERT_EQ(": -np 13 -x PLATO_PERFORMER_ID=2 \\",tLine);
    EXPECT_NO_THROW(std::getline(tInFile,tLine));
    ASSERT_EQ("-x PLATO_INTERFACE_FILE=interface.xml \\",tLine);
    EXPECT_NO_THROW(std::getline(tInFile,tLine));
    ASSERT_EQ("-x PLATO_APP_FILE=plato_main_operations.xml \\",tLine);
    EXPECT_NO_THROW(std::getline(tInFile,tLine));
    ASSERT_EQ("PlatoEngineServices plato_main_input_deck.xml \\",tLine);
    
    std::getline(tInFile,tLine);
    EXPECT_TRUE(tInFile.eof());

    tInFile.close();
    Plato::system("rm -f mpifile");

}


TEST(PlatoTestXMLGenerator, WriteGemmaDefinesFile)
{
    XMLGen::InputData tInputMetaData;

    // define criteria
    XMLGen::Criterion tCriterion;
    tCriterion.id("2");
    tCriterion.type("system_call");
    tCriterion.append("data_group", "1");
    tCriterion.append("data_extraction_operation", "max");
    tCriterion.append("data_file", "matched_power_balance.dat");
    tInputMetaData.append(tCriterion);

    // define objective 
    XMLGen::Objective tObjective;
    tObjective.criteriaIDs.push_back("2");
    tInputMetaData.objective = tObjective;

    // define optimization parameters 
    XMLGen::OptimizationParameters tOptParams;
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    tOptParams.descriptors(tDescriptors);
    tOptParams.append("concurrent_evaluations", "2");
    tInputMetaData.set(tOptParams);

    // define services
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tServiceOne.append("type", "system_call");
    tServiceOne.append("number_processors", "1");
    tInputMetaData.append(tServiceOne);
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "platomain");
    tServiceTwo.append("id", "2");
    tServiceTwo.append("type", "plato_app");
    tServiceTwo.append("number_processors", "2");
    tInputMetaData.append(tServiceTwo);

    XMLGen::XMLGeneratorGemmaProblem tGemmaProblem(tInputMetaData);

    tGemmaProblem.write_defines();

    std::string tLine;
    std::ifstream tInFile("defines.xml");

    EXPECT_NO_THROW(std::getline(tInFile,tLine));
    ASSERT_EQ("<?xml version=\"1.0\"?>",tLine);
    
    EXPECT_NO_THROW(std::getline(tInFile,tLine));
    ASSERT_EQ("<Array name=\"Parameters\" type=\"int\" from=\"0\" to=\"2\" />",tLine);
    
    EXPECT_NO_THROW(std::getline(tInFile,tLine));
    ASSERT_EQ("<Array name=\"Performers\" type=\"int\" from=\"0\" to=\"1\" />",tLine);
    
    std::getline(tInFile,tLine);
    EXPECT_TRUE(tInFile.eof());
    tInFile.close();
    //Plato::system("rm -f defines.xml");
}



TEST(PlatoTestXMLGenerator, WriteGemmaPlatoMainInput)
{
    XMLGen::InputData tInputMetaData;

    // define criteria
    XMLGen::Criterion tCriterion;
    tCriterion.id("2");
    tCriterion.type("system_call");
    tCriterion.append("data_group", "1");
    tCriterion.append("data_extraction_operation", "max");
    tCriterion.append("data_file", "matched_power_balance.dat");
    tInputMetaData.append(tCriterion);

    // define objective 
    XMLGen::Objective tObjective;
    tObjective.criteriaIDs.push_back("2");
    tInputMetaData.objective = tObjective;

    // define optimization parameters 
    XMLGen::OptimizationParameters tOptParams;
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    tOptParams.descriptors(tDescriptors);
    tOptParams.append("concurrent_evaluations", "2");
    tInputMetaData.set(tOptParams);

    // define services
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tServiceOne.append("type", "system_call");
    tServiceOne.append("number_processors", "1");
    tInputMetaData.append(tServiceOne);
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "platomain");
    tServiceTwo.append("id", "2");
    tServiceTwo.append("type", "plato_app");
    tServiceTwo.append("number_processors", "2");
    tInputMetaData.append(tServiceTwo);

    XMLGen::XMLGeneratorGemmaProblem tGemmaProblem(tInputMetaData);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tGemmaProblem.write_plato_main_input(tDocument));
    ASSERT_FALSE(tDocument.empty());

    tDocument.save_file("main.xml", "  ");

    auto tOutput = tDocument.child("output");
    ASSERT_FALSE(tOutput.empty());
    
    std::vector<std::string> tKeys = {"file", "format"};
    std::vector<std::string> tValues = {"platomain", "exodus"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOutput);

    tOutput = tOutput.next_sibling("output");
    ASSERT_TRUE(tOutput.empty());
}


TEST(PlatoTestXMLGenerator, move_file_to_subdirectories)
{
    std::ofstream tOutFile;
    tOutFile.open("dummy.txt", std::ofstream::out | std::ofstream::trunc);
    tOutFile << "hello";
    tOutFile.close();

    XMLGen::move_file_to_subdirectories("dummy.txt", {"dummy_dir"});

    auto tReadData = XMLGen::read_data_from_file("./dummy_dir/dummy.txt");
    auto tGoldString = std::string("hello");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
    Plato::system("rm -rf dummy_dir");
    Plato::system("rm -f dummy.txt");
}

TEST(PlatoTestXMLGenerator, get_criterion_value_argument_metadata)
{
    XMLGen::InputData tInputMetaData;

    XMLGen::OperationMetaData tOperationMetaData;
    tOperationMetaData.append("concurrent_evaluations", "2");
    tOperationMetaData.append("objective_criteria_ids", "10");

    auto tSharedDataArgs = XMLGen::get_criterion_value_argument_metadata("objective", tOperationMetaData);
    EXPECT_EQ(2u, tSharedDataArgs.size());
    EXPECT_STREQ("1", tSharedDataArgs[0].get("size").c_str());
    EXPECT_STREQ("objective value for criterion 10 and evaluation 0", tSharedDataArgs[0].get("name").c_str());
    EXPECT_STREQ("output", tSharedDataArgs[0].get("type").c_str());
    EXPECT_STREQ("scalar", tSharedDataArgs[0].get("layout").c_str());

    EXPECT_STREQ("1", tSharedDataArgs[1].get("size").c_str());
    EXPECT_STREQ("objective value for criterion 10 and evaluation 1", tSharedDataArgs[1].get("name").c_str());
    EXPECT_STREQ("output", tSharedDataArgs[1].get("type").c_str());
    EXPECT_STREQ("scalar", tSharedDataArgs[1].get("layout").c_str());
}

TEST(PlatoTestXMLGenerator, get_criteria_ids)
{
    XMLGen::InputData tInputMetaData;
    // define criterion
    XMLGen::Criterion tCriterionOne;
    tCriterionOne.id("1");
    tCriterionOne.type("volume");
    tInputMetaData.append(tCriterionOne);
    XMLGen::Criterion tCriterionTwo;
    tCriterionTwo.id("2");
    tCriterionTwo.type("system_call");
    tInputMetaData.append(tCriterionTwo);
    XMLGen::Criterion tCriterionThree;
    tCriterionThree.id("3");
    tCriterionThree.type("mass");
    tInputMetaData.append(tCriterionThree);

    auto tIDs = XMLGen::get_criteria_ids("system_call", tInputMetaData);
    EXPECT_EQ(1u, tIDs.size());
    EXPECT_STREQ("2", tIDs.front().c_str());
}

TEST(PlatoTestXMLGenerator, append_general_harvest_data_from_file_operation_options)
{
    XMLGen::InputData tMetaData;
    // define optimization parameters 
    XMLGen::OptimizationParameters tOptParams;
    tOptParams.append("concurrent_evaluations", "2");
    tMetaData.set(tOptParams);
    // call function
    XMLGen::OperationMetaData tOperationMetaData;
    XMLGen::append_general_harvest_data_from_file_operation_options(tMetaData, tOperationMetaData);
    EXPECT_STREQ("2", tOperationMetaData.get("concurrent_evaluations").front().c_str());
    EXPECT_EQ(2u, tOperationMetaData.get("names").size());
    EXPECT_STREQ("harvest_data_0", tOperationMetaData.get("names")[0].c_str());
    EXPECT_STREQ("harvest_data_1", tOperationMetaData.get("names")[1].c_str());
}

TEST(PlatoTestXMLGenerator, write_run_app_via_system_call_operation)
{
    pugi::xml_document tDocument;
    XMLGen::OperationMetaData tOperationMetaData;
    
    tOperationMetaData.append("concurrent_evaluations", "2");
    tOperationMetaData.append("names", "gemma_0");
    tOperationMetaData.append("names", "gemma_1");
    tOperationMetaData.append("commands", "curl");
    tOperationMetaData.append("commands", "curl");
    tOperationMetaData.append("functions", "SystemCall");
    tOperationMetaData.append("functions", "SystemCall");
    tOperationMetaData.append("subdirectories", "evaluation_0");
    tOperationMetaData.append("subdirectories", "evaluation_1");
    tOperationMetaData.append("arguments", "http://localhost:7000/run_gemma/");
    tOperationMetaData.append("arguments", "http://localhost:7001/run_gemma/");

    XMLGen::write_run_app_via_system_call_operation_to_file(tOperationMetaData, tDocument);
    tDocument.save_file("dummy.txt", " ");

    auto tReadData = XMLGen::read_data_from_file("dummy.txt");
    auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>SystemCall</Function><Name>gemma_0</Name><Command>curl</Command><OnChange>true</OnChange><AppendInput>false</AppendInput><ChDir>evaluation_0</ChDir><Argument>http://localhost:7000/run_gemma/</Argument></Operation><Operation><Function>SystemCall</Function><Name>gemma_1</Name><Command>curl</Command><OnChange>true</OnChange><AppendInput>false</AppendInput><ChDir>evaluation_1</ChDir><Argument>http://localhost:7001/run_gemma/</Argument></Operation>");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
    Plato::system("rm -f dummy.txt");
}

TEST(PlatoTestXMLGenerator, append_run_system_call_shared_data_arguments)
{
    // use case 1: base local host number is not defined, use default value of 7000
    XMLGen::OperationMetaData tOperationMetaDataOne;
    tOperationMetaDataOne.append("algorithm", "matched_power_balance");
    tOperationMetaDataOne.append("type", "web_app");
    tOperationMetaDataOne.append("type", "web_app");
    tOperationMetaDataOne.append("executables", "gemma");
    tOperationMetaDataOne.append("executables", "gemma");
    XMLGen::append_run_system_call_shared_data_arguments(tOperationMetaDataOne);
    // test web_app arguments
    EXPECT_EQ(2u, tOperationMetaDataOne.get("arguments").size());
    EXPECT_STREQ("http://localhost:7000/run_gemma/", tOperationMetaDataOne.get("arguments").front().c_str());
    EXPECT_STREQ("http://localhost:7001/run_gemma/", tOperationMetaDataOne.get("arguments").back().c_str());

    // use case 2: base local host number is defined
    XMLGen::OperationMetaData tOperationMetaDataTwo;
    tOperationMetaDataTwo.append("base_web_port_number", "8000");
    tOperationMetaDataTwo.append("algorithm", "matched_power_balance");
    tOperationMetaDataTwo.append("type", "web_app");
    tOperationMetaDataTwo.append("type", "web_app");
    tOperationMetaDataTwo.append("type", "web_app");
    tOperationMetaDataTwo.append("executables", "gemma");
    tOperationMetaDataTwo.append("executables", "gemma");
    tOperationMetaDataTwo.append("executables", "gemma");
    XMLGen::append_run_system_call_shared_data_arguments(tOperationMetaDataTwo);
    // test web_app arguments
    EXPECT_EQ(3u, tOperationMetaDataTwo.get("arguments").size());
    EXPECT_STREQ("http://localhost:8000/run_gemma/", tOperationMetaDataTwo.get("arguments")[0].c_str());
    EXPECT_STREQ("http://localhost:8001/run_gemma/", tOperationMetaDataTwo.get("arguments")[1].c_str());
    EXPECT_STREQ("http://localhost:8002/run_gemma/", tOperationMetaDataTwo.get("arguments")[2].c_str());
}

TEST(PlatoTestXMLGenerator, set_general_run_app_via_system_call_options)
{
    XMLGen::InputData tMetaData;
    // define service
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tServiceOne.append("type", "web_app");
    tServiceOne.append("number_processors", "1");
    tMetaData.append(tServiceOne);
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "platomain");
    tServiceTwo.append("id", "2");
    tServiceTwo.append("type", "plato_app");
    tServiceTwo.append("number_processors", "2");
    tMetaData.append(tServiceTwo);
    // define optimization parameters 
    XMLGen::OptimizationParameters tOptParams;
    tOptParams.append("concurrent_evaluations", "3");
    tMetaData.set(tOptParams);

    XMLGen::OperationMetaData tOperationMetaData;
    tOperationMetaData.append("code", "gemma");
    EXPECT_NO_THROW(XMLGen::set_general_run_app_via_system_call_options(tMetaData, tOperationMetaData));

    // test output
    auto tKeys = tOperationMetaData.keys();
    EXPECT_EQ(4u, tKeys.size());
    std::vector<std::string> tGoldKeys = {"number_processors", "type", "code", "concurrent_evaluations"};
    for(auto tKey : tKeys)
    {
        EXPECT_TRUE(std::find(tGoldKeys.begin(), tGoldKeys.end(), tKey) != tGoldKeys.end());
    }
    // test concurrent_evaluations
    EXPECT_EQ(1u, tOperationMetaData.get("concurrent_evaluations").size());
    EXPECT_STREQ("3", tOperationMetaData.get("concurrent_evaluations").front().c_str());    

    // test number processors
    EXPECT_EQ(3u, tOperationMetaData.get("number_processors").size());
    EXPECT_STREQ("1", tOperationMetaData.get("number_processors")[0].c_str());
    EXPECT_STREQ("1", tOperationMetaData.get("number_processors")[1].c_str());
    EXPECT_STREQ("1", tOperationMetaData.get("number_processors")[2].c_str());

    // test type
    EXPECT_EQ(3u, tOperationMetaData.get("type").size());
    EXPECT_STREQ("web_app", tOperationMetaData.get("type")[0].c_str());
    EXPECT_STREQ("web_app", tOperationMetaData.get("type")[1].c_str());
    EXPECT_STREQ("web_app", tOperationMetaData.get("type")[2].c_str());
}

TEST(PlatoTestXMLGenerator, check_app_service_type)
{
    EXPECT_THROW(XMLGen::check_app_service_type("plato_app"), std::runtime_error);
    EXPECT_NO_THROW(XMLGen::check_app_service_type("web_app"));
    EXPECT_NO_THROW(XMLGen::check_app_service_type("system_call"));
}

TEST(PlatoTestXMLGenerator, are_aprepro_input_options_defined)
{
    XMLGen::OperationMetaData tOperationMetaData1;
    tOperationMetaData1.append("concurrent_evaluations", "2");
    tOperationMetaData1.set("names", {"aprepro_0", "aprepro_1"});
    tOperationMetaData1.set("options", {"length=", "depth=", "width="});
    tOperationMetaData1.set("commands", std::vector<std::string>(2u, "aprepro"));
    tOperationMetaData1.set("functions", std::vector<std::string>(2u, "SystemCall"));
    tOperationMetaData1.set("arguments", {"-q", "matched.yaml.template", "matched.yaml"});
    EXPECT_NO_THROW(XMLGen::are_aprepro_input_options_defined(tOperationMetaData1));

    XMLGen::OperationMetaData tOperationMetaData2;
    tOperationMetaData2.set("names", {"aprepro_0", "aprepro_1"});
    tOperationMetaData2.set("options", {"length=", "depth=", "width="});
    tOperationMetaData2.set("commands", std::vector<std::string>(2u, "aprepro"));
    tOperationMetaData2.set("functions", std::vector<std::string>(2u, "SystemCall"));
    tOperationMetaData2.set("arguments", {"-q", "matched.yaml.template", "matched.yaml"});
    EXPECT_THROW(XMLGen::are_aprepro_input_options_defined(tOperationMetaData2), std::runtime_error);
}

TEST(PlatoTestXMLGenerator, get_service_indices)
{
    XMLGen::InputData tMetaData;
    // define service
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tMetaData.append(tServiceOne);
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "platomain");
    tServiceTwo.append("id", "2");
    tMetaData.append(tServiceTwo);

    // test gemma indices
    auto tIndices = XMLGen::get_service_indices("gemma", tMetaData);
    EXPECT_EQ(1u, tIndices.size());
    EXPECT_EQ(0, tIndices.front());

    // test plato indices
    tIndices = XMLGen::get_service_indices("PLATOMAIN", tMetaData);
    EXPECT_EQ(1u, tIndices.size());
    EXPECT_EQ(1, tIndices.front());
}

TEST(PlatoTestXMLGenerator, get_unique_scenario_ids)
{
    XMLGen::InputData tMetaData;

    // TEST 1: target scenario defined
    // define objective
    XMLGen::Objective tObjective;
    tObjective.type = "weighted_sum";
    tObjective.criteriaIDs.push_back("1");
    tObjective.serviceIDs.push_back("1");
    tObjective.scenarioIDs.push_back("1");
    tMetaData.objective = tObjective;

    // define service
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tMetaData.append(tServiceOne);

    // call get_unique_scenario_ids
    auto tUniqueGemmaScenarioIDs = XMLGen::get_unique_scenario_ids("gemma", tMetaData);
    EXPECT_EQ(1u, tUniqueGemmaScenarioIDs.size());
    EXPECT_STREQ("1", tUniqueGemmaScenarioIDs.front().c_str());
    auto tUniquePAScenarioIDs = XMLGen::get_unique_scenario_ids("plato_analyze", tMetaData);
    EXPECT_TRUE(tUniquePAScenarioIDs.empty());

    // TEST 2: one target scenario and two distinct criteria, scenarios & services defined 
    // define objective: add new criteria, scenario, and service info to objective
    tMetaData.objective.criteriaIDs.push_back("2");
    tMetaData.objective.serviceIDs.push_back("2");
    tMetaData.objective.scenarioIDs.push_back("2");
    EXPECT_EQ(2u, tMetaData.objective.criteriaIDs.size());

    // define objective
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "plato_analyze");
    tServiceTwo.append("id", "2");
    tMetaData.append(tServiceTwo);
    EXPECT_EQ(2u, tMetaData.services().size());

    // call get_unique_scenario_ids
    tUniqueGemmaScenarioIDs = XMLGen::get_unique_scenario_ids("gemma", tMetaData);
    EXPECT_EQ(1u, tUniqueGemmaScenarioIDs.size());
    EXPECT_STREQ("1", tUniqueGemmaScenarioIDs.front().c_str());
    tUniquePAScenarioIDs = XMLGen::get_unique_scenario_ids("plato_analyze", tMetaData);
    EXPECT_EQ(1u, tUniquePAScenarioIDs.size());
    EXPECT_STREQ("2", tUniquePAScenarioIDs.front().c_str());

    // TEST 3: one target scenario, two distinct services and three criterion defined
    tMetaData.objective.criteriaIDs.push_back("3");
    tMetaData.objective.serviceIDs.push_back("1");
    tMetaData.objective.scenarioIDs.push_back("1");
    EXPECT_EQ(3u, tMetaData.objective.criteriaIDs.size());
    EXPECT_EQ(2u, tMetaData.services().size());

    // call get_unique_scenario_ids
    tUniqueGemmaScenarioIDs = XMLGen::get_unique_scenario_ids("gemma", tMetaData);
    EXPECT_EQ(1u, tUniqueGemmaScenarioIDs.size());
    EXPECT_STREQ("1", tUniqueGemmaScenarioIDs.front().c_str());
    tUniquePAScenarioIDs = XMLGen::get_unique_scenario_ids("plato_analyze", tMetaData);
    EXPECT_EQ(1u, tUniquePAScenarioIDs.size());
    EXPECT_STREQ("2", tUniquePAScenarioIDs.front().c_str());

    // TEST 4: two distinct services, four criterion and three scenarios defined
    tMetaData.objective.criteriaIDs.push_back("4");
    tMetaData.objective.serviceIDs.push_back("1");
    tMetaData.objective.scenarioIDs.push_back("3");
    EXPECT_EQ(4u, tMetaData.objective.criteriaIDs.size());
    EXPECT_EQ(2u, tMetaData.services().size());

    // call get_unique_scenario_ids
    tUniqueGemmaScenarioIDs = XMLGen::get_unique_scenario_ids("gemma", tMetaData);
    EXPECT_EQ(2u, tUniqueGemmaScenarioIDs.size());
    EXPECT_STREQ("1", tUniqueGemmaScenarioIDs.front().c_str());
    EXPECT_STREQ("3", tUniqueGemmaScenarioIDs.back().c_str());
    tUniquePAScenarioIDs = XMLGen::get_unique_scenario_ids("plato_analyze", tMetaData);
    EXPECT_EQ(1u, tUniquePAScenarioIDs.size());
    EXPECT_STREQ("2", tUniquePAScenarioIDs.front().c_str());
}

// TEST(PlatoTestXMLGenerator, write_web_app_executable)
// {
//     // TODO: FINISH TEST
//     XMLGen::OperationMetaData tOperationMetaData;
//     tOperationMetaData.append("executables", "gemma");
//     tOperationMetaData.append("executables", "gemma");
//     tOperationMetaData.append("arguments", "gemma");
//     tOperationMetaData.append("arguments", "matched.yaml");
//     tOperationMetaData.append("web_port_numbers", "6002");
//     tOperationMetaData.append("web_port_numbers", "6003");
//     XMLGen::flask::write_web_app_executable("run_web_app", ".py", tOperationMetaData);

//     auto tReadData = XMLGen::read_data_from_file("./run_web_app_0.py");
//     auto tGoldString = std::string("fromflaskimportFlaskimportsubprocessapp=Flask(__name__)@app.route('/run_gemma/')defrun_gemma():tExitStatus=subprocess.call([\"gemma\",\"matched.yaml\"])returnstr(tExitStatus)if__name__=='__main__':app.run(host='0.0.0.0',port=6002)");
//     ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
//     Plato::system("rm -f run_web_app_0.py");

//     tReadData = XMLGen::read_data_from_file("./run_web_app_1.py");
//     tGoldString = std::string("fromflaskimportFlaskimportsubprocessapp=Flask(__name__)@app.route('/run_gemma/')defrun_gemma():tExitStatus=subprocess.call([\"gemma\",\"matched.yaml\"])returnstr(tExitStatus)if__name__=='__main__':app.run(host='0.0.0.0',port=6003)");
//     ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
//     Plato::system("rm -f run_web_app_1.py");
// }

// TEST(PlatoTestXMLGenerator, write_web_app_input_deck)
// {
//     XMLGen::InputData tInputMetaData;
//     // define optimization parameters 
//     XMLGen::OptimizationParameters tOptParams;
//     tOptParams.append("concurrent_evaluations", "2");
//     tInputMetaData.set(tOptParams);
//     // service parameters
//     XMLGen::Service tServiceOne;
//     tServiceOne.append("code", "gemma");
//     tServiceOne.append("id", "1");
//     tServiceOne.append("type", "web_app");
//     tServiceOne.append("number_processors", "1");
//     tInputMetaData.append(tServiceOne);
//     XMLGen::Service tServiceTwo;
//     tServiceTwo.append("code", "platomain");
//     tServiceTwo.append("id", "2");
//     tServiceTwo.append("type", "plato_app");
//     tServiceTwo.append("number_processors", "2");
//     tInputMetaData.append(tServiceTwo);

//     XMLGen::gemma::write_web_app_input_deck(tInputMetaData);

//     // test
//     auto tReadData = XMLGen::read_data_from_file("./gemma_evaluation_0/run_web_app_0.py");
//     auto tGoldString = std::string("fromflaskimportFlaskimportsubprocessapp=Flask(__name__)@app.route('/run_gemma/')defrun_gemma():tExitStatus=subprocess.call([\"gemma\",\"gemma_matched_power_balance_input_deck.yaml\"])returnstr(tExitStatus)if__name__=='__main__':app.run(host='0.0.0.0',port=7000)");
//     ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
//     Plato::system("rm -rf gemma_evaluation_0/");

//     tReadData = XMLGen::read_data_from_file("./gemma_evaluation_1/run_web_app_1.py");
//     tGoldString = std::string("fromflaskimportFlaskimportsubprocessapp=Flask(__name__)@app.route('/run_gemma/')defrun_gemma():tExitStatus=subprocess.call([\"gemma\",\"gemma_matched_power_balance_input_deck.yaml\"])returnstr(tExitStatus)if__name__=='__main__':app.run(host='0.0.0.0',port=7001)");
//     ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
//     Plato::system("rm -rf gemma_evaluation_1/");
// }

// TEST(PlatoTestXMLGenerator, AppendApreproSystemCallOperationsToPlatoMainOperationsFile)
// {
//     // use case: gemma call is defined by a system call mpi operation
//     XMLGen::InputData tInputMetaData;
//     // define criterion
//     XMLGen::Criterion tCriterionOne;
//     tCriterionOne.id("1");
//     tCriterionOne.type("volume");
//     tInputMetaData.append(tCriterionOne);
//     XMLGen::Criterion tCriterionTwo;
//     tCriterionTwo.id("2");
//     tCriterionTwo.type("system_call");
//     tCriterionTwo.append("data_group", "column_1");
//     tCriterionTwo.append("data_extraction_operation", "max");
//     tCriterionTwo.append("data_file", "matched_power_balance.dat");
//     tInputMetaData.append(tCriterionTwo);
//     XMLGen::Criterion tCriterionThree;
//     tCriterionThree.id("3");
//     tCriterionThree.type("system_call");
//     tCriterionThree.append("data_group", "column_2");
//     tCriterionThree.append("data_extraction_operation", "max");
//     tInputMetaData.append(tCriterionThree);
//     // define objective
//     XMLGen::Objective tObjective;
//     tObjective.criteriaIDs.push_back("1");
//     tObjective.criteriaIDs.push_back("2");
//     tInputMetaData.objective = tObjective;
//     // define constraints
//     XMLGen::Constraint tConstraint;
//     tConstraint.criterion("3");
//     tInputMetaData.constraints.push_back(tConstraint);
//     // define optimization parameters 
//     XMLGen::OptimizationParameters tOptParams;
//     std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
//     tOptParams.descriptors(tDescriptors);
//     tOptParams.append("concurrent_evaluations", "2");
//     tInputMetaData.set(tOptParams);
//     // service parameters
//     XMLGen::Service tServiceOne;
//     tServiceOne.append("code", "gemma");
//     tServiceOne.append("id", "1");
//     tServiceOne.append("type", "system_call");
//     tServiceOne.append("number_processors", "1");
//     tInputMetaData.append(tServiceOne);
//     XMLGen::Service tServiceTwo;
//     tServiceTwo.append("code", "platomain");
//     tServiceTwo.append("id", "2");
//     tServiceTwo.append("type", "plato_app");
//     tServiceTwo.append("number_processors", "2");
//     tInputMetaData.append(tServiceTwo);
      
//     pugi::xml_document tDocument;
//     ASSERT_NO_THROW(XMLGen::matched_power_balance::write_aprepro_system_call_operation("evaluations", tInputMetaData, tDocument));
//     ASSERT_FALSE(tDocument.empty());

//     // TEST RESULTS AGAINST GOLD VALUES
//     auto tOperation = tDocument.child("Operation");
//     ASSERT_FALSE(tOperation.empty());
//     ASSERT_STREQ("Operation", tOperation.name());
//     std::vector<std::string> tKeys = {"Function", "Name", 
//         "Command", 
//         "ChDir",
//         "OnChange", 
//         "Argument", "Argument", "Argument",
//         "AppendInput",
//         "Option","Option","Option",
//          "Input"};
//     std::vector<std::string> tValues = {"SystemCall", "aprepro_0", 
//         "aprepro", 
//         "evaluations_0/",
//         "true", 
//         "-q", "gemma_matched_power_balance_input_deck.yaml.template", "gemma_matched_power_balance_input_deck.yaml",
//         "true",
//         "slot_length", "slot_width", "slot_depth",
//         ""};
//     PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);
//     auto tInput = tOperation.child("Input");
//     ASSERT_FALSE(tInput.empty());
//     tKeys = {"ArgumentName","Layout","Size"};
//     tValues = {"parameters_0","scalar","3"};
//     PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);

//     tOperation = tOperation.next_sibling("Operation");
//     ASSERT_FALSE(tOperation.empty());
//     ASSERT_STREQ("Operation", tOperation.name());

//     tKeys = {"Function", "Name", 
//         "Command", 
//         "ChDir",
//         "OnChange", 
//         "Argument", "Argument", "Argument",
//         "AppendInput",
//         "Option","Option","Option",
//          "Input"};
//     tValues = {"SystemCall", "aprepro_1", 
//         "aprepro", 
//         "evaluations_1/",
//         "true", 
//         "-q", "gemma_matched_power_balance_input_deck.yaml.template", "gemma_matched_power_balance_input_deck.yaml",
//         "true",
//         "slot_length", "slot_width", "slot_depth",
//         ""};

//     PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);
//     tInput = tOperation.child("Input");
//     ASSERT_FALSE(tInput.empty());
//     tKeys = {"ArgumentName","Layout","Size"};
//     tValues = {"parameters_1","scalar","3"};
//     PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);

//     tOperation = tOperation.next_sibling("Operation");
//     ASSERT_TRUE(tOperation.empty());
// }

// TEST(PlatoTestXMLGenerator, AppendGemmaMPISystemCallOperationsToPlatoMainOperationsFile)
// {
//     // use case: gemma call is defined by a system call mpi operation
//     XMLGen::InputData tInputMetaData;
//     // define criterion
//     XMLGen::Criterion tCriterionOne;
//     tCriterionOne.id("1");
//     tCriterionOne.type("volume");
//     tInputMetaData.append(tCriterionOne);
//     XMLGen::Criterion tCriterionTwo;
//     tCriterionTwo.id("2");
//     tCriterionTwo.type("system_call");
//     tCriterionTwo.append("data_group", "column_1");
//     tCriterionTwo.append("data_extraction_operation", "max");
//     tCriterionTwo.append("data_file", "matched_power_balance.dat");
//     tInputMetaData.append(tCriterionTwo);
//     XMLGen::Criterion tCriterionThree;
//     tCriterionThree.id("3");
//     tCriterionThree.type("system_call");
//     tCriterionThree.append("data_group", "column_2");
//     tCriterionThree.append("data_extraction_operation", "max");
//     tInputMetaData.append(tCriterionThree);
//     // define objective
//     XMLGen::Objective tObjective;
//     tObjective.criteriaIDs.push_back("1");
//     tObjective.criteriaIDs.push_back("2");
//     tInputMetaData.objective = tObjective;
//     // define constraints
//     XMLGen::Constraint tConstraint;
//     tConstraint.criterion("3");
//     tInputMetaData.constraints.push_back(tConstraint);
//     // define optimization parameters 
//     XMLGen::OptimizationParameters tOptParams;
//     std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
//     tOptParams.descriptors(tDescriptors);
//     tOptParams.append("concurrent_evaluations", "2");
//     tInputMetaData.set(tOptParams);
//     // service parameters
//     XMLGen::Service tServiceOne;
//     tServiceOne.append("code", "gemma");
//     tServiceOne.append("id", "1");
//     tServiceOne.append("type", "system_call");
//     tServiceOne.append("number_processors", "2");
//     tInputMetaData.append(tServiceOne);
//     XMLGen::Service tServiceTwo;
//     tServiceTwo.append("code", "platomain");
//     tServiceTwo.append("id", "2");
//     tServiceTwo.append("type", "plato_app");
//     tServiceTwo.append("number_processors", "2");
//     tInputMetaData.append(tServiceTwo);
     
//     pugi::xml_document tDocument;
//     ASSERT_NO_THROW(XMLGen::matched_power_balance::write_run_app_via_system_call_operation("evaluations", tInputMetaData, tDocument));
//     ASSERT_FALSE(tDocument.empty());

//     // TEST RESULTS AGAINST GOLD VALUES
//     auto tOperation = tDocument.child("Operation");
//     ASSERT_FALSE(tOperation.empty());
//     ASSERT_STREQ("Operation", tOperation.name());
//     std::vector<std::string> tKeys = {"Function", "Name", 
//         "Command", 
//         "ChDir",
//         "OnChange", 
//         "NumRanks",
//         "Argument", 
//         "AppendInput"
//         };
//     std::vector<std::string> tValues = {"SystemCallMPI", "gemma_0", 
//         "gemma", 
//         "evaluations_0/",
//         "true", 
//         "2", 
//         "gemma_matched_power_balance_input_deck.yaml",
//         "false"};
    
//     tOperation = tOperation.next_sibling("Operation");
//     ASSERT_FALSE(tOperation.empty());
//     ASSERT_STREQ("Operation", tOperation.name());

//     tValues = {"SystemCallMPI", "gemma_1", 
//         "gemma", 
//         "evaluations_1/",
//         "true", 
//         "2", 
//         "gemma_matched_power_balance_input_deck.yaml",
//         "false"};

//     tOperation = tOperation.next_sibling("Operation");
//     ASSERT_TRUE(tOperation.empty());
// }

// TEST(PlatoTestXMLGenerator, AppendHarvestSystemCallOperationsToPlatoMainOperationsFile)
// {
//     // use case: gemma call is defined by a system call mpi operation
//     XMLGen::InputData tInputMetaData;
//     // define criterion
//     XMLGen::Criterion tCriterionOne;
//     tCriterionOne.id("1");
//     tCriterionOne.type("volume");
//     tInputMetaData.append(tCriterionOne);
//     XMLGen::Criterion tCriterionTwo;
//     tCriterionTwo.id("2");
//     tCriterionTwo.type("system_call");
//     tCriterionTwo.append("data_group", "column_1");
//     tCriterionTwo.append("data_extraction_operation", "max");
//     tCriterionTwo.append("data_file", "matched_power_balance.dat");
//     tInputMetaData.append(tCriterionTwo);
//     XMLGen::Criterion tCriterionThree;
//     tCriterionThree.id("3");
//     tCriterionThree.type("system_call");
//     tCriterionThree.append("data_group", "column_2");
//     tCriterionThree.append("data_extraction_operation", "max");
//     tInputMetaData.append(tCriterionThree);
//     // define objective
//     XMLGen::Objective tObjective;
//     tObjective.criteriaIDs.push_back("1");
//     tObjective.criteriaIDs.push_back("2");
//     tInputMetaData.objective = tObjective;
//     // define constraints
//     XMLGen::Constraint tConstraint;
//     tConstraint.criterion("3");
//     tInputMetaData.constraints.push_back(tConstraint);
//     // define optimization parameters 
//     XMLGen::OptimizationParameters tOptParams;
//     std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
//     tOptParams.descriptors(tDescriptors);
//     tOptParams.append("concurrent_evaluations", "2");
//     tInputMetaData.set(tOptParams);
//     // service parameters
//     XMLGen::Service tServiceOne;
//     tServiceOne.append("code", "gemma");
//     tServiceOne.append("id", "1");
//     tServiceOne.append("type", "system_call");
//     tServiceOne.append("number_processors", "2");
//     tInputMetaData.append(tServiceOne);
//     XMLGen::Service tServiceTwo;
//     tServiceTwo.append("code", "platomain");
//     tServiceTwo.append("id", "2");
//     tServiceTwo.append("type", "plato_app");
//     tServiceTwo.append("number_processors", "2");
//     tInputMetaData.append(tServiceTwo);
    
//     pugi::xml_document tDocument;
//     ASSERT_NO_THROW(XMLGen::matched_power_balance::write_harvest_data_from_file_operation("evaluations", tInputMetaData, tDocument));
    
//     XMLGen::gemma::write_operation_file(tInputMetaData);
//     ASSERT_FALSE(tDocument.empty());

//     // TEST RESULTS AGAINST GOLD VALUES
//     auto tOperation = tDocument.child("Operation");
    
//     ASSERT_FALSE(tOperation.empty());
//     ASSERT_STREQ("Operation", tOperation.name());
//     std::vector<std::string> tKeys = {
//         "Function", 
//         "Name", 
//         "File",
//         "Operation", 
//         "Column",
//         "Output"};
//     std::vector<std::string> tValues = {
//         "HarvestDataFromFile", 
//         "harvest_objective_criterion_id_2_eval_0", 
//         "./evaluations_0/gemma_matched_power_balance_input_deck_power_balance.dat", 
//         "max", 
//         "1",
//         ""};
//     PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);
//     auto tInput = tOperation.child("Output");
//     ASSERT_FALSE(tInput.empty());
//     tKeys = {"ArgumentName","Layout","Size"};
//     tValues = {"objective_value_0","scalar","1"};
//     PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);

//     tOperation = tOperation.next_sibling("Operation");
//     ASSERT_FALSE(tOperation.empty());
//     ASSERT_STREQ("Operation", tOperation.name());

//     tKeys = {"Function", "Name", 
//         "File",
//         "Operation", 
//         "Column",
//         "Output"};
//     tValues = {"HarvestDataFromFile", "harvest_objective_criterion_id_2_eval_1", 
//         "./evaluations_1/gemma_matched_power_balance_input_deck_power_balance.dat", 
//         "max", 
//         "1",
//         ""};
//     PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);
//     tInput = tOperation.child("Input");
//     ASSERT_FALSE(tInput.empty());
//     tKeys = {"ArgumentName","Layout","Size"};
//     tValues = {"objective_value_1","scalar","1"};
//     PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);

//     tOperation = tOperation.next_sibling("Operation");
//     ASSERT_TRUE(tOperation.empty());
// }

// TEST(PlatoTestXMLGenerator, write_operation_file_gemma_usecase_system_call)
// {
//     // use case: gemma call is defined by a system call mpi operation
//     XMLGen::InputData tInputMetaData;
//     // define criterion
//     XMLGen::Criterion tCriterionOne;
//     tCriterionOne.id("1");
//     tCriterionOne.type("volume");
//     tInputMetaData.append(tCriterionOne);
//     XMLGen::Criterion tCriterionTwo;
//     tCriterionTwo.id("2");
//     tCriterionTwo.type("system_call");
//     tCriterionTwo.append("data_group", "column_1");
//     tCriterionTwo.append("data_extraction_operation", "max");
//     tCriterionTwo.append("data_file", "matched_power_balance.dat");
//     tInputMetaData.append(tCriterionTwo);
//     XMLGen::Criterion tCriterionThree;
//     tCriterionThree.id("3");
//     tCriterionThree.type("system_call");
//     tCriterionThree.append("data_group", "column_2");
//     tCriterionThree.append("data_extraction_operation", "max");
//     tInputMetaData.append(tCriterionThree);
//     // define objective
//     XMLGen::Objective tObjective;
//     tObjective.criteriaIDs.push_back("1");
//     tObjective.criteriaIDs.push_back("2");
//     tInputMetaData.objective = tObjective;
//     // define constraints
//     XMLGen::Constraint tConstraint;
//     tConstraint.criterion("3");
//     tInputMetaData.constraints.push_back(tConstraint);
//     // define optimization parameters 
//     XMLGen::OptimizationParameters tOptParams;
//     std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
//     tOptParams.descriptors(tDescriptors);
//     tOptParams.append("concurrent_evaluations", "2");
//     tInputMetaData.set(tOptParams);
//     // service parameters
//     XMLGen::Service tServiceOne;
//     tServiceOne.append("code", "gemma");
//     tServiceOne.append("id", "1");
//     tServiceOne.append("type", "system_call");
//     tServiceOne.append("number_processors", "1");
//     tInputMetaData.append(tServiceOne);
//     XMLGen::Service tServiceTwo;
//     tServiceTwo.append("code", "platomain");
//     tServiceTwo.append("id", "2");
//     tServiceTwo.append("type", "plato_app");
//     tServiceTwo.append("number_processors", "2");
//     tInputMetaData.append(tServiceTwo);
    
//     // write files
//     XMLGen::gemma::write_operation_file(tInputMetaData);
//     auto tReadData = XMLGen::read_data_from_file("plato_gemma_app_operation_file.xml");
//     auto tGoldString = std::string("<?xmlversion=\"1.0\"?>")+
//     "<Operation><Function>SystemCall</Function><Name>aprepro_0</Name><Command>aprepro</Command><ChDir>gemma_evaluation_0/</ChDir><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>gemma_matched_power_balance_input_deck.yaml.template</Argument><Argument>gemma_matched_power_balance_input_deck.yaml</Argument><Option>slot_length</Option><Option>slot_width</Option><Option>slot_depth</Option><Input><ArgumentName>parameters_0</ArgumentName><Layout>scalar</Layout><Size>3</Size></Input></Operation>"+
//     "<Operation><Function>SystemCall</Function><Name>aprepro_1</Name><Command>aprepro</Command><ChDir>gemma_evaluation_1/</ChDir><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>gemma_matched_power_balance_input_deck.yaml.template</Argument><Argument>gemma_matched_power_balance_input_deck.yaml</Argument><Option>slot_length</Option><Option>slot_width</Option><Option>slot_depth</Option><Input><ArgumentName>parameters_1</ArgumentName><Layout>scalar</Layout><Size>3</Size></Input></Operation>"+
//     "<Operation><Function>SystemCallMPI</Function><Name>gemma_0</Name><Command>gemma</Command><OnChange>true</OnChange><NumRanks>1</NumRanks><AppendInput>false</AppendInput><ChDir>gemma_evaluation_0/</ChDir><Argument>gemma_matched_power_balance_input_deck.yaml</Argument></Operation>"+
//     "<Operation><Function>SystemCallMPI</Function><Name>gemma_1</Name><Command>gemma</Command><OnChange>true</OnChange><NumRanks>1</NumRanks><AppendInput>false</AppendInput><ChDir>gemma_evaluation_1/</ChDir><Argument>gemma_matched_power_balance_input_deck.yaml</Argument></Operation>"+
//     "<Operation><Function>SystemCall</Function><Name>WaitToHarvest_0</Name><Command>while lsof -u $USER | grep ./gemma_evaluation_0/matched_power_balance.dat; do sleep 1; done; </Command><OnChange>false</OnChange></Operation>"+
//     "<Operation><Function>SystemCall</Function><Name>WaitToHarvest_1</Name><Command>while lsof -u $USER | grep ./gemma_evaluation_1/matched_power_balance.dat; do sleep 1; done; </Command><OnChange>false</OnChange></Operation>"+
//     "<Operation><Function>HarvestDataFromFile</Function><Name>harvest_objective_criterion_id_2_eval_0</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>objectivevalueforcriterion2andevaluation0</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation>"+
//     "<Operation><Function>HarvestDataFromFile</Function><Name>harvest_objective_criterion_id_2_eval_1</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>objectivevalueforcriterion2andevaluation1</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation>"+
//     "<Operation><Function>HarvestDataFromFile</Function><Name>harvest_constraint_criterion_id_3_eval_0</Name><File>./gemma_evaluation_0/matched_power_balance.dat</File><Operation>max</Operation><Column>2</Column><Output><ArgumentName>constraintvalueforcriterion3andevaluation0</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation>"+
//     "<Operation><Function>HarvestDataFromFile</Function><Name>harvest_constraint_criterion_id_3_eval_1</Name><File>./gemma_evaluation_1/matched_power_balance.dat</File><Operation>max</Operation><Column>2</Column><Output><ArgumentName>constraintvalueforcriterion3andevaluation1</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation>";
//     ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
//     //Plato::system("rm -f plato_gemma_app_operation_file.xml");
// }

// TEST(PlatoTestXMLGenerator, write_operation_file_gemma_usecase_web_app)
// {
//     // use case: gemma call is defined by a web app
//     XMLGen::InputData tInputMetaData;
//     // define criterion
//     XMLGen::Criterion tCriterionOne;
//     tCriterionOne.id("1");
//     tCriterionOne.type("volume");
//     tInputMetaData.append(tCriterionOne);
//     XMLGen::Criterion tCriterionTwo;
//     tCriterionTwo.id("2");
//     tCriterionTwo.type("system_call");
//     tCriterionTwo.append("data_group", "column_1");
//     tCriterionTwo.append("data_extraction_operation", "max");
//     tCriterionTwo.append("data_file", "matched_power_balance.dat");
//     tInputMetaData.append(tCriterionTwo);
//     XMLGen::Criterion tCriterionThree;
//     tCriterionThree.id("3");
//     tCriterionThree.type("system_call");
//     tCriterionThree.append("data_group", "column_2");
//     tCriterionThree.append("data_extraction_operation", "max");
//     tInputMetaData.append(tCriterionThree);
//     // define objective
//     XMLGen::Objective tObjective;
//     tObjective.criteriaIDs.push_back("1");
//     tObjective.criteriaIDs.push_back("2");
//     tInputMetaData.objective = tObjective;
//     // define constraints
//     XMLGen::Constraint tConstraint;
//     tConstraint.criterion("3");
//     tInputMetaData.constraints.push_back(tConstraint);
//     // define optimization parameters 
//     XMLGen::OptimizationParameters tOptParams;
//     std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
//     tOptParams.descriptors(tDescriptors);
//     tOptParams.append("concurrent_evaluations", "2");
//     tInputMetaData.set(tOptParams);
//     // service parameters
//     XMLGen::Service tServiceOne;
//     tServiceOne.append("code", "gemma");
//     tServiceOne.append("id", "1");
//     tServiceOne.append("type", "web_app");
//     tServiceOne.append("number_processors", "1");
//     tInputMetaData.append(tServiceOne);
//     XMLGen::Service tServiceTwo;
//     tServiceTwo.append("code", "platomain");
//     tServiceTwo.append("id", "2");
//     tServiceTwo.append("type", "plato_app");
//     tServiceTwo.append("number_processors", "2");
//     tInputMetaData.append(tServiceTwo);
    
//     // write files
//     XMLGen::gemma::write_operation_file(tInputMetaData);
//     auto tReadData = XMLGen::read_data_from_file("plato_gemma_app_operation_file.xml");
//     auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>SystemCall</Function><Name>aprepro_0</Name><Command>aprepro</Command><ChDir>gemma_evaluation_0/</ChDir><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>gemma_matched_power_balance_input_deck.yaml.template</Argument><Argument>gemma_matched_power_balance_input_deck.yaml</Argument><Option>slot_length</Option><Option>slot_width</Option><Option>slot_depth</Option><Input><ArgumentName>parameters_0</ArgumentName><Layout>scalar</Layout><Size>3</Size></Input></Operation><Operation><Function>SystemCall</Function><Name>aprepro_1</Name><Command>aprepro</Command><ChDir>gemma_evaluation_1/</ChDir><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>gemma_matched_power_balance_input_deck.yaml.template</Argument><Argument>gemma_matched_power_balance_input_deck.yaml</Argument><Option>slot_length</Option><Option>slot_width</Option><Option>slot_depth</Option><Input><ArgumentName>parameters_1</ArgumentName><Layout>scalar</Layout><Size>3</Size></Input></Operation><Operation><Function>SystemCall</Function><Name>gemma_0</Name><Command>curl</Command><OnChange>true</OnChange><AppendInput>false</AppendInput><ChDir>gemma_evaluation_0/</ChDir><Argument>http://localhost:7000/run_gemma/</Argument></Operation><Operation><Function>SystemCall</Function><Name>gemma_1</Name><Command>curl</Command><OnChange>true</OnChange><AppendInput>false</AppendInput><ChDir>gemma_evaluation_1/</ChDir><Argument>http://localhost:7001/run_gemma/</Argument></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_objective_criterion_id_2_eval_0</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>objectivevalueforcriterion2andevaluation0</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_objective_criterion_id_2_eval_1</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>objectivevalueforcriterion2andevaluation1</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_constraint_criterion_id_3_eval_0</Name><File>./gemma_evaluation_0/matched_power_balance.dat</File><Operation>max</Operation><Column>2</Column><Output><ArgumentName>constraintvalueforcriterion3andevaluation0</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_constraint_criterion_id_3_eval_1</Name><File>./gemma_evaluation_1/matched_power_balance.dat</File><Operation>max</Operation><Column>2</Column><Output><ArgumentName>constraintvalueforcriterion3andevaluation1</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation>");
//     ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
//     Plato::system("rm -f plato_gemma_app_operation_file.xml");
// }

// TEST(PlatoTestXMLGenerator, write_harvest_data_from_file_operation_matched_power_balance_usecase_1)
// {
//     // use case: only objective functions are defined
//     XMLGen::InputData tInputMetaData;
//     // define criterion
//     XMLGen::Criterion tCriterionOne;
//     tCriterionOne.id("1");
//     tCriterionOne.type("volume");
//     tInputMetaData.append(tCriterionOne);
//     XMLGen::Criterion tCriterionTwo;
//     tCriterionTwo.id("2");
//     tCriterionTwo.type("system_call");
//     tCriterionTwo.append("data_group", "column_1");
//     tCriterionTwo.append("data_extraction_operation", "max");
//     tCriterionTwo.append("data_file", "matched_power_balance.dat");
//     tInputMetaData.append(tCriterionTwo);
//     XMLGen::Criterion tCriterionThree;
//     tCriterionThree.id("3");
//     tCriterionThree.type("mass");
//     tInputMetaData.append(tCriterionThree);
//     // define objective
//     XMLGen::Objective tObjective;
//     tObjective.criteriaIDs.push_back("1");
//     tObjective.criteriaIDs.push_back("2");
//     tObjective.criteriaIDs.push_back("3");
//     tInputMetaData.objective = tObjective;
//     // define optimization parameters 
//     XMLGen::OptimizationParameters tOptParams;
//     tOptParams.append("concurrent_evaluations", "2");
//     tInputMetaData.set(tOptParams);

//     // test function 
//     pugi::xml_document tDocument;
//     XMLGen::matched_power_balance::write_harvest_data_from_file_operation("gemma_evaluation", tInputMetaData, tDocument);
//     tDocument.save_file("dummy.txt", " ");

//     auto tReadData = XMLGen::read_data_from_file("dummy.txt");
//     auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>HarvestDataFromFile</Function><Name>harvest_objective_criterion_id_2_eval_0</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>objectivevalueforcriterion2andevaluation0</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_objective_criterion_id_2_eval_1</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>objectivevalueforcriterion2andevaluation1</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation>");
//     ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
//     Plato::system("rm -f dummy.txt");
// }

// TEST(PlatoTestXMLGenerator, write_harvest_data_from_file_operation_matched_power_balance_usecase_2)
// {
//     // use case: only constraint functions are defined
//     XMLGen::InputData tInputMetaData;
//     // define criterion
//     XMLGen::Criterion tCriterionOne;
//     tCriterionOne.id("1");
//     tCriterionOne.type("volume");
//     tInputMetaData.append(tCriterionOne);
//     XMLGen::Criterion tCriterionTwo;
//     tCriterionTwo.id("2");
//     tCriterionTwo.type("system_call");
//     tCriterionTwo.append("data_group", "column_1");
//     tCriterionTwo.append("data_extraction_operation", "max");
//     tCriterionTwo.append("data_file", "matched_power_balance.dat");
//     tInputMetaData.append(tCriterionTwo);
//     XMLGen::Criterion tCriterionThree;
//     tCriterionThree.id("3");
//     tCriterionThree.type("mass");
//     tInputMetaData.append(tCriterionThree);
//     // define constraints
//     XMLGen::Constraint tConstraintOne;
//     tConstraintOne.criterion("1");
//     tInputMetaData.constraints.push_back(tConstraintOne);
//     XMLGen::Constraint tConstraintTwo;
//     tConstraintTwo.criterion("2");
//     tInputMetaData.constraints.push_back(tConstraintTwo);
//     XMLGen::Constraint tConstraintThree;
//     tConstraintThree.criterion("3");
//     tInputMetaData.constraints.push_back(tConstraintThree);
//     // define optimization parameters 
//     XMLGen::OptimizationParameters tOptParams;
//     tOptParams.append("concurrent_evaluations", "2");
//     tInputMetaData.set(tOptParams);

//     // test function 
//     pugi::xml_document tDocument;
//     XMLGen::matched_power_balance::write_harvest_data_from_file_operation("gemma_evaluation", tInputMetaData, tDocument);
//     tDocument.save_file("dummy.txt", " ");

//     auto tReadData = XMLGen::read_data_from_file("dummy.txt");
//     auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>HarvestDataFromFile</Function><Name>harvest_constraint_criterion_id_2_eval_0</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>constraintvalueforcriterion2andevaluation0</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_constraint_criterion_id_2_eval_1</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>constraintvalueforcriterion2andevaluation1</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation>");
//     ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
//     Plato::system("rm -f dummy.txt");
// }

// TEST(PlatoTestXMLGenerator, write_harvest_data_from_file_operation_matched_power_balance_usecase_3)
// {
//     // use case: constraint and objective functions defined
//     XMLGen::InputData tInputMetaData;
//     // define criterion
//     XMLGen::Criterion tCriterionOne;
//     tCriterionOne.id("1");
//     tCriterionOne.type("volume");
//     tInputMetaData.append(tCriterionOne);
//     XMLGen::Criterion tCriterionTwo;
//     tCriterionTwo.id("2");
//     tCriterionTwo.type("system_call");
//     tCriterionTwo.append("data_group", "column_1");
//     tCriterionTwo.append("data_extraction_operation", "max");
//     tCriterionTwo.append("data_file", "matched_power_balance.dat");
//     tInputMetaData.append(tCriterionTwo);
//     XMLGen::Criterion tCriterionThree;
//     tCriterionThree.id("3");
//     tCriterionThree.type("system_call");
//     tCriterionThree.append("data_group", "column_2");
//     tCriterionThree.append("data_extraction_operation", "max");
//     tInputMetaData.append(tCriterionThree);
//     // define objective
//     XMLGen::Objective tObjective;
//     tObjective.criteriaIDs.push_back("1");
//     tObjective.criteriaIDs.push_back("2");
//     tInputMetaData.objective = tObjective;
//     // define constraints
//     XMLGen::Constraint tConstraint;
//     tConstraint.criterion("3");
//     tInputMetaData.constraints.push_back(tConstraint);
//     // define optimization parameters 
//     XMLGen::OptimizationParameters tOptParams;
//     tOptParams.append("concurrent_evaluations", "2");
//     tInputMetaData.set(tOptParams);

//     // test function 
//     pugi::xml_document tDocument;
//     XMLGen::matched_power_balance::write_harvest_data_from_file_operation("gemma_evaluation", tInputMetaData, tDocument);
//     tDocument.save_file("dummy.txt", " ");

//     auto tReadData = XMLGen::read_data_from_file("dummy.txt");
//     auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>HarvestDataFromFile</Function><Name>harvest_objective_criterion_id_2_eval_0</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>objectivevalueforcriterion2andevaluation0</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_objective_criterion_id_2_eval_1</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>objectivevalueforcriterion2andevaluation1</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_constraint_criterion_id_3_eval_0</Name><File>./gemma_evaluation_0/matched_power_balance.dat</File><Operation>max</Operation><Column>2</Column><Output><ArgumentName>constraintvalueforcriterion3andevaluation0</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_constraint_criterion_id_3_eval_1</Name><File>./gemma_evaluation_1/matched_power_balance.dat</File><Operation>max</Operation><Column>2</Column><Output><ArgumentName>constraintvalueforcriterion3andevaluation1</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation>");
//     ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
//     Plato::system("rm -f dummy.txt");
// }

// TEST(PlatoTestXMLGenerator, append_system_call_objective_criteria_options)
// {
//     XMLGen::InputData tInputMetaData;
//     // define criterion
//     XMLGen::Criterion tCriterionOne;
//     tCriterionOne.id("1");
//     tCriterionOne.type("volume");
//     tInputMetaData.append(tCriterionOne);
//     XMLGen::Criterion tCriterionTwo;
//     tCriterionTwo.id("2");
//     tCriterionTwo.type("system_call");
//     tCriterionTwo.append("data_group", "column_1");
//     tCriterionTwo.append("data_extraction_operation", "max");
//     tCriterionTwo.append("data_file", "matched_power_balance.dat");
//     tInputMetaData.append(tCriterionTwo);
//     XMLGen::Criterion tCriterionThree;
//     tCriterionThree.id("3");
//     tCriterionThree.type("mass");
//     tInputMetaData.append(tCriterionThree);
//     // define objective
//     XMLGen::Objective tObjective;
//     tObjective.criteriaIDs.push_back("1");
//     tObjective.criteriaIDs.push_back("2");
//     tObjective.criteriaIDs.push_back("3");
//     tInputMetaData.objective = tObjective;
    
//     XMLGen::OperationMetaData tOperationMetaData;
//     auto tSystemCallIDs = XMLGen::get_criteria_ids("system_call", tInputMetaData);
//     XMLGen::append_system_call_objective_criteria_options(tSystemCallIDs, tInputMetaData, tOperationMetaData);
//     EXPECT_STREQ("2", tOperationMetaData.get("objective_criteria_ids").front().c_str());
//     EXPECT_STREQ("column_1", tOperationMetaData.get("objective_data_groups").front().c_str());
//     EXPECT_STREQ("max", tOperationMetaData.get("objective_data_extraction_operations").front().c_str());
//     EXPECT_STREQ("matched_power_balance.dat", tOperationMetaData.get("objective_data_files").front().c_str());
// }

// TEST(PlatoTestXMLGenerator, append_system_call_constraint_criteria_options)
// {
//     XMLGen::InputData tInputMetaData;
//     // define criterion
//     XMLGen::Criterion tCriterionOne;
//     tCriterionOne.id("1");
//     tCriterionOne.type("system_call");
//     tCriterionOne.append("data_group", "column_2");
//     tCriterionOne.append("data_extraction_operation", "min");
//     tCriterionOne.append("data_file", "matched_power_balance.dat");
//     tInputMetaData.append(tCriterionOne);
//     XMLGen::Criterion tCriterionTwo;
//     tCriterionTwo.id("2");
//     tCriterionTwo.type("volume");
//     tInputMetaData.append(tCriterionTwo);
//     XMLGen::Criterion tCriterionThree;
//     tCriterionThree.id("3");
//     tCriterionThree.type("mass");
//     tInputMetaData.append(tCriterionThree);
//     // define constraints
//     XMLGen::Constraint tConstraintOne;
//     tConstraintOne.criterion("1");
//     tInputMetaData.constraints.push_back(tConstraintOne);
//     XMLGen::Constraint tConstraintTwo;
//     tConstraintTwo.criterion("2");
//     tInputMetaData.constraints.push_back(tConstraintTwo);
//     XMLGen::Constraint tConstraintThree;
//     tConstraintThree.criterion("3");
//     tInputMetaData.constraints.push_back(tConstraintThree);
    
//     XMLGen::OperationMetaData tOperationMetaData;
//     auto tSystemCallIDs = XMLGen::get_criteria_ids("system_call", tInputMetaData);
//     XMLGen::append_system_call_constraint_criteria_options(tSystemCallIDs, tInputMetaData, tOperationMetaData);
//     EXPECT_STREQ("1", tOperationMetaData.get("constraint_criteria_ids").front().c_str());
//     EXPECT_STREQ("column_2", tOperationMetaData.get("constraint_data_groups").front().c_str());
//     EXPECT_STREQ("min", tOperationMetaData.get("constraint_data_extraction_operations").front().c_str());
//     EXPECT_STREQ("matched_power_balance.dat", tOperationMetaData.get("constraint_data_files").front().c_str());
// }

// TEST(PlatoTestXMLGenerator, write_run_app_via_system_call_operation_matched_power_balance)
// {
//     XMLGen::InputData tMetaData;
//     // define service
//     XMLGen::Service tServiceOne;
//     tServiceOne.append("code", "gemma");
//     tServiceOne.append("id", "1");
//     tServiceOne.append("type", "system_call");
//     tServiceOne.append("number_processors", "1");
//     tMetaData.append(tServiceOne);
//     XMLGen::Service tServiceTwo;
//     tServiceTwo.append("code", "platomain");
//     tServiceTwo.append("id", "2");
//     tServiceTwo.append("type", "plato_app");
//     tServiceTwo.append("number_processors", "2");
//     tMetaData.append(tServiceTwo);
//     // define optimization parameters 
//     XMLGen::OptimizationParameters tOptParams;
//     tOptParams.append("concurrent_evaluations", "2");
//     tMetaData.set(tOptParams);

//     pugi::xml_document tDocument;
//     XMLGen::matched_power_balance::write_run_app_via_system_call_operation("gemma_evaluation", tMetaData, tDocument);
//     tDocument.save_file("dummy.txt", " ");

//     auto tReadData = XMLGen::read_data_from_file("dummy.txt");
//     auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>SystemCallMPI</Function><Name>gemma_0</Name><Command>gemma</Command><OnChange>true</OnChange><AppendInput>false</AppendInput><ChDir>gemma_evaluation_0/</ChDir><Argument>gemma_matched_power_balance_input_deck.yaml</Argument></Operation><Operation><Function>SystemCallMPI</Function><Name>gemma_1</Name><Command>gemma</Command><OnChange>true</OnChange><AppendInput>false</AppendInput><ChDir>gemma_evaluation_1/</ChDir><Argument>gemma_matched_power_balance_input_deck.yaml</Argument></Operation>");
//     ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
//     Plato::system("rm -f dummy.txt");
// }

// TEST(PlatoTestXMLGenerator, append_operation_options_based_on_app_type)
// {
//     XMLGen::gemma::Options tAppOptions;
//     XMLGen::OperationMetaData tOperationMetaDataOne;
//     tOperationMetaDataOne.append("concurrent_evaluations", "3");

//     // test one: web_app options
//     tOperationMetaDataOne.set("type", std::vector<std::string>(3, "web_app"));
//     XMLGen::append_operation_options_based_on_app_type("commands", tAppOptions.get("commands"), tOperationMetaDataOne);
//     EXPECT_EQ(3u, tOperationMetaDataOne.get("commands").size());
//     EXPECT_STREQ("curl", tOperationMetaDataOne.get("commands")[0].c_str());
//     EXPECT_STREQ("curl", tOperationMetaDataOne.get("commands")[1].c_str());
//     EXPECT_STREQ("curl", tOperationMetaDataOne.get("commands")[2].c_str());

//     XMLGen::append_operation_options_based_on_app_type("functions", tAppOptions.get("functions"), tOperationMetaDataOne);
//     EXPECT_EQ(3u, tOperationMetaDataOne.get("functions").size());
//     EXPECT_STREQ("SystemCall", tOperationMetaDataOne.get("functions")[0].c_str());
//     EXPECT_STREQ("SystemCall", tOperationMetaDataOne.get("functions")[1].c_str());
//     EXPECT_STREQ("SystemCall", tOperationMetaDataOne.get("functions")[2].c_str());
    
//     EXPECT_THROW(XMLGen::append_operation_options_based_on_app_type("ranks", tAppOptions.get("ranks"), tOperationMetaDataOne), std::runtime_error); // error: key not defined

//     // test two: system_call options
//     XMLGen::OperationMetaData tOperationMetaDataTwo;
//     tOperationMetaDataTwo.append("concurrent_evaluations", "3");
//     tOperationMetaDataTwo.set("type", std::vector<std::string>(3, "system_call"));
//     XMLGen::append_operation_options_based_on_app_type("commands", tAppOptions.get("commands"), tOperationMetaDataTwo);
//     EXPECT_EQ(3u, tOperationMetaDataTwo.get("commands").size());
//     EXPECT_STREQ("gemma", tOperationMetaDataTwo.get("commands")[0].c_str());
//     EXPECT_STREQ("gemma", tOperationMetaDataTwo.get("commands")[1].c_str());
//     EXPECT_STREQ("gemma", tOperationMetaDataTwo.get("commands")[2].c_str());

//     XMLGen::append_operation_options_based_on_app_type("functions", tAppOptions.get("functions"), tOperationMetaDataTwo);
//     EXPECT_EQ(3u, tOperationMetaDataTwo.get("functions").size());
//     EXPECT_STREQ("SystemCallMPI", tOperationMetaDataTwo.get("functions")[0].c_str());
//     EXPECT_STREQ("SystemCallMPI", tOperationMetaDataTwo.get("functions")[1].c_str());
//     EXPECT_STREQ("SystemCallMPI", tOperationMetaDataTwo.get("functions")[2].c_str());
// }

// TEST(PlatoTestXMLGenerator, set_operation_option_from_service_metadata)
// {
//     XMLGen::InputData tMetaData;
//     // define service
//     XMLGen::Service tServiceOne;
//     tServiceOne.append("code", "gemma");
//     tServiceOne.append("id", "1");
//     tServiceOne.append("type", "web_app");
//     tServiceOne.append("number_processors", "1");
//     tMetaData.append(tServiceOne);
//     XMLGen::Service tServiceTwo;
//     tServiceTwo.append("code", "platomain");
//     tServiceTwo.append("id", "2");
//     tServiceTwo.append("type", "plato_app");
//     tServiceTwo.append("number_processors", "2");
//     tMetaData.append(tServiceTwo);

//     XMLGen::OperationMetaData tOperationMetaData;
//     tOperationMetaData.append("code", "gemma");
//     tOperationMetaData.append("concurrent_evaluations", "3");

//     // test one: one service defined 
//     XMLGen::set_operation_option_from_service_metadata("type", tMetaData, tOperationMetaData);
//     EXPECT_EQ(3u, tOperationMetaData.get("type").size());
//     EXPECT_STREQ("web_app", tOperationMetaData.get("type")[0].c_str());
//     EXPECT_STREQ("web_app", tOperationMetaData.get("type")[1].c_str());
//     EXPECT_STREQ("web_app", tOperationMetaData.get("type")[2].c_str());

//     XMLGen::set_operation_option_from_service_metadata("number_processors", tMetaData, tOperationMetaData);
//     EXPECT_EQ(3u, tOperationMetaData.get("number_processors").size());
//     EXPECT_STREQ("1", tOperationMetaData.get("number_processors")[0].c_str());
//     EXPECT_STREQ("1", tOperationMetaData.get("number_processors")[1].c_str());
//     EXPECT_STREQ("1", tOperationMetaData.get("number_processors")[2].c_str());

//     // test two: number of services match the number of concurrent evaluations
//     XMLGen::Service tServiceThree;
//     tServiceThree.append("code", "gemma");
//     tServiceThree.append("id", "3");
//     tServiceThree.append("type", "web_app");
//     tServiceThree.append("number_processors", "1");
//     tMetaData.append(tServiceThree);
//     XMLGen::Service tServiceFour;
//     tServiceFour.append("code", "gemma");
//     tServiceFour.append("id", "4");
//     tServiceFour.append("type", "system_call");
//     tServiceFour.append("number_processors", "2");
//     tMetaData.append(tServiceFour);

//     XMLGen::set_operation_option_from_service_metadata("type", tMetaData, tOperationMetaData);
//     EXPECT_EQ(3u, tOperationMetaData.get("type").size());
//     EXPECT_STREQ("web_app", tOperationMetaData.get("type")[0].c_str());
//     EXPECT_STREQ("web_app", tOperationMetaData.get("type")[1].c_str());
//     EXPECT_STREQ("system_call", tOperationMetaData.get("type")[2].c_str());

//     XMLGen::set_operation_option_from_service_metadata("number_processors", tMetaData, tOperationMetaData);
//     EXPECT_EQ(3u, tOperationMetaData.get("number_processors").size());
//     EXPECT_STREQ("1", tOperationMetaData.get("number_processors")[0].c_str());
//     EXPECT_STREQ("1", tOperationMetaData.get("number_processors")[1].c_str());
//     EXPECT_STREQ("2", tOperationMetaData.get("number_processors")[2].c_str());

//     // test three: error -> the number of services exceeds the number of concurrent evaluations
//     XMLGen::Service tServiceFive;
//     tServiceFive.append("code", "gemma");
//     tServiceFive.append("id", "5");
//     tServiceFive.append("type", "system_call");
//     tServiceFive.append("number_processors", "2");
//     tMetaData.append(tServiceFive);

//     EXPECT_THROW(XMLGen::set_operation_option_from_service_metadata("type", tMetaData, tOperationMetaData), std::runtime_error);
// }

// TEST(PlatoTestXMLGenerator, get_values_from_service_metadata)
// {
//     XMLGen::InputData tMetaData;
//     // define service
//     XMLGen::Service tServiceOne;
//     tServiceOne.append("code", "gemma");
//     tServiceOne.append("id", "1");
//     tServiceOne.append("type", "web_app");
//     tServiceOne.append("number_processors", "1");
//     tMetaData.append(tServiceOne);
//     XMLGen::Service tServiceTwo;
//     tServiceTwo.append("code", "platomain");
//     tServiceTwo.append("id", "2");
//     tServiceTwo.append("type", "plato_app");
//     tServiceTwo.append("number_processors", "2");
//     tMetaData.append(tServiceTwo);

//     // test gemma 
//     auto tValues = XMLGen::get_values_from_service_metadata("gemma", "number_processors", tMetaData);
//     EXPECT_EQ(1u, tValues.size());
//     EXPECT_STREQ("1", tValues.front().c_str());
//     tValues = XMLGen::get_values_from_service_metadata("GEMMA", "TYPE", tMetaData);
//     EXPECT_EQ(1u, tValues.size());
//     EXPECT_STREQ("web_app", tValues.front().c_str());

//     // test platomain 
//     tValues = XMLGen::get_values_from_service_metadata("platomain", "number_processors", tMetaData);
//     EXPECT_EQ(1u, tValues.size());
//     EXPECT_STREQ("2", tValues.front().c_str());
//     tValues = XMLGen::get_values_from_service_metadata("Platomain", "Type", tMetaData);
//     EXPECT_EQ(1u, tValues.size());
//     EXPECT_STREQ("plato_app", tValues.front().c_str());
// }

// TEST(PlatoTestXMLGenerator, aprepro_system_call_matched_power_balance)
// {
//     XMLGen::InputData tMetaData;
//     // define optmization parameters
//     XMLGen::OptimizationParameters tOptParams;
//     std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
//     tOptParams.descriptors(tDescriptors);
//     tOptParams.append("concurrent_evaluations", "3");
//     tMetaData.set(tOptParams);

//     pugi::xml_document tDocument;
//     XMLGen::matched_power_balance::write_aprepro_system_call_operation("gemma_evaluation", tMetaData, tDocument);
//     tDocument.save_file("dummy.txt");

//     auto tReadData = XMLGen::read_data_from_file("dummy.txt");
//     auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>SystemCall</Function><Name>aprepro_0</Name><Command>aprepro</Command><ChDir>gemma_evaluation_0/</ChDir><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>gemma_matched_power_balance_input_deck.yaml.template</Argument><Argument>gemma_matched_power_balance_input_deck.yaml</Argument><Option>slot_length</Option><Option>slot_width</Option><Option>slot_depth</Option><Input><ArgumentName>parameters_0</ArgumentName><Layout>scalar</Layout><Size>3</Size></Input></Operation><Operation><Function>SystemCall</Function><Name>aprepro_1</Name><Command>aprepro</Command><ChDir>gemma_evaluation_1/</ChDir><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>gemma_matched_power_balance_input_deck.yaml.template</Argument><Argument>gemma_matched_power_balance_input_deck.yaml</Argument><Option>slot_length</Option><Option>slot_width</Option><Option>slot_depth</Option><Input><ArgumentName>parameters_1</ArgumentName><Layout>scalar</Layout><Size>3</Size></Input></Operation><Operation><Function>SystemCall</Function><Name>aprepro_2</Name><Command>aprepro</Command><ChDir>gemma_evaluation_2/</ChDir><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>gemma_matched_power_balance_input_deck.yaml.template</Argument><Argument>gemma_matched_power_balance_input_deck.yaml</Argument><Option>slot_length</Option><Option>slot_width</Option><Option>slot_depth</Option><Input><ArgumentName>parameters_2</ArgumentName><Layout>scalar</Layout><Size>3</Size></Input></Operation>");
//     ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
//     Plato::system("rm -f dummy.txt");
// }

// TEST(PlatoTestXMLGenerator, write_aprepro_system_call_operation)
// {
//     XMLGen::OperationMetaData tOperationMetaData;
//     tOperationMetaData.append("concurrent_evaluations", "2");
//     tOperationMetaData.set("names", {"aprepro_0", "aprepro_1"});
//     tOperationMetaData.set("options", {"length=", "depth=", "width="});
//     tOperationMetaData.set("subdirectories", {"evaluation_0", "evaluation_1"});
//     tOperationMetaData.set("commands", std::vector<std::string>(2u, "aprepro"));
//     tOperationMetaData.set("functions", std::vector<std::string>(2u, "SystemCall"));
//     tOperationMetaData.set("arguments", {"-q", "matched.yaml.template", "matched.yaml"});

//     std::vector<XMLGen::OperationArgument> tInputs;
//     XMLGen::OperationArgument tInputOne;
//     tInputOne.set("size", "3");
//     tInputOne.set("type", "input");
//     tInputOne.set("layout", "scalars");
//     tInputOne.set("name", "parameters_0");
//     tInputs.push_back(tInputOne);
//     XMLGen::OperationArgument tInputTwo;
//     tInputTwo.set("size", "3");
//     tInputTwo.set("type", "input");
//     tInputTwo.set("layout", "scalars");
//     tInputTwo.set("name", "parameters_1");
//     tInputs.push_back(tInputTwo);

//     pugi::xml_document tDocument;
//     XMLGen::write_aprepro_system_call_operation(tInputs, tOperationMetaData, tDocument);
//     tDocument.save_file("dummy.txt");

//     auto tReadData = XMLGen::read_data_from_file("dummy.txt");
//     auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>SystemCall</Function><Name>aprepro_0</Name><Command>aprepro</Command><ChDir>evaluation_0</ChDir><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>matched.yaml.template</Argument><Argument>matched.yaml</Argument><Option>length=</Option><Option>depth=</Option><Option>width=</Option><Input><ArgumentName>parameters_0</ArgumentName><Layout>scalars</Layout><Size>3</Size></Input></Operation><Operation><Function>SystemCall</Function><Name>aprepro_1</Name><Command>aprepro</Command><ChDir>evaluation_1</ChDir><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>matched.yaml.template</Argument><Argument>matched.yaml</Argument><Option>length=</Option><Option>depth=</Option><Option>width=</Option><Input><ArgumentName>parameters_1</ArgumentName><Layout>scalars</Layout><Size>3</Size></Input></Operation>");
//     ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
//     Plato::system("rm -f dummy.txt");
// }

// TEST(PlatoTestXMLGenerator, set_input_args_metadata_for_aprepro_operation)
// {
//     // define optmization parameters
//     XMLGen::OperationMetaData tOperationMetaData;
//     std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
//     tOperationMetaData.set("descriptors", tDescriptors);
//     tOperationMetaData.append("concurrent_evaluations", "3");

//     auto tData = XMLGen::set_input_args_metadata_for_aprepro_operation(tOperationMetaData);
//     EXPECT_EQ(3u, tData.size());

//     EXPECT_STREQ("3", tData[0].get("size").c_str());
//     EXPECT_STREQ("input", tData[0].get("type").c_str());
//     EXPECT_STREQ("scalar", tData[0].get("layout").c_str());
//     EXPECT_STREQ("parameters_0", tData[0].get("name").c_str());

//     EXPECT_STREQ("3", tData[1].get("size").c_str());
//     EXPECT_STREQ("input", tData[1].get("type").c_str());
//     EXPECT_STREQ("scalar", tData[1].get("layout").c_str());
//     EXPECT_STREQ("parameters_1", tData[1].get("name").c_str());

//     EXPECT_STREQ("3", tData[2].get("size").c_str());
//     EXPECT_STREQ("input", tData[2].get("type").c_str());
//     EXPECT_STREQ("scalar", tData[2].get("layout").c_str());
//     EXPECT_STREQ("parameters_2", tData[2].get("name").c_str());
// }

// TEST(PlatoTestXMLGenerator, append_concurrent_evaluation_index_to_option)
// {
//     XMLGen::OperationMetaData tOperationMetaData;
//     tOperationMetaData.append("concurrent_evaluations", "3");
//     XMLGen::append_concurrent_evaluation_index_to_option("names", "aprepro_", tOperationMetaData);

//     auto tOutput = tOperationMetaData.get("names");
//     EXPECT_EQ(3u, tOutput.size());
//     EXPECT_STREQ("aprepro_0", tOutput[0].c_str());
//     EXPECT_STREQ("aprepro_1", tOutput[1].c_str());
//     EXPECT_STREQ("aprepro_2", tOutput[2].c_str());
// }

// TEST(PlatoTestXMLGenerator, append_general_aprepro_operation_options)
// {
//     XMLGen::InputData tMetaData;
//     // define optmization parameters
//     XMLGen::OptimizationParameters tOptParams;
//     std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
//     tOptParams.set("descriptors", tDescriptors);
//     tOptParams.append("concurrent_evaluations", "3");
//     tMetaData.set(tOptParams);

//     XMLGen::OperationMetaData tOperationMetaData;
//     XMLGen::append_general_aprepro_operation_options(tMetaData, tOperationMetaData);

//     auto tMyDescriptors = tOperationMetaData.get("descriptors");
//     EXPECT_EQ(3u, tMyDescriptors.size());
//     EXPECT_STREQ(tDescriptors[0].c_str(), tMyDescriptors[0].c_str());
//     EXPECT_STREQ(tDescriptors[1].c_str(), tMyDescriptors[1].c_str());
//     EXPECT_STREQ(tDescriptors[2].c_str(), tMyDescriptors[2].c_str());
//     EXPECT_STREQ("3", tOperationMetaData.get("concurrent_evaluations").front().c_str());
// }

// TEST(PlatoTestXMLGenerator, matched_power_balance_write_input_deck_to_file)
// {
//     std::unordered_map<std::string,std::string> tMap;
//     tMap["slot_width"] = "0.1";
//     tMap["slot_depth"] = "0.2";
//     tMap["slot_length"] = "0.3";
//     tMap["cavity_radius"] = "1";
//     tMap["cavity_height"] = "2";
//     tMap["frequency_min"] = "10";
//     tMap["frequency_max"] = "100";
//     tMap["frequency_step"] = "5";
//     tMap["conductivity"] = "11";
//     XMLGen::matched_power_balance::write_input_deck_to_file("input_deck.yaml",tMap);
//     auto tReadData = XMLGen::read_data_from_file("input_deck.yaml");
//     std::string tGoldString = std::string("%YAML1.1---Gemma-dynamic:Global:Description:HigginscylinderSolutiontype:powerbalancePowerbalance:Algorithm:matchedboundRadius:1Height:2Conductivity:11Slotlength:0.3Slotwidth:0.1Slotdepth:0.2Startfrequencyrange:10Endfrequencyrange:100Frequencyintervalsize:5...");
//     ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
//     Plato::system("rm -f input_deck.yaml");
// }

// TEST(PlatoTestXMLGenerator, matched_power_balance_get_input_key_value_pairs)
// {
//     XMLGen::Scenario tScenario;
//     tScenario.append("frequency_min", "10");
//     tScenario.append("frequency_max", "100");
//     tScenario.append("frequency_step", "5");
//     tScenario.append("cavity_radius", "0.1016");
//     tScenario.append("cavity_height", "0.1018");

//     XMLGen::Material tMaterial;
//     tMaterial.property("conductivity", "1e6");

//     std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
//     auto tPairs = XMLGen::matched_power_balance::get_input_key_value_pairs(tScenario, tMaterial, tDescriptors);
//     EXPECT_EQ(9u, tPairs.size());
//     EXPECT_STREQ("10", tPairs.at("frequency_min").c_str());
//     EXPECT_STREQ("5", tPairs.at("frequency_step").c_str());
//     EXPECT_STREQ("1e6", tPairs.at("conductivity").c_str());
//     EXPECT_STREQ("100", tPairs.at("frequency_max").c_str());
//     EXPECT_STREQ("0.1016", tPairs.at("cavity_radius").c_str());
//     EXPECT_STREQ("0.1018", tPairs.at("cavity_height").c_str());
//     EXPECT_STREQ("{slot_width}", tPairs.at("slot_width").c_str());
//     EXPECT_STREQ("{slot_depth}", tPairs.at("slot_depth").c_str());
//     EXPECT_STREQ("{slot_length}", tPairs.at("slot_length").c_str());
// }

// TEST(PlatoTestXMLGenerator, set_descriptor_value)
// {
//     std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_height"};
//     auto tOutput = XMLGen::set_descriptor_value("slot_length", "0.01", tDescriptors);
//     EXPECT_STREQ("{slot_length}", tOutput.c_str());

//     tOutput = XMLGen::set_descriptor_value("slot_width", "0.01", tDescriptors);
//     EXPECT_STREQ("{slot_width}", tOutput.c_str());

//     tOutput = XMLGen::set_descriptor_value("slot_height", "0.01", tDescriptors);
//     EXPECT_STREQ("{slot_height}", tOutput.c_str());

//     tOutput = XMLGen::set_descriptor_value("conductivity", "0.01", tDescriptors);
//     EXPECT_STREQ("0.01", tOutput.c_str());
// }

// TEST(PlatoTestXMLGenerator, ParseOperationMetaDataCriterion)
// {

//     std::string tStringInput =
//         "begin optimization_parameters\n"
//         "  optimization_type dakota\n"
//         "  dakota_workflow mdps\n"
//         "  concurrent_evaluations 3\n"
//         "  lower_bounds       0.01       0.001       0.002\n"
//         "  upper_bounds       0.10       0.010       0.020\n"
//         "  descriptors     slot_length slot_width slot_height\n"
//         "  mdps_partitions     2           3          4\n"
//         "  mdps_response_functions 1\n"
//         "end optimization_parameters\n"
//         "begin criterion MyOperationMetaData\n"
//         "  type system_call\n"
//         "  aprepro_template_name input.yaml.template"
//         "  command gemma\n"
//         "  arguments input.yaml\n"
//         "  data_file output.dat\n"
//         "  data column_1\n"
//         "  data_name shielding_effectiveness\n"
//         "  data_extraction_operation max\n"
//         "end criterion\n";
//     std::istringstream tInputSS;
//     tInputSS.str(tStringInput);
// }

}
// PlatoTestXMLGenerator