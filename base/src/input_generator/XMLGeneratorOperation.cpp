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
    
XMLGeneratorOperation::XMLGeneratorOperation
(const std::string& aName,
 const std::string& aFunction) :
 mName(aName),
 mFunction(aFunction)
{
}

void XMLGeneratorOperation::appendCommonChildren
(pugi::xml_node& aOperationNode)
{
    addChild(aOperationNode, "Function", mFunction);
    addChild(aOperationNode, "Name", mName);
}

XMLGeneratorOperationWait::XMLGeneratorOperationWait
(const std::string& aName,
 const std::string& aFile,
 const std::string& aEvaluation) :
 XMLGeneratorOperation(aName, "SystemCall"),
 mEvaluation(aEvaluation)
{
    mCommand = std::string("while lsof -u $USER | grep ./") + aFile + "; do sleep 1; done";
    if( mEvaluation == "" )
    {
        mChDir = false;
    }
    else
    {
        mName += "_" + mEvaluation;
        mChDir = true;
    }
    mOnChange = false;
}

void XMLGeneratorOperationWait::write(pugi::xml_document& aDocument)
{
    auto tOperationNode = aDocument.append_child("Operation");
    appendCommonChildren(tOperationNode);
    addChild(tOperationNode, "Command", mCommand);
    if(mChDir)
        addChild(tOperationNode, "ChDir", std::string("evaluations_") + mEvaluation);
    addChild(tOperationNode, "OnChange", (mOnChange ? "true" : "false"));
}

XMLGeneratorOperationGemmaMPISystemCall::XMLGeneratorOperationGemmaMPISystemCall
(const std::string& aInputDeck, 
 const std::string& aNumRanks, 
 const std::string& aEvaluation) :
 XMLGeneratorOperation("gemma", "SystemCallMPI"),
 mCommand("gemma"),
 mArgument(aInputDeck),
 mNumRanks(aNumRanks),
 mEvaluation(aEvaluation)
{
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
}

 void XMLGeneratorOperationGemmaMPISystemCall::write(pugi::xml_document& aDocument)
 {
    auto tOperationNode = aDocument.append_child("Operation");
    appendCommonChildren(tOperationNode);
    addChild(tOperationNode, "Command", mCommand);

    if(mChDir)
        addChild(tOperationNode, "ChDir", std::string("evaluations_") + mEvaluation);
    addChild(tOperationNode, "OnChange", (mOnChange ? "true" : "false"));
    addChild(tOperationNode, "NumRanks", mNumRanks );
    addChild(tOperationNode, "Argument", mArgument );
    addChild(tOperationNode, "AppendInput", "false" );
 }

XMLGeneratorOperationAprepro::XMLGeneratorOperationAprepro
(const std::string& aInputDeck, 
 const std::vector<std::string>& aOptions, 
 const std::string& aEvaluation) :
 XMLGeneratorOperation("aprepro", "SystemCall"),
 mCommand("aprepro"),
 mEvaluation(aEvaluation)
{
    mArgument.push_back("-q");
    mArgument.push_back(aInputDeck + ".template");
    mArgument.push_back(aInputDeck);
    mOptions = aOptions;

    mInput.mLayout = "scalar";
    mInput.mSize = std::to_string(mOptions.size());
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
    mOnChange = true;
}

void XMLGeneratorOperationAprepro::write(pugi::xml_document& aDocument)
{
    auto tOperationNode = aDocument.append_child("Operation");
    appendCommonChildren(tOperationNode);
    addChild(tOperationNode, "Command", mCommand);

    if(mChDir)
        addChild(tOperationNode, "ChDir", std::string("evaluations_") + mEvaluation);
    addChild(tOperationNode, "OnChange", (mOnChange ? "true" : "false"));
    
    for( auto tA : mArgument )
        addChild(tOperationNode, "Argument", tA );
    
    addChild(tOperationNode, "AppendInput", "true" );

    for( auto tO : mOptions )
        addChild(tOperationNode, "Option", tO );
    
    auto tInputNode = tOperationNode.append_child("Input");
    addChild(tInputNode, "ArgumentName", mInput.mArgumentName);
    addChild(tInputNode, "Layout", mInput.mLayout);
    addChild(tInputNode, "Size", mInput.mSize);
}

XMLGeneratorOperationHarvestDataFunction::XMLGeneratorOperationHarvestDataFunction
(const std::string& aFile,
 const std::string& aMathOperation,
 const std::string& aDataColumn,
 const std::string& aEvaluation) : 
 XMLGeneratorOperation("harvest_data", "HarvestDataFromFile"),
 mFile(aFile),
 mOperation(aMathOperation),
 mColumn(aDataColumn),
 mEvaluation(aEvaluation)
{
    mOutput.mLayout = "scalar";
    mOutput.mSize = "1";
    mOutput.mArgumentName = "criterion value";
    if( mEvaluation == "" )
    {
        mChDir = false;
    }
    else
    {
        mName += "_" + mEvaluation;
        mChDir = true;
    }
    mOnChange = true;
}

void XMLGeneratorOperationHarvestDataFunction::write(pugi::xml_document& aDocument)
{
    auto tOperationNode = aDocument.append_child("Operation");
    appendCommonChildren(tOperationNode);

    if(mChDir)
        addChild(tOperationNode, "ChDir", std::string("evaluations_") + mEvaluation);

    addChild(tOperationNode, "File", mFile);
    addChild(tOperationNode, "Operation", mOperation);
    addChild(tOperationNode, "Column", mColumn);

    auto tOutputNode = tOperationNode.append_child("Output");
    addChild(tOutputNode, "ArgumentName", mOutput.mArgumentName);
    addChild(tOutputNode, "Layout", mOutput.mLayout);
    addChild(tOutputNode, "Size", mOutput.mSize);
}

}