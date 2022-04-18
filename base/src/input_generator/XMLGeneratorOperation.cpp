/*
 * XMLGeneratorOperation.cpp
 *
 *  Created on: April 12, 2022
 */

#include "XMLGeneratorOperation.hpp"
#include "XMLGeneratorUtilities.hpp"
#include "XMLGeneratorPlatoMainOperationFileUtilities.hpp"
#include <regex>

namespace XMLGen
{
    
XMLGeneratorOperation::XMLGeneratorOperation
(const std::string& aName,
 const std::string& aFunction,
 std::shared_ptr<XMLGeneratorPerformer> aPerformer,
 int aConcurrentEvaluations) :
 mName(aName),
 mFunction(aFunction),
 mPerformer(aPerformer),
 mConcurrentEvaluations(aConcurrentEvaluations)
{
    mEvaluationTag = "{E}";
    mTagExpression = "\\{E\\}";
}

void XMLGeneratorOperation::appendCommonChildren
(pugi::xml_node& aOperationNode,
 std::string aEvaluationNumber)
{
    addChild(aOperationNode, "Function", mFunction);
    addChild(aOperationNode, "Name", name(aEvaluationNumber));
}
void XMLGeneratorOperation::addNameTag()
{
    mName += "_" + mEvaluationTag;
}

/*void XMLGeneratorOperation::for_write_definition(pugi::xml_document& aDocument, std::string aLoopMaxVar)
{
    auto tOperationNode = aDocument.append_child(std::string("For var=\"") + mEvaluationTag + "\" in=\"" + aLoopMaxVar+ "\"");
    write_definition(aDocument);
}*/

std::string XMLGeneratorOperation::name(std::string aEvaluationNumber)
{
    if(aEvaluationNumber=="")
        return mName;
    else
    {
        return std::regex_replace (mName,mTagExpression,aEvaluationNumber);
    }    
}

XMLGeneratorOperationWait::XMLGeneratorOperationWait
(const std::string& aName,
 const std::string& aFile,
 std::shared_ptr<XMLGeneratorPerformer> aPerformer,
 int aConcurrentEvaluations) :
 XMLGeneratorOperation(aName, "SystemCall",aPerformer, aConcurrentEvaluations)
{
    mCommand = std::string("while lsof -u $USER | grep ./") + aFile + "; do sleep 1; done";
    
    if( mConcurrentEvaluations == 0 )
    {
        mChDir = false;
    }
    else
    {
        addNameTag();
        mChDir = true;
    }
    mOnChange = false;
    
}

void XMLGeneratorOperationWait::write_definition(pugi::xml_document& aDocument, std::string aEvaluationNumber)
{
    auto tOperationNode = aDocument.append_child("Operation");
    appendCommonChildren(tOperationNode,aEvaluationNumber);

    std::string tEvaluationTag;
    if(aEvaluationNumber == "")
        tEvaluationTag = mEvaluationTag;
    else
        tEvaluationTag = aEvaluationNumber;

    addChild(tOperationNode, "Command", mCommand);
    if(mChDir)
        addChild(tOperationNode, "ChDir", std::string("evaluations_") + tEvaluationTag);
    addChild(tOperationNode, "OnChange", (mOnChange ? "true" : "false"));
}

void XMLGeneratorOperationWait::write_interface(pugi::xml_document& aDocument, std::string aEvaluationNumber)
{
    auto tOperationNode = aDocument.append_child("Operation");
    addChild(tOperationNode, "Name", name(aEvaluationNumber));
    addChild(tOperationNode, "PerformerName", mPerformer->name(aEvaluationNumber));
}

XMLGeneratorOperationGemmaMPISystemCall::XMLGeneratorOperationGemmaMPISystemCall
(const std::string& aInputDeck, 
 const std::string& aNumRanks, 
 std::shared_ptr<XMLGeneratorPerformer> aPerformer,
 int aConcurrentEvaluations) :
 XMLGeneratorOperation("gemma", "SystemCallMPI",aPerformer, aConcurrentEvaluations),
 mCommand("gemma"),
 mArgument(aInputDeck),
 mNumRanks(aNumRanks)
{
    
    if( mConcurrentEvaluations == 0 )
    {
        mChDir = false;
    }
    else
    {
        addNameTag();
        mChDir = true;
    }
    mOnChange = true;
}

 void XMLGeneratorOperationGemmaMPISystemCall::write_definition(pugi::xml_document& aDocument, std::string aEvaluationNumber)
 {
    auto tOperationNode = aDocument.append_child("Operation");
    appendCommonChildren(tOperationNode,aEvaluationNumber);
    addChild(tOperationNode, "Command", mCommand);

    std::string tEvaluationTag;
    if(aEvaluationNumber == "")
        tEvaluationTag = mEvaluationTag;
    else
        tEvaluationTag = aEvaluationNumber;

    if(mChDir)
        addChild(tOperationNode, "ChDir", std::string("evaluations_") + tEvaluationTag);
    addChild(tOperationNode, "OnChange", (mOnChange ? "true" : "false"));
    addChild(tOperationNode, "NumRanks", mNumRanks );
    addChild(tOperationNode, "Argument", mArgument );
    addChild(tOperationNode, "AppendInput", "false" );
 }

 void XMLGeneratorOperationGemmaMPISystemCall::write_interface(pugi::xml_document& aDocument, std::string aEvaluationNumber)
{
    auto tOperationNode = aDocument.append_child("Operation");
    addChild(tOperationNode, "Name", name(aEvaluationNumber));
    addChild(tOperationNode, "PerformerName",  mPerformer->name(aEvaluationNumber));
}

XMLGeneratorOperationAprepro::XMLGeneratorOperationAprepro
(const std::string& aInputDeck, 
 const std::vector<std::string>& aOptions, 
 std::shared_ptr<XMLGeneratorSharedData> aSharedData,
 std::shared_ptr<XMLGeneratorPerformer> aPerformer, 
 int aConcurrentEvaluations) :
 XMLGeneratorOperation("aprepro", "SystemCall", aPerformer, aConcurrentEvaluations),
 mCommand("aprepro")
{
    mArgument.push_back("-q");
    mArgument.push_back(aInputDeck + ".template");
    mArgument.push_back(aInputDeck);
    mOptions = aOptions;

    mInput.mLayout = "scalar";
    mInput.mSize = std::to_string(mOptions.size());
    
    mInput.mSharedData = aSharedData;

    mInput.mArgumentName = aSharedData->name("");

    if( mConcurrentEvaluations == 0 )
    {
        mChDir = false;
    }
    else
    {
        addNameTag();
        mChDir = true;
       
    }
    mOnChange = true;
}

void XMLGeneratorOperationAprepro::write_definition(pugi::xml_document& aDocument, std::string aEvaluationNumber)
{
    auto tOperationNode = aDocument.append_child("Operation");
    appendCommonChildren(tOperationNode,aEvaluationNumber);
    addChild(tOperationNode, "Command", mCommand);

    std::string tEvaluationTag;
    if(aEvaluationNumber == "")
        tEvaluationTag = mEvaluationTag;
    else
        tEvaluationTag = aEvaluationNumber;

    if(mChDir)
        addChild(tOperationNode, "ChDir", std::string("evaluations_") + tEvaluationTag);
    addChild(tOperationNode, "OnChange", (mOnChange ? "true" : "false"));
    
    for( auto tA : mArgument )
        addChild(tOperationNode, "Argument", tA );
    
    addChild(tOperationNode, "AppendInput", "true" );

    for( auto tO : mOptions )
        addChild(tOperationNode, "Option", tO );
    
    auto tInputNode = tOperationNode.append_child("Input");
    addChild(tInputNode, "ArgumentName",std::regex_replace (mInput.mArgumentName,mTagExpression,tEvaluationTag) );
    addChild(tInputNode, "Layout", mInput.mLayout);
    addChild(tInputNode, "Size", mInput.mSize);
}

void XMLGeneratorOperationAprepro::write_interface(pugi::xml_document& aDocument, std::string aEvaluationNumber)
{
    auto tOperationNode = aDocument.append_child("Operation");
    addChild(tOperationNode, "Name", name(aEvaluationNumber));
    addChild(tOperationNode, "PerformerName",  mPerformer->name(aEvaluationNumber));
    auto tInputNode = tOperationNode.append_child("Input");
    addChild(tInputNode, "ArgumentName",std::regex_replace (mInput.mArgumentName,mTagExpression,aEvaluationNumber) );
    addChild(tInputNode, "SharedDataName", mInput.mSharedData->name(aEvaluationNumber));
}

XMLGeneratorOperationHarvestDataFunction::XMLGeneratorOperationHarvestDataFunction
(const std::string& aFile,
 const std::string& aMathOperation,
 const std::string& aDataColumn,
 std::shared_ptr<XMLGeneratorSharedData> aSharedData,
 std::shared_ptr<XMLGeneratorPerformer> aPerformer,
 int aConcurrentEvaluations) : 
 XMLGeneratorOperation("harvest_data", "HarvestDataFromFile", aPerformer, aConcurrentEvaluations),
 mFile(aFile),
 mOperation(aMathOperation),
 mColumn(aDataColumn)
{
    mOutput.mLayout = "scalar";
    mOutput.mSize = "1";
    mOutput.mSharedData = aSharedData;
    mOutput.mArgumentName = mOutput.mSharedData->name();
    
    if( mConcurrentEvaluations == 0 )
    {
        mChDir = false;
    }
    else
    {
        addNameTag();
        mChDir = true;
    }
    
    mOnChange = true;
}

void XMLGeneratorOperationHarvestDataFunction::write_definition(pugi::xml_document& aDocument, std::string aEvaluationNumber)
{
    auto tOperationNode = aDocument.append_child("Operation");
    appendCommonChildren(tOperationNode,aEvaluationNumber);

    std::string tEvaluationTag;
    if(aEvaluationNumber == "")
        tEvaluationTag = mEvaluationTag;
    else
        tEvaluationTag = aEvaluationNumber;

    if(mChDir)
        addChild(tOperationNode, "ChDir", std::string("evaluations_") + tEvaluationTag);

    addChild(tOperationNode, "File", mFile);
    addChild(tOperationNode, "Operation", mOperation);
    addChild(tOperationNode, "Column", mColumn);

    auto tOutputNode = tOperationNode.append_child("Output");
    addChild(tOutputNode, "ArgumentName", std::regex_replace (mOutput.mArgumentName,mTagExpression,aEvaluationNumber) );
    addChild(tOutputNode, "Layout", mOutput.mLayout);
    addChild(tOutputNode, "Size", mOutput.mSize);
}


void XMLGeneratorOperationHarvestDataFunction::write_interface(pugi::xml_document& aDocument, std::string aEvaluationNumber)
{
    auto tOperationNode = aDocument.append_child("Operation");
    addChild(tOperationNode, "Name", name(aEvaluationNumber));
    addChild(tOperationNode, "PerformerName",  mPerformer->name(aEvaluationNumber));
    auto tOutputNode = tOperationNode.append_child("Output");
    addChild(tOutputNode, "ArgumentName", mOutput.mArgumentName);
    addChild(tOutputNode, "SharedDataName", mOutput.mSharedData->name(aEvaluationNumber));
}

}