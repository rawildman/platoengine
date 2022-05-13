 /*
 * UnitTesterTools_UnitTester.cpp
 *
 *  Created on: April 12, 2022
 */
#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>

#include "pugixml.hpp"
#include "XMLGenerator_UnitTester_Tools.hpp"
#include "XMLGeneratorUtilities.hpp"

/*
#define FAIL_TO_PASS(statement)\
do {\
  ::testing::TestPartResultArray gtest_failures; \
  ::testing::ScopedFakeTestPartResultReporter gtest_reporter( \
    ::testing::ScopedFakeTestPartResultReporter::  \
    INTERCEPT_ONLY_CURRENT_THREAD, &gtest_failures);  \
  if (::testing::internal::AlwaysTrue()) {                        \
        statement;                                                    \
       }   \
  std::cout<<gtest_failures.size()<<std::endl; \
  ASSERT_EQ(gtest_failures.size(),1); \
} while (::testing::internal::AlwaysFalse());\
*/

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
    PlatoTestXMLGenerator::test_children(tKeys,tValues, tOperationNode);
}

TEST(PlatoTestXMLGenerator, TestChildrenExpectFailToPassABXX)
{
    //Keys are flipped from gold
    pugi::xml_document tDocument;
    auto tOperationNode = tDocument.append_child("Operation");
    addChild(tOperationNode, "File", "test");
    addChild(tOperationNode, "File2", "test");

    std::vector<std::string> tKeys = {
        "File2", 
        "File"};
    std::vector<std::string> tValues = {
        "test", 
        "test"};
   
    unsigned int fails=0;
    do {
        ::testing::TestPartResultArray gtest_failures;
        ::testing::ScopedFakeTestPartResultReporter gtest_reporter(
        ::testing::ScopedFakeTestPartResultReporter::
        INTERCEPT_ONLY_CURRENT_THREAD, &gtest_failures);
        PlatoTestXMLGenerator::test_children(tKeys,tValues, tOperationNode);
        fails += gtest_failures.size();
        std::cout<<gtest_failures.size()<<std::endl;
    } while (::testing::internal::AlwaysFalse());

    ASSERT_GT(fails,0);
}

TEST(PlatoTestXMLGenerator, TestChildrenExpectFailToPassAAXY)
{
    //Values are flipped from gold
    pugi::xml_document tDocument;
    auto tOperationNode = tDocument.append_child("Operation");
    addChild(tOperationNode, "File", "test2");
    addChild(tOperationNode, "File", "test");

    std::vector<std::string> tKeys = {
        "File", 
        "File"};
    std::vector<std::string> tValues = {
        "test", 
        "test2"};
       
    unsigned int fails=0;
    do {
        ::testing::TestPartResultArray gtest_failures;
        ::testing::ScopedFakeTestPartResultReporter gtest_reporter(
        ::testing::ScopedFakeTestPartResultReporter::
        INTERCEPT_ONLY_CURRENT_THREAD, &gtest_failures);
        PlatoTestXMLGenerator::test_children(tKeys,tValues, tOperationNode);
        fails += gtest_failures.size();
        std::cout<<gtest_failures.size()<<std::endl;
    } while (::testing::internal::AlwaysFalse());

    ASSERT_GT(fails,0);
}

TEST(PlatoTestXMLGenerator, TestChildrenExpectFailToPassABAXYY)
{
    //Duplicate parent, different children
    pugi::xml_document tDocument;
    auto tOperationNode = tDocument.append_child("Operation");
    addChild(tOperationNode, "File", "test");
    addChild(tOperationNode, "File2", "test2");
    addChild(tOperationNode, "File", "test");

    std::vector<std::string> tKeys = {
        "File", 
        "File2",
        "File"};
    std::vector<std::string> tValues = {
        "test", 
        "test2",
        "test3"};
       
    unsigned int fails=0;
    do {
        ::testing::TestPartResultArray gtest_failures;
        ::testing::ScopedFakeTestPartResultReporter gtest_reporter(
        ::testing::ScopedFakeTestPartResultReporter::
        INTERCEPT_ONLY_CURRENT_THREAD, &gtest_failures);
        PlatoTestXMLGenerator::test_children(tKeys,tValues, tOperationNode);
        fails += gtest_failures.size();
        std::cout<<gtest_failures.size()<<std::endl;
    } while (::testing::internal::AlwaysFalse());

    ASSERT_GT(fails,0);
}


TEST(PlatoTestXMLGenerator, TestChildrenExpectFailToPassABCXYX)
{
    //Unique parents, 1 duplicate child
    pugi::xml_document tDocument;
    auto tOperationNode = tDocument.append_child("Operation");
    addChild(tOperationNode, "File", "test");
    addChild(tOperationNode, "File2", "test2");
    addChild(tOperationNode, "File3", "test3");

    std::vector<std::string> tKeys = {
        "File", 
        "File2",
        "File3"};
    std::vector<std::string> tValues = {
        "test", 
        "test2",
        "test"};
     
    unsigned int fails=0;
    do {
        ::testing::TestPartResultArray gtest_failures;
        ::testing::ScopedFakeTestPartResultReporter gtest_reporter(
        ::testing::ScopedFakeTestPartResultReporter::
        INTERCEPT_ONLY_CURRENT_THREAD, &gtest_failures);
        PlatoTestXMLGenerator::test_children(tKeys,tValues, tOperationNode);
        fails += gtest_failures.size();
        std::cout<<gtest_failures.size()<<std::endl;
    } while (::testing::internal::AlwaysFalse());

    ASSERT_GT(fails,0);
}


TEST(PlatoTestXMLGenerator, TestChildrenExpectFailToPassTooManyKeyValues)
{
    //Unique parents, 1 duplicate child
    pugi::xml_document tDocument;
    auto tOperationNode = tDocument.append_child("Operation");
    addChild(tOperationNode, "File", "test");
    addChild(tOperationNode, "File2", "test2");
    

    std::vector<std::string> tKeys = {
        "File", 
        "File2",
        "File3"};
    std::vector<std::string> tValues = {
        "test", 
        "test2",
        "test3"};
     
    unsigned int fails=0;
    do {
        ::testing::TestPartResultArray gtest_failures;
        ::testing::ScopedFakeTestPartResultReporter gtest_reporter(
        ::testing::ScopedFakeTestPartResultReporter::
        INTERCEPT_ONLY_CURRENT_THREAD, &gtest_failures);
        PlatoTestXMLGenerator::test_children(tKeys,tValues, tOperationNode);
        fails += gtest_failures.size();
        std::cout<<gtest_failures.size()<<std::endl;
    } while (::testing::internal::AlwaysFalse());

    ASSERT_GT(fails,0);
}


TEST(PlatoTestXMLGenerator, TestChildrenExpectFailToPassTooFewKeyValues)
{
    //Unique parents, 1 duplicate child
    pugi::xml_document tDocument;
    auto tOperationNode = tDocument.append_child("Operation");
    addChild(tOperationNode, "File", "test");
    addChild(tOperationNode, "File2", "test2");
    
    std::vector<std::string> tKeys = {
        "File"};
    std::vector<std::string> tValues = {
        "test"};
     
    unsigned int fails=0;
    do {
        ::testing::TestPartResultArray gtest_failures;
        ::testing::ScopedFakeTestPartResultReporter gtest_reporter(
        ::testing::ScopedFakeTestPartResultReporter::
        INTERCEPT_ONLY_CURRENT_THREAD, &gtest_failures);
        PlatoTestXMLGenerator::test_children(tKeys,tValues, tOperationNode);
        fails += gtest_failures.size();
        std::cout<<gtest_failures.size()<<std::endl;
    } while (::testing::internal::AlwaysFalse());

    ASSERT_GT(fails,0);
}


TEST(PlatoTestXMLGenerator, TestChildrenExpectFailToPassEmptyNode)
{
    
    pugi::xml_document tDocument;
    auto tOperationNode = tDocument.append_child("Operation");
    
    std::vector<std::string> tKeys = {
        "File"};
    std::vector<std::string> tValues = {
        "test"};
     
    unsigned int fails=0;
    do {
        ::testing::TestPartResultArray gtest_failures;
        ::testing::ScopedFakeTestPartResultReporter gtest_reporter(
        ::testing::ScopedFakeTestPartResultReporter::
        INTERCEPT_ONLY_CURRENT_THREAD, &gtest_failures);
        PlatoTestXMLGenerator::test_children(tKeys,tValues, tOperationNode);
        fails += gtest_failures.size();
        std::cout<<gtest_failures.size()<<std::endl;
    } while (::testing::internal::AlwaysFalse());

    ASSERT_GT(fails,0);
}

}
  