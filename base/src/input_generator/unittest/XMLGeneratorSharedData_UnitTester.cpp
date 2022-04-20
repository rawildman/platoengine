 /*
 * XMLGeneratorPlatoGemma_UnitTester.cpp
 *
 *  Created on: April 12, 2022
 */
#include <gtest/gtest.h>

#include "pugixml.hpp"
#include "XMLGenerator_UnitTester_Tools.hpp"
#include "XMLGeneratorSharedData.hpp"
#include "XMLGeneratorPerformer.hpp"

namespace PlatoTestXMLGenerator
{

TEST(PlatoTestXMLGenerator, WriteDesignParametersSharedDataWithTag)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedData>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tSharedData->write_interface(tDocument));
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

TEST(PlatoTestXMLGenerator, WriteDesignParametersSharedData)
{
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",0);
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData = std::make_shared<XMLGen::XMLGeneratorSharedData>("design_parameters","3",tPerformerMain,tUserPerformers,2);
    
    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tSharedData->write_interface(tDocument,"2"));
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

}