 /*
 * XMLGeneratorPlatoGemma_UnitTester.cpp
 *
 *  Created on: April 12, 2022
 */
#include <gtest/gtest.h>

#include "pugixml.hpp"
#include "XMLGeneratorOperation.hpp"
#include "XMLGeneratorDataStruct.hpp"
#include "XMLGeneratorParserUtilities.hpp"
#include "XMLGeneratorUtilities.hpp"
#include "XMLGenerator_UnitTester_Tools.hpp"

namespace PlatoTestXMLGenerator
{

TEST(PlatoTestXMLGenerator, WriteWaitOperation)
{
    XMLGen::XMLGeneratorOperationWait tWait("wait", "file", "0");

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tWait.write(tDocument));
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
    XMLGen::XMLGeneratorOperationWait tWait("wait", "file", "");

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tWait.write(tDocument));
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

TEST(PlatoTestXMLGenerator, WriteGemmaMPIOperation)
{
    XMLGen::XMLGeneratorOperationGemmaMPISystemCall tGemma("match.yaml", "2", "1");

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tGemma.write(tDocument));
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
    XMLGen::XMLGeneratorOperationGemmaMPISystemCall tGemma("match.yaml", "2", "");

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tGemma.write(tDocument));
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

TEST(PlatoTestXMLGenerator, WriteApreproOperation)
{
    XMLGen::XMLGeneratorOperationAprepro tAprepro("match.yaml", {"l", "w", "d"}, "2");

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAprepro.write(tDocument));
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
    tValues = {"parameters_2", "scalar", "3"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);
    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteApreproOperationNoEvaluations)
{
    XMLGen::XMLGeneratorOperationAprepro tAprepro("match.yaml", {"l", "w", "d"}, "");

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAprepro.write(tDocument));
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
    tValues = {"parameters", "scalar", "3"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);
    tInput = tInput.next_sibling("Input");
    ASSERT_TRUE(tInput.empty());

    tOperation = tOperation.next_sibling("Operation");
    ASSERT_TRUE(tOperation.empty());
}

TEST(PlatoTestXMLGenerator, WriteHarvestDataOperation)
{
    XMLGen::XMLGeneratorOperationHarvestDataFunction tHarvestData("power_balance.dat", "max", "1", "0");

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tHarvestData.write(tDocument));
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

}