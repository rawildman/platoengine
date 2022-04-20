 /*
 * XMLGeneratorPlatoGemma_UnitTester.cpp
 *
 *  Created on: April 12, 2022
 */
#include <gtest/gtest.h>

#include "pugixml.hpp"
#include "XMLGenerator_UnitTester_Tools.hpp"
#include "XMLGeneratorPerformer.hpp"

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

}