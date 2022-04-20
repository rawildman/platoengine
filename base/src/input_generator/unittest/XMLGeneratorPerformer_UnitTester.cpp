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

TEST(PlatoTestXMLGenerator, WritePerformer)
{
    int numRanks = 16;
    XMLGen::XMLGeneratorPerformer tPerformer("name","code",numRanks,3);

    ASSERT_EQ(numRanks , tPerformer.numberRanks());
    ASSERT_EQ("1" , tPerformer.ID(1,"0"));
    ASSERT_EQ("2" , tPerformer.ID(2,"0"));
    ASSERT_EQ("{E+2}" , tPerformer.ID(2,""));
    ASSERT_EQ("{E+1}" , tPerformer.ID());

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tPerformer.write_interface(tDocument,1,"2"));
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

TEST(PlatoTestXMLGenerator, WritePerformerWithTag)
{
    
    int numRanks = 16;
    XMLGen::XMLGeneratorPerformer tPerformer("name","code",numRanks,3);
    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tPerformer.write_interface(tDocument));
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

    ASSERT_EQ(numRanks , tPerformer.numberRanks());
}


TEST(PlatoTestXMLGenerator, WritePerformerWithTagAndOffset)
{
    
    int numRanks = 16;
    XMLGen::XMLGeneratorPerformer tPerformer("name","code",numRanks,3);
    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tPerformer.write_interface(tDocument,4,""));
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
        "{E+4}", 
        "name_{E}", 
        "code"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);

    tOperation = tOperation.next_sibling("Performer");
    ASSERT_TRUE(tOperation.empty());

    ASSERT_EQ(numRanks , tPerformer.numberRanks());
}



}