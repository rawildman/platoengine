 /*
 * XMLGeneratorPlatoGemma_UnitTester.cpp
 *
 *  Created on: April 12, 2022
 */
#include <gtest/gtest.h>

#include "pugixml.hpp"
#include "XMLGeneratorStage.hpp"
#include "XMLGeneratorOperation.hpp"
#include "XMLGeneratorSharedData.hpp"
#include "XMLGeneratorPerformer.hpp"
#include "XMLGeneratorDataStruct.hpp"
#include "XMLGeneratorParserUtilities.hpp"
#include "XMLGeneratorUtilities.hpp"
#include "XMLGenerator_UnitTester_Tools.hpp"

namespace PlatoTestXMLGenerator
{

TEST(PlatoTestXMLGenerator, WriteDefinitionWaitOperationWithConcurrency)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    XMLGen::XMLGeneratorOperationWait tWait("wait", "file", tPerformer, 1);

   // ASSERT_STREQ("wait_{E}", tWait.name());
    //ASSERT_STREQ("wait_1", tWait.name("1"));

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

TEST(PlatoTestXMLGenerator, OuterOperationForNodeWaitOperationWithConcurrency)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    XMLGen::XMLGeneratorOperationWait tWait("wait", "file", tPerformer, 2);

    //Need to copy FileObject unit test and add outer Operation loop    
    ASSERT_FALSE(true);

}



TEST(PlatoTestXMLGenerator, WriteDefinitionWaitOperationNoConcurrency)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    XMLGen::XMLGeneratorOperationWait tWait("wait", "file", tPerformer, 0);

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
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    XMLGen::XMLGeneratorOperationWait tWait("wait", "file", tPerformer,  1);

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
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    XMLGen::XMLGeneratorOperationWait tWait("wait", "file", tPerformer, 1);

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
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",numRanks,2);
    XMLGen::XMLGeneratorOperationGemmaMPISystemCall tGemma("match.yaml", "2", tPerformer, 1);

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
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    XMLGen::XMLGeneratorOperationGemmaMPISystemCall tGemma("match.yaml", "2", tPerformer, 0);

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
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    XMLGen::XMLGeneratorOperationGemmaMPISystemCall tGemma("match.yaml", "2", tPerformer, 1);

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
        "gemma", 
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
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    XMLGen::XMLGeneratorOperationGemmaMPISystemCall tGemma("match.yaml", "2", tPerformer, 1);

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
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain");
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    XMLGen::XMLGeneratorOperationAprepro tAprepro("match.yaml", {"l", "w", "d"},tSharedData, tPerformer, 2);

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
        "l",
        "w",
        "d",
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    auto tInput = tOperation.child("Input");
    ASSERT_FALSE(tInput.empty());
    tKeys = {"ArgumentName", "Layout", "Size"};
    tValues = {"design_parameters_2", "scalar", "3"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);
    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteDefinitionApreproOperationWithTag)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",16,0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    XMLGen::XMLGeneratorOperationAprepro tAprepro("match.yaml", {"l", "w", "d"},tSharedData, tPerformer, 2);

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
        "l",
        "w",
        "d",
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    auto tInput = tOperation.child("Input");
    ASSERT_FALSE(tInput.empty());
    tKeys = {"ArgumentName", "Layout", "Size"};
    tValues = {"design_parameters_{E}", "scalar", "3"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);
    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteInterfaceApreproOperationWithConcurrency)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",16,0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    XMLGen::XMLGeneratorOperationAprepro tAprepro("match.yaml", {"l", "w", "d"}, tSharedData, tPerformer, 2);

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
    tValues = {"Parameters", "design_parameters_2"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);
    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteDefinitionApreproOperationNoConcurrency)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,0);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",16,0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,0);
    std::vector<std::string> vars = {"l","w","d"};

    XMLGen::XMLGeneratorOperationAprepro tAprepro("match.yaml", vars, tSharedData, tPerformer, 0);

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
        "l",
        "w",
        "d",
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    auto tInput = tOperation.child("Input");
    ASSERT_FALSE(tInput.empty());
    tKeys = {"ArgumentName", "Layout", "Size"};
    tValues = {"design_parameters", "scalar", "3"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);
    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteDefinitionHarvestDataOperationWithConcurrency)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",16,0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("criterion value","3",tPerformerMain,tUserPerformers,2);
    
    XMLGen::XMLGeneratorOperationHarvestDataFunction tHarvestData("power_balance.dat", "max", "1", tSharedData, tPerformer, 3);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tHarvestData.write_definition(tDocument,"0"));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {"Function",
        "Name", 
        "ChDir",
        "File", 
        "Operation",
        "Column",
        "Output"};
    std::vector<std::string> tValues = {"HarvestDataFromFile", 
        "harvest_data_0", 
        "evaluations_0", 
        "power_balance.dat",
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
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",16,0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("criterion_1_service_1_scenario_1","3",tPerformerMain,tUserPerformers,3);
    
    XMLGen::XMLGeneratorOperationHarvestDataFunction tHarvestData("power_balance.dat", "max", "1", tSharedData, tPerformer, 3);

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
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",16,0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    XMLGen::XMLGeneratorOperationCubitTet10Conversion tCubit("myFile.exo", {"1", "3", "4"},tSharedData, tPerformer, 2);

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
    tValues = {"design_parameters_{E}"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);
    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteInterfaceCubitTet10Operation)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",16,0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    XMLGen::XMLGeneratorOperationCubitTet10Conversion tCubit("myFile.exo", {"1", "3", "4"},tSharedData, tPerformer, 2);

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
    tValues = {"Parameters", "design_parameters_2"};
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

    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,0);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",16,0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,0);
    
    XMLGen::XMLGeneratorOperationCubitTet10Conversion tCubit("myFile.exo", {"1", "3", "4"},tSharedData, tPerformer, 0);
    
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
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",16,0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    XMLGen::XMLGeneratorOperationCubitTet10Conversion tCubit("myFile.exo", {"1", "3", "4"},tSharedData, tPerformer, 2);
    
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
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",16,0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    XMLGen::XMLGeneratorOperationCubitSubBlock tCubit("myFile.exo", {"1", "3", "4"},tSharedData, tPerformer, 2);

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
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",16,0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    XMLGen::XMLGeneratorOperationCubitSubBlock tCubit("myFile.exo", {"1", "3", "4"},tSharedData, tPerformer, 2);

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
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",16,0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    XMLGen::XMLGeneratorOperationCubitSubBlock tCubit("myFile.exo", {"-0.872", "-5", "-5", "0.872", "10", "10"},tSharedData, tPerformer, 2);
    
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
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,0);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",16,0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("design_parameters","3",tPerformerMain,tUserPerformers,0);
    
    XMLGen::XMLGeneratorOperationCubitSubBlock tCubit("myFile.exo", {"-0.872", "-5", "-5", "0.872", "10", "10"},tSharedData, tPerformer, 0);
    
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
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);    
    XMLGen::XMLGeneratorOperationDecomp tDecomp("myFile.exo", 16 , tPerformer, 2);

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
    
    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteInterfaceDecompOperationInterfaceFile)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",16,2);    
    XMLGen::XMLGeneratorOperationDecomp tDecomp("myFile.exo", 16 , tPerformer, 2);

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
    
    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}


}