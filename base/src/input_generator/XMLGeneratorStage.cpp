/*
 * XMLGeneratorStage.cpp
 *
 *  Created on: April 14, 2022
 */

#include "XMLGeneratorStage.hpp"


namespace XMLGen
{
    
XMLGeneratorStage::XMLGeneratorStage
(const std::string& aName,
 const std::string& aInputSharedData,
 const std::string& aOutputSharedData):
 mName(aName),
 mInputSharedData(aInputSharedData),
 mOutputSharedData(aOutputSharedData)
{
}

void XMLGeneratorStage::addStageOperation(std::shared_ptr<XMLGeneratorOperation> aOperation, int aPriority)
{
    mOperationQueue.push(std::make_pair(aPriority,aOperation));
}

void XMLGeneratorStage::write
(pugi::xml_document& aDocument)
{
    /*
    auto tStageNode = aDocument.append_child("Stage");
    addChild(tStageNode, "Name", mName);

    this->write_input();
    for(tOperationName : mOperationNames)
    {
        auto tOperation = XMLGen::find_operation(tOperationName); // NEED TO IMPLEMENT THIS IN UTILITIES
        tOperation.write_interface(); // NEED TO IMPLEMENT THIS IN OPERATION CLASS
    }
    this->write_output();*/
}

}