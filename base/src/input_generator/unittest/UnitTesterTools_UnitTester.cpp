 /*
 * UnitTesterTools_UnitTester.cpp
 *
 *  Created on: April 12, 2022
 */
#include <gtest/gtest.h>

#include "pugixml.hpp"
#include "XMLGenerator_UnitTester_Tools.hpp"
#include "XMLGeneratorUtilities.hpp"

namespace PlatoTestXMLGenerator
{
    using namespace XMLGen;

TEST(PlatoTestXMLGenerator, TestChildrenABXY)
{
    pugi::xml_document tDocument;
    auto tOperationNode = tDocument.append_child("Operation");
    addChild(tOperationNode, "File", "test");
    addChild(tOperationNode, "File2", "test2");

    std::vector<std::string> tKeys = {
        "File", 
        "File2"};
    std::vector<std::string> tValues = {
        "test", 
        "test2"};
    PlatoTestXMLGenerator::test_children(tKeys,tValues, tOperationNode);
}


TEST(PlatoTestXMLGenerator, TestChildrenAAXY)
{
    pugi::xml_document tDocument;
    auto tOperationNode = tDocument.append_child("Operation");
    addChild(tOperationNode, "File", "test");
    addChild(tOperationNode, "File", "test2");

    std::vector<std::string> tKeys = {
        "File", 
        "File"};
    std::vector<std::string> tValues = {
        "test", 
        "test2"};
    PlatoTestXMLGenerator::test_children(tKeys,tValues, tOperationNode);
}


TEST(PlatoTestXMLGenerator, TestChildrenABXX)
{
    pugi::xml_document tDocument;
    auto tOperationNode = tDocument.append_child("Operation");
    addChild(tOperationNode, "File", "test");
    addChild(tOperationNode, "File2", "test");

    std::vector<std::string> tKeys = {
        "File", 
        "File2"};
    std::vector<std::string> tValues = {
        "test", 
        "test"};
    PlatoTestXMLGenerator::test_children(tKeys,tValues, tOperationNode);
    
    tKeys = {
        "File2", 
        "File"};
    tValues = {
        "test", 
        "test"};
    PlatoTestXMLGenerator::test_children(tKeys,tValues, tOperationNode);


}


TEST(PlatoTestXMLGenerator, TestChildrenABAXYZ)
{
    pugi::xml_document tDocument;
    auto tOperationNode = tDocument.append_child("Operation");
    addChild(tOperationNode, "File", "test");
    addChild(tOperationNode, "File2", "test2");
    addChild(tOperationNode, "File", "test3");

    std::vector<std::string> tKeys = {
        "File", 
        "File2",
        "File"};
    std::vector<std::string> tValues = {
        "test", 
        "test2",
        "test3"};
    EXPECT_ANY_THROW(PlatoTestXMLGenerator::test_children(tKeys,tValues, tOperationNode));
}



}
  