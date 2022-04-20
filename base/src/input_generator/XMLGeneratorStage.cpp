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

    auto tForOrStageNode = for_node(tStageNode,"Parameters");
    mInputSharedData->write_stage(tForOrStageNode);
    
    for(unsigned int iOperation = 0; iOperation < mOperationQueue.size(); ++iOperation)
    {
        tForOrStageNode = for_node(tStageNode,"Parameters");
        mOperationQueue[iOperation]->write_interface(tForOrStageNode); 
    }

    tForOrStageNode = for_node(tStageNode,"Parameters");
    mOutputSharedData->write_stage(tForOrStageNode);
    
}

}