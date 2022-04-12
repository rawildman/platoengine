/*
 * XMLGeneratorOperation.cpp
 *
 *  Created on: April 12, 2022
 */

#include "XMLGeneratorOperation.hpp"
#include "XMLGeneratorUtilities.hpp"
#include "XMLGeneratorPlatoMainOperationFileUtilities.hpp"

namespace XMLGen
{
    
XMLGeneratorOperation::XMLGeneratorOperation()
{
}
void XMLGeneratorOperation::write
(pugi::xml_document& aDocument)
{
    


}
void XMLGeneratorOperation::appendCommonChildren
(pugi::xml_node &aOperationNode)
{
    addChild(aOperationNode, "Function", mFunction);
    addChild(aOperationNode, "Name", mName);
    addChild(aOperationNode, "Command", mCommand);
}

XMLGeneratorOperationWait::XMLGeneratorOperationWait
(std::string aName,
    std::string aFile,
    std::string aEvaluation)
{
    mEvaluation=aEvaluation;
    mName = aName;
    mFunction = "SystemCall";

    mCommand = std::string("while lsof -u $USER | grep ./") + aFile + "; do sleep 1; done";
    if( mEvaluation == "" )
    {
        mChDir = false;
    }
    else
    {
        mName += "_"+mEvaluation;
        mChDir = true;
    }
    mOnChange = false;
}
void XMLGeneratorOperationWait::write
(pugi::xml_document& aDocument)
{
    pugi::xml_node tOperationNode = aDocument.append_child("Operation");
    appendCommonChildren(tOperationNode);
    if(mChDir)
        addChild(tOperationNode, "ChDir", std::string("evaluations_") + mEvaluation);
    addChild(tOperationNode, "OnChange", (mOnChange?"true":"false"));
    
    //auto tInputNode = operationNode.append_child("Input");
    //XMLGen::append_children({"ArgumentName"}, {"Parameters"}, tInputNode);
}

XMLGeneratorOperationGemmaMPISystemCall::XMLGeneratorOperationGemmaMPISystemCall(std::string aInputDeck, std::string aNumRanks, std::string aEvaluation)
{
    mFunction = "SystemCallMPI";
    mEvaluation=aEvaluation;
    mName = "gemma";

    mCommand = "gemma";
    if( mEvaluation == "" )
    {
        mChDir = false;
    }
    else
    {
        mName += "_"+mEvaluation;
        mChDir = true;
    }
    mOnChange = true;
    mNumRanks = aNumRanks;
    mArgument = aInputDeck;

}
 void XMLGeneratorOperationGemmaMPISystemCall::write(pugi::xml_document& aDocument)
 {
    pugi::xml_node tOperationNode = aDocument.append_child("Operation");
    appendCommonChildren(tOperationNode);
    if(mChDir)
        addChild(tOperationNode, "ChDir", std::string("evaluations_") + mEvaluation);
    addChild(tOperationNode, "OnChange", (mOnChange?"true":"false"));
    addChild(tOperationNode, "NumRanks", mNumRanks );
    addChild(tOperationNode, "Argument", mArgument );
    addChild(tOperationNode, "AppendInput", "false" );
 }


XMLGeneratorOperationAprepro::XMLGeneratorOperationAprepro(std::string aInputDeck, std::vector<std::string> aOptions , std::string aEvaluation)
{
    mFunction = "SystemCall";
    mEvaluation=aEvaluation;
    mName = "aprepro";

    mCommand = "aprepro";

    
    mOnChange = true;
    
    mArgument.push_back("-q");
    mArgument.push_back(aInputDeck+".template");
    mArgument.push_back(aInputDeck);
    mOptions = aOptions;

    mInput.mLayout = "scalar";
    mInput.mSize = mOptions.size();
    mInput.mArgumentName = "parameters";
    if( mEvaluation == "" )
    {
        mChDir = false;
    }
    else
    {
        mName += "_" + mEvaluation;
        mChDir = true;
        mInput.mArgumentName += "_" + mEvaluation;
    }


}
void XMLGeneratorOperationAprepro::write(pugi::xml_document& aDocument)
{
    pugi::xml_node tOperationNode = aDocument.append_child("Operation");
    appendCommonChildren(tOperationNode);
    if(mChDir)
        addChild(tOperationNode, "ChDir", std::string("evaluations_") + mEvaluation);
    addChild(tOperationNode, "OnChange", (mOnChange?"true":"false"));
    
    for( auto tA : mArgument )
        addChild(tOperationNode, "Argument", tA );
    
    addChild(tOperationNode, "AppendInput", "true" );
    
    for( auto tO : mOptions )
        addChild(tOperationNode, "Option", tO );
    
    auto tInputNode = tOperationNode.append_child("Input");
    addChild(tInputNode,"ArgumentName",mInput.mArgumentName);
    addChild(tInputNode,"Layout",mInput.mLayout);
    addChild(tInputNode,"Size",mInput.mSize);
    
}

/*

XMLGeneratorOperationSystemCallMPI::XMLGeneratorOperationSystemCallMPI
(const XMLGen::InputData& aInputMetaData, 
int aCriteria, 
int aEvaluation):XMLGeneratorOperation(aInputMetaData)
{


}
void XMLGeneratorOperationSystemCallMPI::write
(pugi::xml_document& aDocument)
{


}

 XMLGeneratorOperationHarvestDataFunction::XMLGeneratorOperationHarvestDataFunction
 (const XMLGen::InputData& aInputMetaData, 
 int aCriteria, 
 int aEvaluation):XMLGeneratorOperation(aInputMetaData)
 {


 }
void XMLGeneratorOperationHarvestDataFunction::write
(pugi::xml_document& aDocument)
{


}

XMLGeneratorOperationAprepro::XMLGeneratorOperationAprepro
(const XMLGen::InputData& aInputMetaData, 
int aCriteria,
int aEvaluation):XMLGeneratorOperation(aInputMetaData)
{


}

void XMLGeneratorOperationAprepro::write
(pugi::xml_document& aDocument)
{


}
*/


}