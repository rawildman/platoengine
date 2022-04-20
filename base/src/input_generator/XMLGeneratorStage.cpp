/*
 * XMLGeneratorStage.cpp
 *
 *  Created on: April 14, 2022
 */

#include "XMLGeneratorStage.hpp"
#include "XMLGeneratorUtilities.hpp"

namespace XMLGen
{
    
XMLGeneratorStage::XMLGeneratorStage
(const std::string& aName,
 std::shared_ptr<XMLGeneratorSharedData> aInputSharedData,
 std::shared_ptr<XMLGeneratorSharedData> aOutputSharedData):
 XMLGeneratorFileObject(aName),
 mInputSharedData(aInputSharedData),
 mOutputSharedData(aOutputSharedData)
{
}

void XMLGeneratorStage::addStageOperation(std::shared_ptr<XMLGeneratorOperation> aOperation)
{
    //mOperationQueue.push(std::make_pair(aPriority,aOperation));
    mOperationQueue.push_back(aOperation);
}

void XMLGeneratorStage::write
(pugi::xml_document& aDocument)
{
    auto tStageNode = aDocument.append_child("Stage");
    
    addChild(tStageNode, "Name", name());

    this->appendInput(tStageNode);

    for(unsigned int iOperation = 0; iOperation < mOperationQueue.size(); ++iOperation)
    {
        auto tForOrStageNode = forNode(tStageNode,"Parameters");
        mOperationQueue[iOperation]->write_interface(tForOrStageNode); 
    }

    this->appendOutput(tStageNode);
}

void XMLGeneratorStage::appendInput(pugi::xml_node& aNode)
{
    if(mInputSharedData)
    {
        auto tForOrStageNode = forNode(aNode,"Parameters");
        auto tSharedDataNode = tForOrStageNode.append_child("Input");
        XMLGen::append_children({"SharedDataName"},{mInputSharedData->name()},tSharedDataNode);
    }
}

void XMLGeneratorStage::appendOutput(pugi::xml_node& aNode)
{
    if(mOutputSharedData)
    {
        auto tForOrStageNode = forNode(aNode,"Parameters");
        auto tSharedDataNode = tForOrStageNode.append_child("Input");
        XMLGen::append_children({"SharedDataName"},{mOutputSharedData->name()},tSharedDataNode);
    }
}


}