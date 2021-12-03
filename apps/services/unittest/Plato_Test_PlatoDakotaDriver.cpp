/*
 * Plato_Test_PlatoDakotaDriver.cpp
 *
 *  Created on: Jul 27, 2021
 */

#include <gtest/gtest.h>

#include <memory>
#include <iterator>
#include <algorithm>
#include <unordered_map>

// Dakota includes
#include "PRPMultiIndex.hpp"
#include "ProblemDescDB.hpp"
#include "DakotaResponse.hpp"
#include "DakotaVariables.hpp"
#include "ParamResponsePair.hpp"
#include "LibraryEnvironment.hpp"
#include "DirectApplicInterface.hpp"
#include "PluginSerialDirectApplicInterface.hpp"

// trilinos includes
#include <Teuchos_ParameterList.hpp>
#include <Teuchos_SerialDenseVector.hpp>

// Plato includes
#include "Plato_DakotaDriver.hpp"
#include "Plato_DakotaDataMap.hpp"
#include "lightmp.hpp"
#include "Plato_Macros.hpp"
#include "Plato_Parser.hpp"
#include "Plato_DriverInterface.hpp"
#include "Plato_DakotaAppInterface.hpp"
#include "Plato_OptimizerUtilities.hpp"

namespace TestPlatoDakotaInterface
{

TEST(PlatoTest, tokenize)
{
    // test 1
    auto tString = std::string("objective_value_1");
    auto tTokens = Plato::tokenize(tString);
    std::vector<std::string> tGold = { "objective", "value", "1" };
    for(auto& Token : tTokens)
    {
        auto tIndex = &Token - &tTokens[0];
        EXPECT_STREQ(tGold[tIndex].c_str(), Token.c_str());
    }

    // test 2
    tString = std::string("inequality_constraint_gradient_1");
    tTokens = Plato::tokenize(tString);
    tGold = { "inequality", "constraint", "gradient", "1" };
    for(auto& Token : tTokens)
    {
        auto tIndex = &Token - &tTokens[0];
        EXPECT_STREQ(tGold[tIndex].c_str(), Token.c_str());
    }

    // test 3
    tString = std::string("equality constraint gradient 1");
    tTokens = Plato::tokenize(tString, ' ');
    tGold = { "equality", "constraint", "gradient", "1" };
    for(auto& Token : tTokens)
    {
        auto tIndex = &Token - &tTokens[0];
        EXPECT_STREQ(tGold[tIndex].c_str(), Token.c_str());
    }
}

TEST(PlatoTest, DakotaDataMap_parseInputs)
{
    // create criterion value stage
    Plato::InputData tCriterionValueStage("Stage");
    tCriterionValueStage.add<std::string>("StageTag", "criterion_value_0");
    tCriterionValueStage.add<std::string>("StageName", "CriteriaValueEvaluation");

    // add inputs for criterion value stage
    Plato::InputData tInput1("Input");
    tInput1.add<std::string>("Tag", "continuous");
    tInput1.add<std::string>("SharedDataName", "design_parameters_0");
    tCriterionValueStage.add<Plato::InputData>("Input", tInput1);
    Plato::InputData tInput2("Input");
    tInput2.add<std::string>("Tag", "continuous");
    tInput2.add<std::string>("SharedDataName", "design_parameters_1");
    tCriterionValueStage.add<Plato::InputData>("Input", tInput2);

    // add outputs for criterion value stage
    Plato::InputData tOutput1("Output");
    tOutput1.add<std::string>("SharedDataName", "compliance_value_0");
    tCriterionValueStage.add<Plato::InputData>("Output", tOutput1);
    Plato::InputData tOutput2("Output");
    tOutput2.add<std::string>("SharedDataName", "compliance_value_1");
    tCriterionValueStage.add<Plato::InputData>("Output", tOutput2);

    // append criterion value stage
    Plato::InputData tDriver("DakotaDriver");
    tDriver.add<Plato::InputData>("Stage", tCriterionValueStage);

    // create criterion gradient stage
    Plato::InputData tCriterionGradientStage("Stage");
    tCriterionGradientStage.add<std::string>("StageTag", "criterion_gradient_0");
    tCriterionGradientStage.add<std::string>("StageName", "CriteriaGradientEvaluation");

    // add inputs for criterion gradient stage
    tCriterionGradientStage.add<Plato::InputData>("Input", tInput1);
    tCriterionGradientStage.add<Plato::InputData>("Input", tInput2);

    // add outputs for criterion gradient stage
    Plato::InputData tOutputG1("Output");
    tOutputG1.add<std::string>("SharedDataName", "compliance_gradient_0");
    tCriterionGradientStage.add<Plato::InputData>("Output", tOutputG1);
    Plato::InputData tOutputG2("Output");
    tOutputG2.add<std::string>("SharedDataName", "compliance_gradient_1");
    tCriterionGradientStage.add<Plato::InputData>("Output", tOutputG2);

    // append criterion gradient stage
    tDriver.add<Plato::InputData>("Stage", tCriterionGradientStage);

    // append dakota driver stage
    Plato::InputData Inputs("Inputs");
    Inputs.add<Plato::InputData>("DakotaDriver", tDriver);

    // construct dakota data map
    Plato::DakotaDataMap tDakotaDataMap(Inputs);

    // test parsing of stage tags
    auto tStageTags = tDakotaDataMap.getStageTags();
    EXPECT_EQ(2u, tStageTags.size());
    std::vector<std::string> tGoldStageTags = {"criterion_value_0", "criterion_gradient_0"};
    for(auto& tStageTag : tStageTags)
    {
        auto tItr = std::find(tGoldStageTags.begin(), tGoldStageTags.end(), tStageTag);
        if(tItr == tGoldStageTags.end())
        {
            ASSERT_STREQ(tStageTag.c_str(), "FAIL - DID NOT FIND VALUE IN GOLD SET");
        }
        EXPECT_STREQ(tItr.operator*().c_str(), tStageTag.c_str());
    }

    // test parsing of stage names
    auto tStageName = tDakotaDataMap.getStageName("criterion_value_0");
    EXPECT_STREQ("CriteriaValueEvaluation", tStageName.c_str());

    tStageName = tDakotaDataMap.getStageName("criterion_gradient_0");
    EXPECT_STREQ("CriteriaGradientEvaluation", tStageName.c_str());

    // test parsing of inputs
    bool tHasInputs = tDakotaDataMap.stageHasInputSharedData("criterion_value_0");
    EXPECT_EQ(tHasInputs, true);
    auto tInputSharedDataNames = tDakotaDataMap.getInputSharedDataNames("criterion_value_0", "continuous"); 
    std::vector<std::string> tGoldInputNames = {"design_parameters_0", "design_parameters_1"};
    for(auto& tName : tInputSharedDataNames)
    {
        auto tIndex = &tName - &tInputSharedDataNames[0];
        EXPECT_STREQ(tGoldInputNames[tIndex].c_str(), tName.c_str());
    }

    tHasInputs = tDakotaDataMap.stageHasInputSharedData("criterion_gradient_0");
    EXPECT_EQ(tHasInputs, true);
    tInputSharedDataNames = tDakotaDataMap.getInputSharedDataNames("criterion_gradient_0", "continuous"); 
    tGoldInputNames = {"design_parameters_0", "design_parameters_1"};
    for(auto& tName : tInputSharedDataNames)
    {
        auto tIndex = &tName - &tInputSharedDataNames[0];
        EXPECT_STREQ(tGoldInputNames[tIndex].c_str(), tName.c_str());
    }

    // test parsing of outputs
    bool tHasOutputs = tDakotaDataMap.stageHasOutputSharedData("criterion_value_0");
    EXPECT_EQ(tHasOutputs, true);
    auto tOutputSharedDataNames = tDakotaDataMap.getOutputSharedDataNames("criterion_value_0");
    std::vector<std::string> tGoldOutputNames = {"compliance_value_0", "compliance_value_1"};
    for(auto& tName : tOutputSharedDataNames)
    {
        auto tIndex = &tName - &tOutputSharedDataNames[0];
        EXPECT_STREQ(tGoldOutputNames[tIndex].c_str(), tName.c_str());
    }

    tHasOutputs = tDakotaDataMap.stageHasOutputSharedData("criterion_gradient_0");
    EXPECT_EQ(tHasOutputs, true);
    tOutputSharedDataNames = tDakotaDataMap.getOutputSharedDataNames("criterion_gradient_0");
    tGoldOutputNames = {"compliance_gradient_0", "compliance_gradient_1"};
    for(auto& tName : tOutputSharedDataNames)
    {
        auto tIndex = &tName - &tOutputSharedDataNames[0];
        EXPECT_STREQ(tGoldOutputNames[tIndex].c_str(), tName.c_str());
    }
}

TEST(PlatoTest, DakotaDataMap_accessContinuousVars)
{
    // create initialize stage
    Plato::InputData tInitializeStage("Stage");
    tInitializeStage.add<std::string>("StageTag", "initialize");
    tInitializeStage.add<std::string>("StageName", "GenerateMeshes");
    
    // add inputs for initialize stage
    Plato::InputData tInput1("Input");
    tInput1.add<std::string>("Tag", "continuous");
    tInput1.add<std::string>("SharedDataName", "design_parameters_0");
    tInitializeStage.add<Plato::InputData>("Input", tInput1);

    // append initialize stage
    Plato::InputData tDriver("DakotaDriver");
    tDriver.add<Plato::InputData>("Stage", tInitializeStage);

    // create criterion value stage
    Plato::InputData tCriterionValueStage("Stage");
    tCriterionValueStage.add<std::string>("StageTag", "criterion_value_0");
    tCriterionValueStage.add<std::string>("StageName", "CriteriaValueEvaluation");

    // add outputs for criterion value stage
    Plato::InputData tOutput1("Output");
    tOutput1.add<std::string>("SharedDataName", "compliance_value_0");
    tCriterionValueStage.add<Plato::InputData>("Output", tOutput1);

    // append criterion value stage
    tDriver.add<Plato::InputData>("Stage", tCriterionValueStage);

    // append dakota driver stage
    Plato::InputData Inputs("Inputs");
    Inputs.add<Plato::InputData>("DakotaDriver", tDriver);

    // construct dakota data map
    Plato::DakotaDataMap tDakotaDataMap(Inputs);

    // create vector of inputs
    Teuchos::SerialDenseVector<int, double> tVariableData(4);
    tVariableData[0] = 1.2;
    tVariableData[1] = 0.8;
    tVariableData[2] = 47.3;
    tVariableData[3] = -5.1;
    
    // check for shared data
    bool tHasInputs = tDakotaDataMap.stageHasInputSharedData("criterion_value_0");
    EXPECT_EQ(tHasInputs, false);
    tHasInputs = tDakotaDataMap.stageHasInputSharedData("initialize");
    EXPECT_EQ(tHasInputs, true);

    // set continuous variables 
    auto tInputSharedDataNames = tDakotaDataMap.getInputSharedDataNames("initialize", "continuous"); 
    tDakotaDataMap.setContinuousVarsSharedData(tInputSharedDataNames[0],4,tVariableData);

    // test parsing of continuous variables
    auto tStoredVariables = tDakotaDataMap.getContinuousVarsSharedData(tInputSharedDataNames[0]);
    std::vector<double> tGoldVals = {1.2, 0.8, 47.3, -5.1};

    for(auto& tVal : tStoredVariables)
    {
        auto tIndex = &tVal - &tStoredVariables[0];
        EXPECT_EQ(tGoldVals[tIndex], tVal);
    }
}

TEST(PlatoTest, DakotaDataMap_accessDiscreteRealVars)
{
    // create criterion value stage
    Plato::InputData tCriterionValueStage("Stage");
    tCriterionValueStage.add<std::string>("StageTag", "criterion_value_0");
    tCriterionValueStage.add<std::string>("StageName", "CriteriaValueEvaluation");

    // add inputs for criterion value stage
    Plato::InputData tInput1("Input");
    tInput1.add<std::string>("Tag", "discrete real");
    tInput1.add<std::string>("SharedDataName", "design_parameters_0");
    tCriterionValueStage.add<Plato::InputData>("Input", tInput1);

    // add outputs for criterion value stage
    Plato::InputData tOutput1("Output");
    tOutput1.add<std::string>("SharedDataName", "compliance_value_0");
    tCriterionValueStage.add<Plato::InputData>("Output", tOutput1);

    // append criterion value stage
    Plato::InputData tDriver("DakotaDriver");
    tDriver.add<Plato::InputData>("Stage", tCriterionValueStage);

    // append dakota driver stage
    Plato::InputData Inputs("Inputs");
    Inputs.add<Plato::InputData>("DakotaDriver", tDriver);

    // construct dakota data map
    Plato::DakotaDataMap tDakotaDataMap(Inputs);

    // create vector of inputs
    Teuchos::SerialDenseVector<int, double> tVariableData(4);
    tVariableData[0] = 1.2;
    tVariableData[1] = 2.4;
    tVariableData[2] = 9.6;
    tVariableData[3] = 1.2;

    // set continuous variables
    bool tHasInputs = tDakotaDataMap.stageHasInputSharedData("criterion_value_0");
    EXPECT_EQ(tHasInputs, true);
    auto tInputSharedDataNames = tDakotaDataMap.getInputSharedDataNames("criterion_value_0", "discrete real"); 
    tDakotaDataMap.setDiscreteRealVarsSharedData(tInputSharedDataNames[0],4,tVariableData);

    // test parsing of continuous variables
    auto tStoredVariables = tDakotaDataMap.getDiscreteRealVarsSharedData(tInputSharedDataNames[0]);
    std::vector<double> tGoldVals = {1.2, 2.4, 9.6, 1.2};

    for(auto& tVal : tStoredVariables)
    {
        auto tIndex = &tVal - &tStoredVariables[0];
        EXPECT_EQ(tGoldVals[tIndex], tVal);
    }
}

TEST(PlatoTest, DakotaDataMap_accessDiscreteIntegerVars)
{
    // create criterion value stage
    Plato::InputData tCriterionValueStage("Stage");
    tCriterionValueStage.add<std::string>("StageTag", "criterion_value_0");
    tCriterionValueStage.add<std::string>("StageName", "CriteriaValueEvaluation");

    // add inputs for criterion value stage
    Plato::InputData tInput1("Input");
    tInput1.add<std::string>("Tag", "discrete integer");
    tInput1.add<std::string>("SharedDataName", "design_parameters_0");
    tCriterionValueStage.add<Plato::InputData>("Input", tInput1);

    // add outputs for criterion value stage
    Plato::InputData tOutput1("Output");
    tOutput1.add<std::string>("SharedDataName", "compliance_value_0");
    tCriterionValueStage.add<Plato::InputData>("Output", tOutput1);

    // append criterion value stage
    Plato::InputData tDriver("DakotaDriver");
    tDriver.add<Plato::InputData>("Stage", tCriterionValueStage);

    // append dakota driver stage
    Plato::InputData Inputs("Inputs");
    Inputs.add<Plato::InputData>("DakotaDriver", tDriver);

    // construct dakota data map
    Plato::DakotaDataMap tDakotaDataMap(Inputs);

    // create vector of inputs
    Teuchos::SerialDenseVector<int, int> tVariableData(4);
    tVariableData[0] = 1;
    tVariableData[1] = 2;
    tVariableData[2] = 9;
    tVariableData[3] = 3;

    // set continuous variables
    bool tHasInputs = tDakotaDataMap.stageHasInputSharedData("criterion_value_0");
    EXPECT_EQ(tHasInputs, true);
    auto tInputSharedDataNames = tDakotaDataMap.getInputSharedDataNames("criterion_value_0", "discrete integer"); 
    tDakotaDataMap.setDiscreteIntegerVarsSharedData(tInputSharedDataNames[0],4,tVariableData);

    // test parsing of continuous variables
    auto tStoredVariables = tDakotaDataMap.getDiscreteIntegerVarsSharedData(tInputSharedDataNames[0]);
    std::vector<double> tGoldVals = {1, 2, 9, 3};

    for(auto& tVal : tStoredVariables)
    {
        auto tIndex = &tVal - &tStoredVariables[0];
        EXPECT_EQ(tGoldVals[tIndex], tVal);
    }
}

TEST(PlatoTest, DakotaDataMap_accessOutputData)
{
    // create criterion gradient stage
    Plato::InputData tCriterionGradientStage("Stage");
    tCriterionGradientStage.add<std::string>("StageTag", "criterion_gradient_0");
    tCriterionGradientStage.add<std::string>("StageName", "CriteriaGradientEvaluation");

    // add inputs for criterion gradient stage
    Plato::InputData tInput1("Input");
    tInput1.add<std::string>("Tag", "discrete integer");
    tInput1.add<std::string>("SharedDataName", "design_parameters_0");
    tCriterionGradientStage.add<Plato::InputData>("Input", tInput1);

    // append criterion gradient stage
    Plato::InputData tDriver("DakotaDriver");
    tDriver.add<Plato::InputData>("Stage", tCriterionGradientStage);
    
    // create finalize stage
    Plato::InputData tFinalizeStage("Stage");
    tFinalizeStage.add<std::string>("StageTag", "finalize");
    tFinalizeStage.add<std::string>("StageName", "OutputGradients");
    
    // add outputs for finalize stage
    Plato::InputData tOutput1("Output");
    tOutput1.add<std::string>("SharedDataName", "compliance_gradient_0");
    tFinalizeStage.add<Plato::InputData>("Output", tOutput1);

    // append finalize stage
    tDriver.add<Plato::InputData>("Stage", tFinalizeStage);

    // append dakota driver stage
    Plato::InputData Inputs("Inputs");
    Inputs.add<Plato::InputData>("DakotaDriver", tDriver);

    // construct dakota data map
    Plato::DakotaDataMap tDakotaDataMap(Inputs);

    // check for shared data
    bool tHasOutputs = tDakotaDataMap.stageHasOutputSharedData("criterion_gradient_0");
    EXPECT_EQ(tHasOutputs, false);
    tHasOutputs = tDakotaDataMap.stageHasOutputSharedData("finalize");
    EXPECT_EQ(tHasOutputs, true);

    // create output metadata
    auto tOutputSharedDataNames = tDakotaDataMap.getOutputSharedDataNames("finalize");
    Plato::dakota::SharedDataMetaData tMetaData;
    tMetaData.mName = tOutputSharedDataNames[0];
    tMetaData.mValues.resize(1u,0.0);

    // set continuous variables
    auto tStageTags = tDakotaDataMap.getStageTags();
    tDakotaDataMap.setOutputVarsSharedData(tStageTags[0],tMetaData,0);

    // test parsing of output
    auto tOutMetadata = tDakotaDataMap.getOutputVarsSharedData(tStageTags[0],0);
    EXPECT_STREQ("compliance_gradient_0", tOutMetadata.mName.c_str());
    EXPECT_EQ(1u, tOutMetadata.mValues.size());
    EXPECT_EQ(0.0, tOutMetadata.mValues[0]);
}

TEST(PlatoTest, DakotaDataMap_noSharedDataNames)
{
    // create criterion value stage
    Plato::InputData tCriterionValueStage("Stage");
    tCriterionValueStage.add<std::string>("StageTag", "criterion_value_0");
    tCriterionValueStage.add<std::string>("StageName", "CriteriaValueEvaluation");

    // add inputs for criterion value stage
    Plato::InputData tInput1("Input");
    tInput1.add<std::string>("Tag", "discrete integer");
    tInput1.add<std::string>("SharedDataName", "design_parameters_0");
    tCriterionValueStage.add<Plato::InputData>("Input", tInput1);

    // add outputs for criterion value stage
    Plato::InputData tOutput1("Output");
    tOutput1.add<std::string>("SharedDataName", "compliance_value_0");
    tCriterionValueStage.add<Plato::InputData>("Output", tOutput1);

    // append criterion value stage
    Plato::InputData tDriver("DakotaDriver");
    tDriver.add<Plato::InputData>("Stage", tCriterionValueStage);

    // append dakota driver stage
    Plato::InputData Inputs("Inputs");
    Inputs.add<Plato::InputData>("DakotaDriver", tDriver);

    // construct dakota data map
    Plato::DakotaDataMap tDakotaDataMap(Inputs);

    // check cases with no stage tag found
    auto tInputSharedDataNames = tDakotaDataMap.getInputSharedDataNames("objective_value", "discrete integer"); 
    EXPECT_EQ(0u, tInputSharedDataNames.size());

    auto tOutputSharedDataNames = tDakotaDataMap.getOutputSharedDataNames("finalize");
    EXPECT_EQ(0u, tOutputSharedDataNames.size());

    // check cases with no input data tag found
    tInputSharedDataNames = tDakotaDataMap.getInputSharedDataNames("criterion_value_0", "continuous"); 
    EXPECT_EQ(0u, tInputSharedDataNames.size());
    tInputSharedDataNames = tDakotaDataMap.getInputSharedDataNames("criterion_value_0", "discrete real"); 
    EXPECT_EQ(0u, tInputSharedDataNames.size());
}

}
// TestPlatoDakotaInterface
