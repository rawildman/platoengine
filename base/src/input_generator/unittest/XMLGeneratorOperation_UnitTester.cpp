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
    pugi::xml_document tDocument;
    
    XMLGen::XMLGeneratorOperationWait tWait("Wait","file","0");

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
        "OnChange" 
        };
    std::vector<std::string> tValues = {"SystemCall", 
        "Wait_0", 
        "while lsof -u $USER | grep ./file; do sleep 1; done", 
        "evaluations_0",
        "false"
        };
    
    
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);
    
    ASSERT_TRUE(tOperation.empty());

}


TEST(PlatoTestXMLGenerator, WriteWaitOperationNoEvaluation)
{
    pugi::xml_document tDocument;
    
    XMLGen::XMLGeneratorOperationWait tWait("Wait","file","");

    ASSERT_NO_THROW(tWait.write(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {"Function",
        "Name", 
        "Command", 
        "OnChange" 
        };
    std::vector<std::string> tValues = {"SystemCall", 
        "Wait", 
        "while lsof -u $USER | grep ./file; do sleep 1; done", 
        "false"
        };
    
    
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);
    
    ASSERT_TRUE(tOperation.empty());

}

TEST(PlatoTestXMLGenerator, WriteGemmaMPIOperation)
{
    pugi::xml_document tDocument;
    
    XMLGen::XMLGeneratorOperationGemmaMPISystemCall tGemma("match.yaml","2","1");

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
        "AppendInput" 
        };
    std::vector<std::string> tValues = {"SystemCallMPI", 
        "gemma_1", 
        "gemma", 
        "evaluations_1",
        "false",
        "2",
        "match.yaml",
        "false"
        };
    
    
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);
    
    ASSERT_TRUE(tOperation.empty());

}



TEST(PlatoTestXMLGenerator, WriteGemmaMPIOperationNoEvaluations)
{
    pugi::xml_document tDocument;
    
    XMLGen::XMLGeneratorOperationGemmaMPISystemCall tGemma("match.yaml","2","");

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
        "AppendInput" 
        };
    std::vector<std::string> tValues = {"SystemCallMPI", 
        "gemma", 
        "gemma", 
        "false",
        "2",
        "match.yaml",
        "false"
        };
    
    
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);
    
    ASSERT_TRUE(tOperation.empty());

}



TEST(PlatoTestXMLGenerator, WriteApreproOperation)
{
    pugi::xml_document tDocument;
    
    XMLGen::XMLGeneratorOperationAprepro tAprepro("match.yaml",{"l","w","d"},"2");

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
        "l",
        "w",
        "d",
        ""
        };
    
    
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);
    
    auto tInput = tOperation.child("Input");
    ASSERT_FALSE(tInput.empty());
    tKeys = {"ArgumentName","Layout","Size"};
    tValues = {"parameters_2","scalar","3"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);
    ASSERT_TRUE(tOperation.empty());

}




/*
// use case: gemma call is defined by a system call mpi operation
    XMLGen::InputData tInputMetaData;
    // define criterion
    XMLGen::Criterion tCriterionOne;
    tCriterionOne.id("1");
    tCriterionOne.type("volume");
    tInputMetaData.append(tCriterionOne);
    XMLGen::Criterion tCriterionTwo;
    tCriterionTwo.id("2");
    tCriterionTwo.type("system_call");
    tCriterionTwo.append("data_group", "column_1");
    tCriterionTwo.append("data_extraction_operation", "max");
    tCriterionTwo.append("data_file", "matched_power_balance.dat");
    tInputMetaData.append(tCriterionTwo);
    XMLGen::Criterion tCriterionThree;
    tCriterionThree.id("3");
    tCriterionThree.type("system_call");
    tCriterionThree.append("data_group", "column_2");
    tCriterionThree.append("data_extraction_operation", "max");
    tInputMetaData.append(tCriterionThree);
    // define objective
    XMLGen::Objective tObjective;
    tObjective.criteriaIDs.push_back("1");
    tObjective.criteriaIDs.push_back("2");
    tInputMetaData.objective = tObjective;
    // define constraints
    XMLGen::Constraint tConstraint;
    tConstraint.criterion("3");
    tInputMetaData.constraints.push_back(tConstraint);
    // define optimization parameters 
    XMLGen::OptimizationParameters tOptParams;
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    tOptParams.descriptors(tDescriptors);
    tOptParams.append("concurrent_evaluations", "2");
    tInputMetaData.set(tOptParams);
    // service parameters
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tServiceOne.append("type", "system_call");
    tServiceOne.append("number_processors", "1");
    tInputMetaData.append(tServiceOne);
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "platomain");
    tServiceTwo.append("id", "2");
    tServiceTwo.append("type", "plato_app");
    tServiceTwo.append("number_processors", "2");
    tInputMetaData.append(tServiceTwo);
      
    pugi::xml_document tDocument;
    XMLGen::XMLGeneratorOperationAprepro tAprepro(tInputMetaData,0,0);
    ASSERT_NO_THROW(tAprepro.write(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOperation = tDocument.child("Operation");
    ASSERT_FALSE(tOperation.empty());
    ASSERT_STREQ("Operation", tOperation.name());
    std::vector<std::string> tKeys = {"Function", "Name", 
        "Command", 
        "ChDir",
        "OnChange", 
        "Argument", "Argument", "Argument",
        "AppendInput",
        "Option","Option","Option",
         "Input"};
    std::vector<std::string> tValues = {"SystemCall", "aprepro_0", 
        "aprepro", 
        "evaluations_0/",
        "true", 
        "-q", "gemma_matched_power_balance_input_deck.yaml.template", "gemma_matched_power_balance_input_deck.yaml",
        "true",
        "slot_length", "slot_width", "slot_depth",
        ""};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOperation);
    auto tInput = tOperation.child("Input");
    ASSERT_FALSE(tInput.empty());
    tKeys = {"ArgumentName","Layout","Size"};
    tValues = {"parameters_0","scalar","3"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tInput);

    ASSERT_TRUE(tOperation.empty());
*/


}