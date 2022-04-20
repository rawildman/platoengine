 /*
 * XMLGeneratorPlatoGemma_UnitTester.cpp
 *
 *  Created on: April 12, 2022
 */
#include <gtest/gtest.h>

#include "pugixml.hpp"
#include "XMLGenerator_UnitTester_Tools.hpp"
#include "XMLGeneratorFileObject.hpp"

namespace PlatoTestXMLGenerator
{

TEST(PlatoTestXMLGenerator, FileObjectTagFunctions)
{
    
    XMLGen::XMLGeneratorFileObject tFileObject("name",3);
    ASSERT_NO_THROW(tFileObject.name());
    ASSERT_STREQ("name_{E}", tFileObject.name().c_str());
    ASSERT_STREQ("name_{E}", tFileObject.name("").c_str());
    ASSERT_STREQ("name_1", tFileObject.name("1").c_str());
    ASSERT_EQ(3,tFileObject.evaluations());

    ASSERT_STREQ("{E}", tFileObject.tag().c_str());
    ASSERT_STREQ("{E}", tFileObject.tag("").c_str());
    ASSERT_STREQ("1", tFileObject.tag("1").c_str());
    ASSERT_STREQ("I", tFileObject.tag("I").c_str());
    ASSERT_STREQ("{I}", tFileObject.tag("{I}").c_str());

    pugi::xml_document tDocument;
    auto tForNode = tFileObject.for_node(tDocument,std::string("Parameters"));

    ASSERT_FALSE(tForNode.empty());
    ASSERT_STREQ("For", tForNode.name());
    
}

TEST(PlatoTestXMLGenerator, FileObjectNoConcurrency)
{
    
    XMLGen::XMLGeneratorFileObject tFileObject("name");
    ASSERT_NO_THROW(tFileObject.name());
    ASSERT_STREQ("name", tFileObject.name().c_str());
    ASSERT_STREQ("name", tFileObject.name("").c_str());
    ASSERT_STREQ("name", tFileObject.name("1").c_str());

    ASSERT_EQ(0,tFileObject.evaluations());

    ASSERT_STREQ("", tFileObject.tag().c_str());
    ASSERT_STREQ("", tFileObject.tag("").c_str());
    ASSERT_STREQ("", tFileObject.tag("1").c_str());

    pugi::xml_document tDocument;
    auto tForNode = tFileObject.for_node(tDocument,std::string("Parameters"));

    ASSERT_STREQ("", tForNode.name());
    
}

}