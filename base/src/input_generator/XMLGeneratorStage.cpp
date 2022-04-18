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
 mName(aName),
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
    
    addChild(tStageNode, "Name", mName);

    for(unsigned int iOperation = 0; iOperation < mOperationQueue.size(); ++iOperation)
    {
        mOperationQueue[iOperation]->write_interface(aDocument);
        
    }
    
}

}