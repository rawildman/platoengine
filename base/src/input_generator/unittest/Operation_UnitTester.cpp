 /*
 * Operation_UnitTester.cpp
 *
 *  Created on: April 12, 2022
 */
#include <gtest/gtest.h>

#include "pugixml.hpp"
#include "Stage.hpp"
#include "Operation.hpp"
#include "SharedData.hpp"
#include "Performer.hpp"
#include "XMLGeneratorDataStruct.hpp"
#include "XMLGeneratorParserUtilities.hpp"
#include "XMLGeneratorUtilities.hpp"
#include "XMLGenerator_UnitTester_Tools.hpp"

namespace PlatoTestXMLGenerator
{

TEST(PlatoTestXMLGenerator, OuterOperationForNodeWaitOperationNoConcurrency)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,0);
    PDir::OperationWait tWait("wait", "file", tPerformer, 0);

    pugi::xml_document tDocument;
    auto tForNode = tWait.forNode(tDocument,std::string("Parameters"));
    XMLGen::addChild(tForNode, "File", "test");

    auto tOperation = tDocument.child("Operation");
    ASSERT_TRUE(tOperation.empty());

    auto tempForNode = tDocument.child("For");
    ASSERT_TRUE(tempForNode.empty());

    PlatoTestXMLGenerator::test_children({"File"}, {"test"}, tDocument);
}

TEST(PlatoTestXMLGenerator, OuterOperationForNodeWaitOperationWithConcurrency)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    PDir::OperationWait tWait("wait", "file", tPerformer, 2);

    pugi::xml_document tDocument;
    auto tForNode = tWait.forNode(tDocument,std::string("Parameters"));
    XMLGen::addChild(tForNode, "File", "test");

    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    auto tempForNode = tOperation.child("For");
    ASSERT_FALSE(tempForNode.empty());

    ASSERT_STREQ("For", tempForNode.name());
    auto tFileNode = tempForNode.child("File");
    ASSERT_FALSE(tFileNode.empty());
    ASSERT_STREQ("File", tFileNode.name());
    
    tempForNode = tempForNode.next_sibling("For");
    ASSERT_TRUE(tempForNode.empty());

    tOperation = tempForNode.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteDefinitionWaitOperationWithConcurrency)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    PDir::OperationWait tWait("wait", "file", tPerformer, 1);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tWait.write_definition(tDocument,"0"));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {"Function",
        "Name", 
        "Command", 
        "ChDir",
        "OnChange"};
    std::vector<std::string> tValues = {"SystemCall", 
        "wait_0", 
        "while lsof -u $USER | grep ./file; do sleep 1; done", 
        "evaluations_0",
        "false"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteDefinitionWaitOperationNoConcurrency)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    PDir::OperationWait tWait("wait", "file", tPerformer, 0);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tWait.write_definition(tDocument,""));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {"Function",
        "Name", 
        "Command", 
        "OnChange"};
    std::vector<std::string> tValues = {"SystemCall", 
        "wait", 
        "while lsof -u $USER | grep ./file; do sleep 1; done", 
        "false"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteDefinitionWaitOperationWithTag)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    PDir::OperationWait tWait("wait", "file", tPerformer,  1);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tWait.write_definition(tDocument,""));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {"Function",
        "Name", 
        "Command", 
        "ChDir",
        "OnChange"};
    std::vector<std::string> tValues = {"SystemCall", 
        "wait_{E}", 
        "while lsof -u $USER | grep ./file; do sleep 1; done", 
        "evaluations_{E}",
        "false"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteInterfaceWaitOperationWithTag)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    PDir::OperationWait tWait("wait", "file", tPerformer, 1);

    pugi::xml_document tDocument;

    ASSERT_NO_THROW(tWait.write_interface(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {
        "Name", 
        "PerformerName"};
    std::vector<std::string> tValues = {
        "wait_{E}", 
        "plato_services_{E}"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteDefinitionGemmaMPIOperationWithConcurrency)
{
    int numRanks = 16;
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",numRanks,2);
    PDir::OperationGemmaMPISystemCall tGemma("match.yaml","", "2", tPerformer, 1);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tGemma.write_definition(tDocument,"1"));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {"Function",
        "Name", 
        "Command", 
        "ChDir",
        "OnChange",
        "NumRanks",
        "Argument",
        "AppendInput"};
    std::vector<std::string> tValues = {"SystemCallMPI", 
        "gemma_1", 
        "gemma", 
        "evaluations_1",
        "true",
        "2",
        "match.yaml",
        "false"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteDefinitionGemmaMPIOperationNoConcurrency)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    PDir::OperationGemmaMPISystemCall tGemma("match.yaml","" ,"2", tPerformer, 0);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tGemma.write_definition(tDocument,""));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {"Function",
        "Name", 
        "Command", 
        "OnChange",
        "NumRanks",
        "Argument",
        "AppendInput"};
    std::vector<std::string> tValues = {"SystemCallMPI", 
        "gemma", 
        "gemma", 
        "true",
        "2",
        "match.yaml",
        "false"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteDefinitionGemmaMPIOperationWithTag)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    PDir::OperationGemmaMPISystemCall tGemma("match.yaml","path/", "2", tPerformer, 1);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tGemma.write_definition(tDocument,""));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {"Function",
        "Name", 
        "Command", 
        "ChDir",
        "OnChange",
        "NumRanks",
        "Argument",
        "AppendInput"};
    std::vector<std::string> tValues = {"SystemCallMPI", 
        "gemma_{E}", 
        "path/gemma", 
        "evaluations_{E}",
        "true",
        "2",
        "match.yaml",
        "false"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteInterfaceGemmaMPIOperationWithTag)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    PDir::OperationGemmaMPISystemCall tGemma("match.yaml", "","2", tPerformer, 1);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tGemma.write_interface(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {
        "Name", 
        "PerformerName"
        };
    std::vector<std::string> tValues = {
        "gemma_{E}", 
        "plato_services_{E}"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteDefinitionApreproOperationWithConcurrency)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    std::shared_ptr<PDir::Performer> tPerformerMain = std::make_shared<PDir::Performer>("platomain","platomain");
    std::vector<std::shared_ptr<PDir::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<PDir::SharedData> tSharedData = std::make_shared<PDir::SharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    PDir::OperationAprepro tAprepro("match.yaml", {"l", "w", "d"},tSharedData, tPerformer, 2);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAprepro.write_definition(tDocument,"2"));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
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
        "aprepro_2", 
        "aprepro", 
        "evaluations_2",
        "true",
        "-q",
        "match.yaml.template",
        "match.yaml",
        "true",
        "l=",
        "w=",
        "d=",
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    auto tInput = tOperation.child("Input");
    ASSERT_FALSE(tInput.empty());
    tKeys = {"ArgumentName", "Layout", "Size"};
    tValues = {"parameters", "scalar", "3"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);
    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteDefinitionApreproOperationWithTag)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    std::shared_ptr<PDir::Performer> tPerformerMain = std::make_shared<PDir::Performer>("platomain","platomain");
    std::vector<std::shared_ptr<PDir::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<PDir::SharedData> tSharedData = std::make_shared<PDir::SharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    PDir::OperationAprepro tAprepro("match.yaml", {"l", "w", "d"},tSharedData, tPerformer, 2);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAprepro.write_definition(tDocument,""));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
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
        "aprepro_{E}", 
        "aprepro", 
        "evaluations_{E}",
        "true",
        "-q",
        "match.yaml.template",
        "match.yaml",
        "true",
        "l=",
        "w=",
        "d=",
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    auto tInput = tOperation.child("Input");
    ASSERT_FALSE(tInput.empty());
    tKeys = {"ArgumentName", "Layout", "Size"};
    tValues = {"parameters", "scalar", "3"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);
    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteInterfaceApreproOperationWithConcurrency)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    std::shared_ptr<PDir::Performer> tPerformerMain = std::make_shared<PDir::Performer>("platomain","platomain");
    std::vector<std::shared_ptr<PDir::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<PDir::SharedData> tSharedData = std::make_shared<PDir::SharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    PDir::OperationAprepro tAprepro("match.yaml", {"l", "w", "d"}, tSharedData, tPerformer, 2);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAprepro.write_interface(tDocument,"2"));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {
        "Name", 
        "PerformerName", 
        "Input"};
    std::vector<std::string> tValues = {
        "aprepro_2", 
        "plato_services_2", 
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    auto tInput = tOperation.child("Input");
    ASSERT_FALSE(tInput.empty());
    tKeys = {"ArgumentName", "SharedDataName"};
    tValues = {"parameters", "design_parameters_2"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);
    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteDefinitionApreproOperationNoConcurrency)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,0);
    std::shared_ptr<PDir::Performer> tPerformerMain = std::make_shared<PDir::Performer>("platomain","platomain");
    std::vector<std::shared_ptr<PDir::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<PDir::SharedData> tSharedData = std::make_shared<PDir::SharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,0);
    std::vector<std::string> vars = {"l","w","d"};

    PDir::OperationAprepro tAprepro("match.yaml", vars, tSharedData, tPerformer, 0);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAprepro.write_definition(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {"Function",
        "Name", 
        "Command", 
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
        "aprepro", 
        "aprepro", 
        "true",
        "-q",
        "match.yaml.template",
        "match.yaml",
        "true",
        "l=",
        "w=",
        "d=",
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    auto tInput = tOperation.child("Input");
    ASSERT_FALSE(tInput.empty());
    tKeys = {"ArgumentName", "Layout", "Size"};
    tValues = {"parameters", "scalar", "3"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);
    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteDefinitionHarvestDataOperationWithConcurrency)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    std::shared_ptr<PDir::Performer> tPerformerMain = std::make_shared<PDir::Performer>("platomain","platomain");
    std::vector<std::shared_ptr<PDir::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<PDir::SharedData> tSharedData = std::make_shared<PDir::SharedDataGlobal>("criterion value","3",tPerformerMain,tUserPerformers,2);
    
    PDir::OperationHarvestDataFunction tHarvestData("power_balance.dat", "max", "1", tSharedData, tPerformer, 3);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tHarvestData.write_definition(tDocument,"0"));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {"Function",
        "Name", 
        "File", 
        "Operation",
        "Column",
        "Output"};
    std::vector<std::string> tValues = {"HarvestDataFromFile", 
        "harvest_data_0", 
        "./evaluations_0/power_balance.dat",
        "max",
        "1",
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    auto tOutput = tOperation.child("Output");
    ASSERT_FALSE(tOutput.empty());
    tKeys = {"ArgumentName", "Layout", "Size"};
    tValues = {"criterion value", "scalar", "1"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOutput);
    tOutput = tOutput.next_sibling("Output");
    ASSERT_TRUE(tOutput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteInterfaceHarvestDataOperationWithTag)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    std::shared_ptr<PDir::Performer> tPerformerMain = std::make_shared<PDir::Performer>("platomain","platomain");
    std::vector<std::shared_ptr<PDir::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<PDir::SharedData> tSharedData = std::make_shared<PDir::SharedDataGlobal>("criterion_1_service_1_scenario_1","3",tPerformerMain,tUserPerformers,3);
    
    PDir::OperationHarvestDataFunction tHarvestData("power_balance.dat", "max", "1", tSharedData, tPerformer, 3);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tHarvestData.write_interface(tDocument,""));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {
        "Name", 
        "PerformerName",
        "Output"};
    std::vector<std::string> tValues = {
        "harvest_data_{E}", 
        "plato_services_{E}", 
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    auto tOutput = tOperation.child("Output");
    ASSERT_FALSE(tOutput.empty());
    tKeys = {"ArgumentName", "SharedDataName"};
    tValues = {"criterion value", "criterion_1_service_1_scenario_1_{E}"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOutput);
    tOutput = tOutput.next_sibling("Output");
    ASSERT_TRUE(tOutput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteDefinitionCubitTet10OperationWithTag)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    std::shared_ptr<PDir::Performer> tPerformerMain = std::make_shared<PDir::Performer>("platomain","platomain");
    std::vector<std::shared_ptr<PDir::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<PDir::SharedData> tSharedData = std::make_shared<PDir::SharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    PDir::OperationCubitTet10Conversion tCubit("myFile.exo", {"1", "3", "4"},tSharedData, tPerformer, 2);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tCubit.write_definition(tDocument,""));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {"Function",
        "Name", 
        "Command", 
        "ChDir",
        "OnChange",
        "Argument",
        "AppendInput",
        "Input"};
    std::vector<std::string> tValues = {"SystemCall", 
        "cubit_convert_to_tet10_{E}", 
        "cubit", 
        "evaluations_{E}",
        "true",
        " -input toTet10.jou -batch -nographics -nogui -noecho -nojournal -nobanner -information off",
        "false",
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    auto tInput = tOperation.child("Input");
    ASSERT_FALSE(tInput.empty());
    tKeys = {"ArgumentName"};
    tValues = {"parameters"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);
    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteInterfaceCubitTet10Operation)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    std::shared_ptr<PDir::Performer> tPerformerMain = std::make_shared<PDir::Performer>("platomain","platomain");
    std::vector<std::shared_ptr<PDir::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<PDir::SharedData> tSharedData = std::make_shared<PDir::SharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    PDir::OperationCubitTet10Conversion tCubit("myFile.exo", {"1", "3", "4"},tSharedData, tPerformer, 2);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tCubit.write_interface(tDocument,"2"));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {
        "Name", 
        "PerformerName", 
        "Input"};
    std::vector<std::string> tValues = {
        "cubit_convert_to_tet10_2", 
        "plato_services_2", 
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    auto tInput = tOperation.child("Input");
    ASSERT_FALSE(tInput.empty());
    tKeys = {"ArgumentName", "SharedDataName"};
    tValues = {"parameters", "design_parameters_2"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);
    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteCubitTet10OperationToTet10FileNoConcurrency)
{
    Plato::system("rm -rf toTet10.jou");
    Plato::system("rm -rf evaluations_0");
    Plato::system("rm -rf evaluations_1");

    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,0);
    std::shared_ptr<PDir::Performer> tPerformerMain = std::make_shared<PDir::Performer>("platomain","platomain");
    std::vector<std::shared_ptr<PDir::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<PDir::SharedData> tSharedData = std::make_shared<PDir::SharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,0);
    
    PDir::OperationCubitTet10Conversion tCubit("myFile.exo", {"1", "3", "4"},tSharedData, tPerformer, 0);
    
    auto tReadData = XMLGen::read_data_from_file("toTet10.jou");
    auto tGold = std::string("importmesh\"myFile.exo\"no_geom") + 
        std::string("block1elementtypetetra10") + 
        std::string("block3elementtypetetra10") + 
        std::string("block4elementtypetetra10") + 
        std::string("setexodusnetcdf4off") + 
        std::string("setlargeexodusfileon") + 
        std::string("exportmesh\"myFile.exo\"overwrite");

    EXPECT_STREQ(tReadData.str().c_str(),tGold.c_str());

    ASSERT_THROW(tReadData = XMLGen::read_data_from_file("evaluations_0/toTet10.jou"),std::runtime_error);

    Plato::system("rm -rf toTet10.jou");
}

TEST(PlatoTestXMLGenerator, WriteCubitTet10OperationToTet10File)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    std::shared_ptr<PDir::Performer> tPerformerMain = std::make_shared<PDir::Performer>("platomain","platomain");
    std::vector<std::shared_ptr<PDir::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<PDir::SharedData> tSharedData = std::make_shared<PDir::SharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    PDir::OperationCubitTet10Conversion tCubit("myFile.exo", {"1", "3", "4"},tSharedData, tPerformer, 2);
    
    auto tReadData = XMLGen::read_data_from_file("toTet10.jou");
    auto tGold = std::string("importmesh\"myFile.exo\"no_geom") + 
        std::string("block1elementtypetetra10") + 
        std::string("block3elementtypetetra10") + 
        std::string("block4elementtypetetra10") + 
        std::string("setexodusnetcdf4off") + 
        std::string("setlargeexodusfileon") + 
        std::string("exportmesh\"myFile.exo\"overwrite");

    EXPECT_STREQ(tReadData.str().c_str(),tGold.c_str());

    tReadData = XMLGen::read_data_from_file("evaluations_0/toTet10.jou");
    EXPECT_STREQ(tReadData.str().c_str(),tGold.c_str());

    tReadData = XMLGen::read_data_from_file("evaluations_1/toTet10.jou");
    EXPECT_STREQ(tReadData.str().c_str(),tGold.c_str());

    Plato::system("rm -rf toTet10.jou");
    Plato::system("rm -rf evaluations_0");
    Plato::system("rm -rf evaluations_1");

}

TEST(PlatoTestXMLGenerator, WriteDefinitionCubitSubBlockOperationWithTag)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    std::shared_ptr<PDir::Performer> tPerformerMain = std::make_shared<PDir::Performer>("platomain","platomain");
    std::vector<std::shared_ptr<PDir::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<PDir::SharedData> tSharedData = std::make_shared<PDir::SharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    PDir::OperationCubitSubBlock tCubit("myFile.exo", {"1", "3", "4"},tSharedData, tPerformer, 2);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tCubit.write_definition(tDocument,""));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {"Function",
        "Name", 
        "Command", 
        "ChDir",
        "OnChange",
        "Argument",
        "AppendInput",
        "Input"};
    std::vector<std::string> tValues = {"SystemCall", 
        "create_sub_block_{E}", 
        "cubit", 
        "evaluations_{E}",
        "true",
        " -input subBlock.jou -batch -nographics -nogui -noecho -nojournal -nobanner -information off",
        "false",
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    auto tInput = tOperation.child("Input");
    ASSERT_FALSE(tInput.empty());
    tKeys = {"ArgumentName"};
    tValues = {"design_parameters_{E}"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);
    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteInterfaceCubitSubBlockOperation)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    std::shared_ptr<PDir::Performer> tPerformerMain = std::make_shared<PDir::Performer>("platomain","platomain");
    std::vector<std::shared_ptr<PDir::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<PDir::SharedData> tSharedData = std::make_shared<PDir::SharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    PDir::OperationCubitSubBlock tCubit("myFile.exo", {"1", "3", "4"},tSharedData, tPerformer, 2);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tCubit.write_interface(tDocument,"2"));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {
        "Name", 
        "PerformerName", 
        "Input"};
    std::vector<std::string> tValues = {
        "create_sub_block_2", 
        "plato_services_2", 
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    auto tInput = tOperation.child("Input");
    ASSERT_FALSE(tInput.empty());
    tKeys = {"ArgumentName", "SharedDataName"};
    tValues = {"Parameters", "design_parameters_2"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);
    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteCubitSubBlockOperationSubBlockFile)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);
    std::shared_ptr<PDir::Performer> tPerformerMain = std::make_shared<PDir::Performer>("platomain","platomain");
    std::vector<std::shared_ptr<PDir::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<PDir::SharedData> tSharedData = std::make_shared<PDir::SharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    PDir::OperationCubitSubBlock tCubit("myFile.exo", {"-0.872", "-5", "-5", "0.872", "10", "10"},tSharedData, tPerformer, 2);
    
    auto tReadData = XMLGen::read_data_from_file("subBlock.jou");
    auto tGold = std::string("importmesh\"myFile.exo\"no_geom") + 
        std::string("deleteblockall") + 
        std::string("block2tetwithx_coord>=-0.872andy_coord>=-5andz_coord>=-5andx_coord<=0.872andy_coord<=10andz_coord<=10") + 
        std::string("block1tetall") + 
        std::string("setexodusnetcdf4off") + 
        std::string("setlargeexodusfileon") + 
        std::string("exportmesh\"myFile.exo\"overwrite");

    EXPECT_STREQ(tReadData.str().c_str(),tGold.c_str());

    tReadData = XMLGen::read_data_from_file("evaluations_0/subBlock.jou");
    EXPECT_STREQ(tReadData.str().c_str(),tGold.c_str());

    tReadData = XMLGen::read_data_from_file("evaluations_1/subBlock.jou");
    EXPECT_STREQ(tReadData.str().c_str(),tGold.c_str());

    Plato::system("rm -rf subBlock.jou");
    Plato::system("rm -rf evaluations_0");
    Plato::system("rm -rf evaluations_1");

}

TEST(PlatoTestXMLGenerator, WriteCubitSubBlockOperationSubBlockFileNoConcurrency)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,0);
    std::shared_ptr<PDir::Performer> tPerformerMain = std::make_shared<PDir::Performer>("platomain","platomain");
    std::vector<std::shared_ptr<PDir::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<PDir::SharedData> tSharedData = std::make_shared<PDir::SharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,0);
    
    PDir::OperationCubitSubBlock tCubit("myFile.exo", {"-0.872", "-5", "-5", "0.872", "10", "10"},tSharedData, tPerformer, 0);
    
    auto tReadData = XMLGen::read_data_from_file("subBlock.jou");
    auto tGold = std::string("importmesh\"myFile.exo\"no_geom") + 
        std::string("deleteblockall") + 
        std::string("block2tetwithx_coord>=-0.872andy_coord>=-5andz_coord>=-5andx_coord<=0.872andy_coord<=10andz_coord<=10") + 
        std::string("block1tetall") + 
        std::string("setexodusnetcdf4off") + 
        std::string("setlargeexodusfileon") + 
        std::string("exportmesh\"myFile.exo\"overwrite");

    EXPECT_STREQ(tReadData.str().c_str(),tGold.c_str());

    ASSERT_THROW(tReadData = XMLGen::read_data_from_file("evaluations_0/subBlock.jou"),std::runtime_error);

    Plato::system("rm -rf subBlock.jou");

}

TEST(PlatoTestXMLGenerator, WriteDefinitionDecompOperationWithTag)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);    
    PDir::OperationDecomp tDecomp("myFile.exo", 16 , tPerformer, 2);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tDecomp.write_definition(tDocument,""));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {"Function",
        "Name", 
        "Command", 
        "ChDir",
        "Argument",
        "Argument",
        "Argument",
        "Argument"};
    std::vector<std::string> tValues = {"SystemCall", 
        "decomp_mesh_{E}", 
        "decomp", 
        "evaluations_{E}",
        " -p ",
        "16",
        " ",
        "myFile.exo"};
    
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);
    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteInterfaceDecompOperationInterfaceFile)
{
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,2);    
    PDir::OperationDecomp tDecomp("myFile.exo", 16 , tPerformer, 2);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tDecomp.write_interface(tDocument,"2"));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {
        "Name", 
        "PerformerName"};
    std::vector<std::string> tValues = {
        "decomp_mesh_2", 
        "plato_services_2"};
    
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);
    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteDefinitionSetBoundsOperationLower)
{
    std::shared_ptr<PDir::Performer> tPerformerMain = std::make_shared<PDir::Performer>("platomain","platomain");
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,0);
    std::vector<std::shared_ptr<PDir::Performer>> tUserPerformers = {tPerformerMain,tPerformer};

    std::shared_ptr<PDir::SharedData> tInputSharedData = std::make_shared<PDir::SharedDataGlobal>("Lower Bound Value","1",tPerformerMain,tUserPerformers);
    std::shared_ptr<PDir::SharedData> tOutputSharedData = std::make_shared<PDir::SharedDataNodalField>("Lower Bound Vector",tPerformerMain,tUserPerformers);

    PDir::OperationSetBounds tSetBounds("Compute Lower Bounds",true,"solid","density",tInputSharedData,tOutputSharedData,tPerformerMain);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tSetBounds.write_definition(tDocument,""));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());

    std::vector<std::string> tKeys = {"Function",
        "Name", 
        "UseCase", 
        "Discretization",
        "Input",
        "Output"};
    std::vector<std::string> tValues = {"SetLowerBounds", 
        "Compute Lower Bounds", 
        "solid", 
        "density",
        "",
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    auto tInput = tOperation.child("Input");
    ASSERT_FALSE(tInput.empty());

    tKeys = {"ArgumentName"};
    tValues = {"Lower Bound Value"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);

    auto tOutput = tOperation.child("Output");
    ASSERT_FALSE(tOutput.empty());

    tKeys = {"ArgumentName"};
    tValues = {"Lower Bound Vector"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOutput);

    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    tOutput = tOutput.next_sibling("Output");
    ASSERT_TRUE(tOutput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteInterfaceSetBoundsInterfaceFileUpper)
{
    std::shared_ptr<PDir::Performer> tPerformerMain = std::make_shared<PDir::Performer>("platomain","platomain");
    std::shared_ptr<PDir::Performer> tPerformer = std::make_shared<PDir::Performer>("plato_services","plato_services",1,16,0);
    std::vector<std::shared_ptr<PDir::Performer>> tUserPerformers = {tPerformerMain,tPerformer};

    std::shared_ptr<PDir::SharedData> tInputSharedData = std::make_shared<PDir::SharedDataGlobal>("Upper Bound Value","1",tPerformerMain,tUserPerformers);
    std::shared_ptr<PDir::SharedData> tOutputSharedData = std::make_shared<PDir::SharedDataNodalField>("Upper Bound Vector",tPerformerMain,tUserPerformers);

    PDir::OperationSetBounds tSetBounds("Compute Upper Bounds",false,"solid","density",tInputSharedData,tOutputSharedData,tPerformerMain);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tSetBounds.write_interface(tDocument,""));
    ASSERT_FALSE(tDocument.empty());

    tDocument.save_file("testint.xml","  ");

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());

    std::vector<std::string> tKeys = {
        "Name", 
        "PerformerName",
        "Input",
        "Output"};
    std::vector<std::string> tValues = {
        "Compute Upper Bounds", 
        "platomain",
        "",
        ""};
    
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    auto tInput = tOperation.child("Input");
    ASSERT_FALSE(tInput.empty());

    tKeys = {"ArgumentName", "SharedDataName"};
    tValues = {"Upper Bound Value","Upper Bound Value"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);

    auto tOutput = tOperation.child("Output");
    ASSERT_FALSE(tOutput.empty());

    tKeys = {"ArgumentName","SharedDataName"};
    tValues = {"Upper Bound Vector","Upper Bound Vector"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOutput);

    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    tOutput = tOutput.next_sibling("Output");
    ASSERT_TRUE(tOutput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

}