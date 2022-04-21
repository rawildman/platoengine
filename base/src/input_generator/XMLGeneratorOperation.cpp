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
 XMLGeneratorFileObject(aName,aConcurrentEvaluations),
 mFunction(aFunction),
 mPerformer(aPerformer)
{
    if( evaluations() == 0 )
        mChDir = false;
    else
        mChDir = true;
}

void XMLGeneratorOperation::appendCommonChildren
(pugi::xml_node& aOperationNode,
 std::string aEvaluationString)
{
    addChild(aOperationNode, "Function", mFunction);
    addChild(aOperationNode, "Name", name(aEvaluationString));
}

pugi::xml_node XMLGeneratorOperation::forNode(pugi::xml_node& aNode, 
                                              const std::string& aXMLLoopVectorName)
{
    if(evaluations() == 0)
        return aNode; 
    else
    {
        auto tOperation = aNode.append_child("Operation");
        auto tForNode = tOperation.append_child("For");
        tForNode.append_attribute("var") = loopVar().c_str();
        tForNode.append_attribute("in") = aXMLLoopVectorName.c_str();
        return tForNode;
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
    mOnChange = false;    
}

void XMLGeneratorOperationWait::write_definition(pugi::xml_document& aDocument, std::string aEvaluationString)
{
    auto tOperationNode = aDocument.append_child("Operation");
    appendCommonChildren(tOperationNode,aEvaluationString);
    addChild(tOperationNode, "Command", mCommand);
    if(mChDir)
        addChild(tOperationNode, "ChDir", std::string("evaluations_") + tag(aEvaluationString));
    addChild(tOperationNode, "OnChange", (mOnChange ? "true" : "false"));
}

void XMLGeneratorOperationWait::write_interface(pugi::xml_node& aNode, std::string aEvaluationString)
{
    auto tOperationNode = aNode.append_child("Operation");
    addChild(tOperationNode, "Name", name(aEvaluationString));
    addChild(tOperationNode, "PerformerName", mPerformer->name(aEvaluationString));
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
    mOnChange = true;
}

 void XMLGeneratorOperationGemmaMPISystemCall::write_definition(pugi::xml_document& aDocument, std::string aEvaluationString)
 {
    auto tOperationNode = aDocument.append_child("Operation");
    appendCommonChildren(tOperationNode,aEvaluationString);
    addChild(tOperationNode, "Command", mCommand);
    if(mChDir)
        addChild(tOperationNode, "ChDir", std::string("evaluations_") + tag(aEvaluationString));
    addChild(tOperationNode, "OnChange", (mOnChange ? "true" : "false"));
    addChild(tOperationNode, "NumRanks", mNumRanks );
    addChild(tOperationNode, "Argument", mArgument );
    addChild(tOperationNode, "AppendInput", "false" );
 }

 void XMLGeneratorOperationGemmaMPISystemCall::write_interface(pugi::xml_node& aNode, std::string aEvaluationString)
{
    auto tOperationNode = aNode.append_child("Operation");
    addChild(tOperationNode, "Name", name(aEvaluationString));
    addChild(tOperationNode, "PerformerName",  mPerformer->name(aEvaluationString));
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

    mOnChange = true;
}

void XMLGeneratorOperationAprepro::write_definition(pugi::xml_document& aDocument, std::string aEvaluationString)
{
    auto tOperationNode = aDocument.append_child("Operation");
    appendCommonChildren(tOperationNode,aEvaluationString);
    addChild(tOperationNode, "Command", mCommand);

    if(mChDir)
        addChild(tOperationNode, "ChDir", std::string("evaluations_") + tag(aEvaluationString) );
    addChild(tOperationNode, "OnChange", (mOnChange ? "true" : "false"));
    
    for( auto tA : mArgument )
        addChild(tOperationNode, "Argument", tA );
    
    addChild(tOperationNode, "AppendInput", "true" );

    for( auto tO : mOptions )
        addChild(tOperationNode, "Option", tO );
    
    auto tInputNode = tOperationNode.append_child("Input");
    addChild(tInputNode, "ArgumentName", "parameters");
    addChild(tInputNode, "Layout", mInput.mLayout);
    addChild(tInputNode, "Size", mInput.mSize);
}

void XMLGeneratorOperationAprepro::write_interface(pugi::xml_node& aNode, std::string aEvaluationString)
{
    auto tOperationNode = aNode.append_child("Operation");
    addChild(tOperationNode, "Name", name(aEvaluationString));
    addChild(tOperationNode, "PerformerName",  mPerformer->name(aEvaluationString));
    auto tInputNode = tOperationNode.append_child("Input");
    addChild(tInputNode, "ArgumentName", "parameters");
    addChild(tInputNode, "SharedDataName", mInput.mSharedData->name(aEvaluationString));
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
    //mOutput.mArgumentName = mOutput.mSharedData->name();
    
    mOnChange = true;
}

void XMLGeneratorOperationHarvestDataFunction::write_definition(pugi::xml_document& aDocument, std::string aEvaluationString)
{
    auto tOperationNode = aDocument.append_child("Operation");
    appendCommonChildren(tOperationNode,aEvaluationString);

    if(mChDir)
        addChild(tOperationNode, "ChDir", std::string("evaluations_") + tag(aEvaluationString));

    addChild(tOperationNode, "File", mFile);
    addChild(tOperationNode, "Operation", mOperation);
    addChild(tOperationNode, "Column", mColumn);

    auto tOutputNode = tOperationNode.append_child("Output");
    addChild(tOutputNode, "ArgumentName", "criterion value");
    addChild(tOutputNode, "Layout", mOutput.mLayout);
    addChild(tOutputNode, "Size", mOutput.mSize);
}

void XMLGeneratorOperationHarvestDataFunction::write_interface(pugi::xml_node& aNode, std::string aEvaluationString)
{
    auto tOperationNode = aNode.append_child("Operation");
    addChild(tOperationNode, "Name", name(aEvaluationString));
    addChild(tOperationNode, "PerformerName",  mPerformer->name(aEvaluationString));
    auto tOutputNode = tOperationNode.append_child("Output");
    addChild(tOutputNode, "ArgumentName", "criterion value");
    addChild(tOutputNode, "SharedDataName", mOutput.mSharedData->name(aEvaluationString));
}


XMLGeneratorOperationCubitTet10Conversion::XMLGeneratorOperationCubitTet10Conversion
(const std::string& aExodusFileName, 
 const std::vector<std::string>& aBlockList, 
 std::shared_ptr<XMLGeneratorSharedData> aSharedData,
 std::shared_ptr<XMLGeneratorPerformer> aPerformer, 
 int aConcurrentEvaluations) :
 XMLGeneratorOperation("cubit_convert_to_tet10", "SystemCall", aPerformer, aConcurrentEvaluations),
 mCommand("cubit")
{
    mArgument.push_back(" -input toTet10.jou -batch -nographics -nogui -noecho -nojournal -nobanner -information off");
    mInput.mSharedData = aSharedData;
    mOnChange = true;

    writeTet10ConversionFile(aExodusFileName,aBlockList);
}

void XMLGeneratorOperationCubitTet10Conversion::write_definition(pugi::xml_document& aDocument, std::string aEvaluationString)
{
    auto tOperationNode = aDocument.append_child("Operation");
    appendCommonChildren(tOperationNode,aEvaluationString);
    addChild(tOperationNode, "Command", mCommand);

    if(mChDir)
        addChild(tOperationNode, "ChDir", std::string("evaluations_") + tag(aEvaluationString) );
    addChild(tOperationNode, "OnChange", (mOnChange ? "true" : "false"));
    
    for( auto tA : mArgument )
        addChild(tOperationNode, "Argument", tA );
    
    addChild(tOperationNode, "AppendInput", "false" );

    auto tInputNode = tOperationNode.append_child("Input");
    addChild(tInputNode, "ArgumentName", "parameters");
}

void XMLGeneratorOperationCubitTet10Conversion::write_interface(pugi::xml_node& aNode, std::string aEvaluationString)
{
    auto tOperationNode = aNode.append_child("Operation");
    addChild(tOperationNode, "Name", name(aEvaluationString));
    addChild(tOperationNode, "PerformerName",  mPerformer->name(aEvaluationString));
    auto tInputNode = tOperationNode.append_child("Input");
    addChild(tInputNode, "ArgumentName", "parameters");
    addChild(tInputNode, "SharedDataName", mInput.mSharedData->name(aEvaluationString));
}

void XMLGeneratorOperationCubitTet10Conversion::writeTet10ConversionFile
(std::string aExodusFileName,
 std::vector<std::string> aBlockList)
{
    //Something to consider is where the file should be created and whether subfolders exist at this time 
    std::ofstream tOutFile("toTet10.jou");
    tOutFile<<"import mesh \""<<aExodusFileName<<"\" no_geom\n";
    for( auto tB : aBlockList )
        tOutFile<<"block "<<tB<<" element type tetra10\n";
    tOutFile<<"set exodus netcdf4 off\n";
    tOutFile<<"set large exodus file on\n";
    tOutFile<<"export mesh \""<<aExodusFileName<<"\" overwrite\n";
    tOutFile.close();

    for( unsigned int iEvaluation = 0 ; iEvaluation < evaluations(); ++iEvaluation)
    {  
        auto tCommand = std::string("mkdir -p ") + "evaluations_" + std::to_string(iEvaluation);
        Plato::system(tCommand.c_str());
        tCommand = std::string("cp ") + "toTet10.jou" + " " + "evaluations_" + std::to_string(iEvaluation);
        Plato::system(tCommand.c_str());
    }
}

XMLGeneratorOperationCubitSubBlock::XMLGeneratorOperationCubitSubBlock
(const std::string& aExodusFileName, 
 const std::vector<std::string>& aBoundingBox, 
 std::shared_ptr<XMLGeneratorSharedData> aSharedData,
 std::shared_ptr<XMLGeneratorPerformer> aPerformer, 
 int aConcurrentEvaluations) :
 XMLGeneratorOperation("create_sub_block", "SystemCall", aPerformer, aConcurrentEvaluations),
 mCommand("cubit")
{
    mArgument.push_back(" -input subBlock.jou -batch -nographics -nogui -noecho -nojournal -nobanner -information off");
    mInput.mSharedData = aSharedData;
    mOnChange = true;

    writeSubBlockFile(aExodusFileName,aBoundingBox);
}

void XMLGeneratorOperationCubitSubBlock::write_definition(pugi::xml_document& aDocument, std::string aEvaluationString)
{
    auto tOperationNode = aDocument.append_child("Operation");
    appendCommonChildren(tOperationNode,aEvaluationString);
    addChild(tOperationNode, "Command", mCommand);

    if(mChDir)
        addChild(tOperationNode, "ChDir", std::string("evaluations_") + tag(aEvaluationString) );
    addChild(tOperationNode, "OnChange", (mOnChange ? "true" : "false"));
    
    for( auto tA : mArgument )
        addChild(tOperationNode, "Argument", tA );
    
    addChild(tOperationNode, "AppendInput", "false" );

    auto tInputNode = tOperationNode.append_child("Input");
    addChild(tInputNode, "ArgumentName", mInput.mSharedData->name(aEvaluationString));
}

void XMLGeneratorOperationCubitSubBlock::write_interface(pugi::xml_node& aNode, std::string aEvaluationString)
{
    auto tOperationNode = aNode.append_child("Operation");
    addChild(tOperationNode, "Name", name(aEvaluationString));
    addChild(tOperationNode, "PerformerName",  mPerformer->name(aEvaluationString));
    auto tInputNode = tOperationNode.append_child("Input");
    addChild(tInputNode, "ArgumentName", mInput.mSharedData->name(aEvaluationString));
    addChild(tInputNode, "SharedDataName", mInput.mSharedData->name(aEvaluationString));
}

void XMLGeneratorOperationCubitSubBlock::writeSubBlockFile
(std::string aExodusFileName,
 std::vector<std::string> aBoundingBox)
{
    std::vector<std::string> tCoords = {"x_coord >= ", "and y_coord >= ", "and z_coord >= ", "and x_coord <= ", "and y_coord <= ", "and z_coord <= " };
    //Something to consider is where the file should be created and whether subfolders exist at this time 
    std::ofstream tOutFile("subBlock.jou");
    tOutFile<<"import mesh \""<<aExodusFileName<<"\" no_geom\n";
    tOutFile<<"delete block all\n";
    tOutFile<<"block 2 tet with ";
    for( unsigned int iB = 0; iB < aBoundingBox.size(); ++iB )
        tOutFile<<tCoords[iB]<<aBoundingBox[iB];
    tOutFile<<"\n";
    tOutFile<<"block 1 tet all\n";
    tOutFile<<"set exodus netcdf4 off\n";
    tOutFile<<"set large exodus file on\n";
    tOutFile<<"export mesh \""<<aExodusFileName<<"\" overwrite\n";
    tOutFile.close();

    for( unsigned int iEvaluation = 0 ; iEvaluation < evaluations(); ++iEvaluation)
    {  
        auto tCommand = std::string("mkdir -p ") + "evaluations_" + std::to_string(iEvaluation);
        Plato::system(tCommand.c_str());
        tCommand = std::string("cp ") + "subBlock.jou" + " " + "evaluations_" + std::to_string(iEvaluation);
        Plato::system(tCommand.c_str());
    }
}

XMLGeneratorOperationDecomp::XMLGeneratorOperationDecomp
(const std::string& aExodusFileName, 
 int aNumberOfPartitions,
 std::shared_ptr<XMLGeneratorPerformer> aPerformer, 
 int aConcurrentEvaluations) :
 XMLGeneratorOperation("decomp_mesh", "SystemCall", aPerformer, aConcurrentEvaluations),
 mCommand("decomp")
{
    mArgument.push_back(" -p ");
    mArgument.push_back(std::to_string(aNumberOfPartitions));
    mArgument.push_back(" ");
    mArgument.push_back(aExodusFileName);
    mOnChange = false;
}

void XMLGeneratorOperationDecomp::write_definition(pugi::xml_document& aDocument, std::string aEvaluationString)
{
    auto tOperationNode = aDocument.append_child("Operation");
    appendCommonChildren(tOperationNode,aEvaluationString);
    addChild(tOperationNode, "Command", mCommand);

    if(mChDir)
        addChild(tOperationNode, "ChDir", std::string("evaluations_") + tag(aEvaluationString) );
    
    for( auto tA : mArgument )
        addChild(tOperationNode, "Argument", tA );    
}

void XMLGeneratorOperationDecomp::write_interface(pugi::xml_node& aNode, std::string aEvaluationString)
{
    auto tOperationNode = aNode.append_child("Operation");
    addChild(tOperationNode, "Name", name(aEvaluationString));
    addChild(tOperationNode, "PerformerName",  mPerformer->name(aEvaluationString));
}



}