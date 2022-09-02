 /*
 * Stage_UnitTester.cpp
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

TEST(PlatoTestXMLGenerator, WriteStageOneWaitOperationNoConcurrencyNoSharedData)
{
    std::shared_ptr<director::Performer> tPerformer = std::make_shared<director::Performer>("plato_services","plato_services",16,0);
    std::shared_ptr<director::Performer> tPerformerMain = std::make_shared<director::Performer>("platomain","platomain",16,0);
    std::vector<std::shared_ptr<director::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    
    
    std::vector<std::shared_ptr<director::Operation>> tOperations;
    tOperations.push_back(std::make_shared<director::OperationWait> ("wait", "file", tPerformer, 0));
    director::Stage tStage("name",tOperations);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tStage.write(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tStageNode = tDocument.child("Stage");
    ASSERT_FALSE(tStageNode.empty());
    ASSERT_STREQ("Stage", tStageNode.name());

    auto tInput = tStageNode.child("Input");
    ASSERT_TRUE(tInput.empty());
    
    auto tOutput = tStageNode.child("Output");
    ASSERT_TRUE(tOutput.empty());

    PlatoTestXMLGenerator::test_children({"Name","Operation"}, {"name",""}, tStageNode);

    auto tOperation = tStageNode.child("Operation");
    ASSERT_FALSE(tOperation.empty());

    std::vector<std::string> tKeys = {"Name",
        "PerformerName"};
    std::vector<std::string> tValues = {"wait", 
        "plato_services"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());

    tStageNode = tStageNode.next_sibling("Stage");
    ASSERT_TRUE(tStageNode.empty());

}

TEST(PlatoTestXMLGenerator, WriteStageOneWaitOperationWithConcurrencyNoSharedData)
{
    std::shared_ptr<director::Performer> tPerformer = std::make_shared<director::Performer>("plato_services","plato_services",1,16,2);
    std::shared_ptr<director::Performer> tPerformerMain = std::make_shared<director::Performer>("platomain","platomain");
    std::vector<std::shared_ptr<director::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    
    std::vector<std::shared_ptr<director::Operation>> tOperations;
    tOperations.push_back(std::make_shared<director::OperationWait> ("wait", "file", tPerformer, 2));
    director::Stage tStage("name",tOperations);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tStage.write(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tStageNode = tDocument.child("Stage");
    ASSERT_FALSE(tStageNode.empty());
    ASSERT_STREQ("Stage", tStageNode.name());

    auto tInput = tStageNode.child("Input");
    ASSERT_TRUE(tInput.empty());
    
    auto tOutput = tStageNode.child("Output");
    ASSERT_TRUE(tOutput.empty());

    PlatoTestXMLGenerator::test_children({"Name","Operation"}, {"name",""}, tStageNode);

    auto tOuterOperation = tStageNode.child("Operation");
    ASSERT_FALSE(tOuterOperation.empty());

    auto tForNode = tOuterOperation.child("For");
    ASSERT_FALSE(tForNode.empty());

    PlatoTestXMLGenerator::test_attributes({"var", "in"}, {"E", "Parameters"}, tForNode);

    auto tOperation = tForNode.child("Operation");
    ASSERT_FALSE(tOperation.empty());

    std::vector<std::string> tKeys = {"Name",
        "PerformerName"};
    std::vector<std::string> tValues = {"wait_{E}", 
        "plato_services_{E}"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());

    tForNode = tForNode.next_sibling("For");
    ASSERT_TRUE(tForNode.empty());

    tOuterOperation = tOuterOperation.next_sibling("Operation");
    ASSERT_TRUE(tOuterOperation.empty());

    tStageNode = tStageNode.next_sibling("Stage");
    ASSERT_TRUE(tStageNode.empty());

}

TEST(PlatoTestXMLGenerator, WriteStageTwoWaitOperationsNoConcurrencyNoSharedData)
{
    std::shared_ptr<director::Performer> tPerformer = std::make_shared<director::Performer>("plato_services","plato_services",16,0);
    std::shared_ptr<director::Performer> tPerformerMain = std::make_shared<director::Performer>("platomain","platomain",16,0);
    std::vector<std::shared_ptr<director::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    
    std::vector<std::shared_ptr<director::Operation>> tOperations;
    tOperations.push_back(std::make_shared<director::OperationWait> ("wait1", "file", tPerformer, 0));
    tOperations.push_back(std::make_shared<director::OperationWait> ("wait2", "file", tPerformer, 0));
    director::Stage tStage("name",tOperations);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tStage.write(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tStageNode = tDocument.child("Stage");
    ASSERT_FALSE(tStageNode.empty());
    ASSERT_STREQ("Stage", tStageNode.name());

    PlatoTestXMLGenerator::test_children({"Name","Operation","Operation"}, {"name","",""}, tStageNode);

    auto tOperation = tStageNode.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    
    std::vector<std::string> tKeys = {"Name",
        "PerformerName"};
    std::vector<std::string> tValues = {"wait1", 
        "plato_services"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_FALSE(tOperation.empty());
    
    tKeys = {"Name",
        "PerformerName"};
    tValues = {"wait2", 
        "plato_services"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());

    tStageNode = tStageNode.next_sibling("Stage");
    ASSERT_TRUE(tStageNode.empty());

}

TEST(PlatoTestXMLGenerator, WriteStageNoOperationNoConcurrencyInputSharedData)
{
    std::shared_ptr<director::Performer> tPerformer = std::make_shared<director::Performer>("plato_services","plato_services",16,0);
    std::shared_ptr<director::Performer> tPerformerMain = std::make_shared<director::Performer>("platomain","platomain",16,0);
    std::vector<std::shared_ptr<director::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    
    std::shared_ptr<director::SharedData> tInputSharedData = std::make_shared<director::SharedDataGlobal>("input","3",tPerformerMain,tUserPerformers);
  
    std::vector<std::shared_ptr<director::Operation>> tOperations;
    
    director::Stage tStage("name",tOperations,tInputSharedData,nullptr);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tStage.write(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tStageNode = tDocument.child("Stage");
    ASSERT_FALSE(tStageNode.empty());
    ASSERT_STREQ("Stage", tStageNode.name());

    PlatoTestXMLGenerator::test_children({"Name","Input"}, {"name",""}, tStageNode);

    auto tInput = tStageNode.child("Input");
    ASSERT_FALSE(tInput.empty());
    
    std::vector<std::string> tKeys = {"SharedDataName"};
    std::vector<std::string> tValues = {"input"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);

    tInput = tInput.next_sibling("Operation");
    ASSERT_TRUE(tInput.empty());

    auto tOutput = tStageNode.child("Output");
    ASSERT_TRUE(tOutput.empty());

    tStageNode = tStageNode.next_sibling("Stage");
    ASSERT_TRUE(tStageNode.empty());
}

TEST(PlatoTestXMLGenerator, WriteStageNoOperationNoConcurrencyOutputSharedData)
{
    std::shared_ptr<director::Performer> tPerformer = std::make_shared<director::Performer>("plato_services","plato_services",16,0);
    std::shared_ptr<director::Performer> tPerformerMain = std::make_shared<director::Performer>("platomain","platomain",16,0);
    std::vector<std::shared_ptr<director::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    
    std::shared_ptr<director::SharedData> tOutputSharedData = std::make_shared<director::SharedDataGlobal>("output","3",tPerformerMain,tUserPerformers);

    std::vector<std::shared_ptr<director::Operation>> tOperations;
    
    director::Stage tStage("name",tOperations,nullptr,tOutputSharedData);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tStage.write(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tStageNode = tDocument.child("Stage");
    ASSERT_FALSE(tStageNode.empty());
    ASSERT_STREQ("Stage", tStageNode.name());

    PlatoTestXMLGenerator::test_children({"Name","Output"}, {"name",""}, tStageNode);

    auto tOutput = tStageNode.child("Output");
    ASSERT_FALSE(tOutput.empty());
    
    std::vector<std::string> tKeys = {"SharedDataName"};
    std::vector<std::string> tValues = {"output"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOutput);

    tOutput = tOutput.next_sibling("Operation");
    ASSERT_TRUE(tOutput.empty());

    auto tInput = tStageNode.child("Input");
    ASSERT_TRUE(tInput.empty());

    tStageNode = tStageNode.next_sibling("Stage");
    ASSERT_TRUE(tStageNode.empty());
}

TEST(PlatoTestXMLGenerator, WriteStageNoOperationNoConcurrencyInputAndOutputSharedData)
{
    std::shared_ptr<director::Performer> tPerformer = std::make_shared<director::Performer>("plato_services","plato_services",16,0);
    std::shared_ptr<director::Performer> tPerformerMain = std::make_shared<director::Performer>("platomain","platomain",16,0);
    std::vector<std::shared_ptr<director::Performer>> tUserPerformers = {tPerformerMain,tPerformer};
    
    std::shared_ptr<director::SharedData> tInputSharedData = std::make_shared<director::SharedDataGlobal>("input","3",tPerformerMain,tUserPerformers);
    std::shared_ptr<director::SharedData> tOutputSharedData = std::make_shared<director::SharedDataGlobal>("output","3",tPerformerMain,tUserPerformers);

    std::vector<std::shared_ptr<director::Operation>> tOperations;
    
    director::Stage tStage("name",tOperations,tInputSharedData,tOutputSharedData);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tStage.write(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tStageNode = tDocument.child("Stage");
    ASSERT_FALSE(tStageNode.empty());
    ASSERT_STREQ("Stage", tStageNode.name());

    PlatoTestXMLGenerator::test_children({"Name","Input","Output"}, {"name","",""}, tStageNode);

    auto tInput = tStageNode.child("Input");
    ASSERT_FALSE(tInput.empty());
    
    std::vector<std::string> tKeys = {"SharedDataName"};
    std::vector<std::string> tValues = {"input"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);

    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    auto tOutput = tStageNode.child("Output");
    ASSERT_FALSE(tOutput.empty());

    tValues = {"output"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOutput);

    tOutput = tOutput.next_sibling("Output");
    ASSERT_TRUE(tOutput.empty());

    tStageNode = tStageNode.next_sibling("Stage");
    ASSERT_TRUE(tStageNode.empty());

}


}