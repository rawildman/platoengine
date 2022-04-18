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

TEST(PlatoTestXMLGenerator, WritePerformerWithTag)
{
    
    XMLGen::XMLGeneratorPerformer tPerformer("name","code",3);
    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tPerformer.write(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Performer");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Performer", tOperation.name());
    std::vector<std::string> tKeys = {
        "PerformerID",
        "Name", 
        "Code"};
    
    std::vector<std::string> tValues = {
        "{E+1}", 
        "name_{E}", 
        "code"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Performer");
    ASSERT_TRUE(tOperation.empty());
}


TEST(PlatoTestXMLGenerator, WritePerformer)
{
    
    XMLGen::XMLGeneratorPerformer tPerformer("name","code",3);
    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tPerformer.write(tDocument,"2"));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Performer");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Performer", tOperation.name());
    std::vector<std::string> tKeys = {
        "PerformerID",
        "Name", 
        "Code"};
    
    std::vector<std::string> tValues = {
        "3", 
        "name_2", 
        "code"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Performer");
    ASSERT_TRUE(tOperation.empty());
}



TEST(PlatoTestXMLGenerator, WriteSharedDataWithTag)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedData>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tSharedData->write(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("SharedData");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("SharedData", tOperation.name());
    std::vector<std::string> tKeys = {
        "Name", 
        "Type", 
        "Layout",
        "Size",
        "OwnerName",
        "UserName",
        "UserName"};
    
    std::vector<std::string> tValues = {
        "design_parameters_{E}", 
        "Scalar", 
        "Global",
        "3",
        "platomain",
        "platomain",
        "plato_services_{E}"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("SharedData");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteSharedData)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedData>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tSharedData->write(tDocument,"2"));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("SharedData");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("SharedData", tOperation.name());
    std::vector<std::string> tKeys = {
        "Name", 
        "Type", 
        "Layout",
        "Size",
        "OwnerName",
        "UserName",
        "UserName"};
    
    std::vector<std::string> tValues = {
        "design_parameters_2", 
        "Scalar", 
        "Global",
        "3",
        "platomain",
        "platomain",
        "plato_services_2"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("SharedData");
    ASSERT_TRUE(tOperation.empty());
}




TEST(PlatoTestXMLGenerator, WriteWaitOperation)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",2);
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

TEST(PlatoTestXMLGenerator, WriteWaitOperationNoEvaluation)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",2);
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

TEST(PlatoTestXMLGenerator, WriteWaitOperationWithTag)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",2);
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

TEST(PlatoTestXMLGenerator, WriteWaitOperationInterfaceWithTag)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",2);
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



TEST(PlatoTestXMLGenerator, WriteGemmaMPIOperation)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",2);
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

TEST(PlatoTestXMLGenerator, WriteGemmaMPIOperationNoEvaluations)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",2);
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

TEST(PlatoTestXMLGenerator, WriteGemmaMPIOperationWithTag)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",2);
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

TEST(PlatoTestXMLGenerator, WriteGemmaMPIOperationInterfaceFile)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",2);
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


TEST(PlatoTestXMLGenerator, WriteApreproOperation)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedData>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
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

TEST(PlatoTestXMLGenerator, WriteApreproOperationWithTag)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedData>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
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

TEST(PlatoTestXMLGenerator, WriteApreproOperationInterfaceFile)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedData>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
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


TEST(PlatoTestXMLGenerator, WriteApreproOperationNoEvaluations)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",0);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedData>("design_parameters","3",tPerformerMain,tUserPerformers,0);
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

TEST(PlatoTestXMLGenerator, WriteHarvestDataOperation)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedData>("criterion value","3",tPerformerMain,tUserPerformers,0);
    
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

TEST(PlatoTestXMLGenerator, WriteStage)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformersJustMain = {tPerformerMain};

    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tInputSharedData = std::make_shared<XMLGen::XMLGeneratorSharedData>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tOutputSharedData = std::make_shared<XMLGen::XMLGeneratorSharedData>("criterion value","1",tPerformer,tUserPerformersJustMain,2);
    
    std::vector<std::string> vars = {"l","w","d"};
    std::shared_ptr<XMLGen::XMLGeneratorOperationAprepro> tAprepro = std::make_shared<XMLGen::XMLGeneratorOperationAprepro>("match.yaml", vars, tInputSharedData, tPerformer, 2 );
                                                                
    std::shared_ptr<XMLGen::XMLGeneratorOperationGemmaMPISystemCall> tGemma = std::make_shared<XMLGen::XMLGeneratorOperationGemmaMPISystemCall>("match.yaml", "2", tPerformer, 2);

    XMLGen::XMLGeneratorStage tStage("Stage",tInputSharedData,tOutputSharedData);
    tStage.addStageOperation(tAprepro);
    tStage.addStageOperation(tGemma);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tStage.write(tDocument));
    ASSERT_FALSE(tDocument.empty());
    tDocument.save_file("dummy.txt", " ");
/*
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
    ASSERT_TRUE(tOperation.empty());*/
}



}