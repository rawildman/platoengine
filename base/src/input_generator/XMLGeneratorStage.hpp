/*
 * XMLGeneratorStage.hpp
 *
 *  Created on: April 14, 2022
 */
#pragma once

#include <vector>
#include <string>
//#include <bits/stdc++.h>
#include "pugixml.hpp"

#include "XMLGeneratorFileObject.hpp"
#include "XMLGeneratorOperation.hpp"
#include "XMLGeneratorSharedData.hpp"

namespace XMLGen
{

typedef std::pair<int,std::shared_ptr<XMLGeneratorOperation>> mPriorityOperation;

class XMLGeneratorStage : public XMLGeneratorFileObject
{
    
private:
    
    std::shared_ptr<XMLGeneratorSharedData> mInputSharedData;
    std::shared_ptr<XMLGeneratorSharedData> mOutputSharedData;
    //std::priority_queue<mPriorityOperation, std::vector<mPriorityOperation>, std::greater<mPriorityOperation> > mOperationQueue;
    std::vector<std::shared_ptr<XMLGeneratorOperation>> mOperationQueue;

    void appendInput(pugi::xml_node& aNode);
    void appendOutput(pugi::xml_node& aNode);

public:
    XMLGeneratorStage
    (const std::string& aName,
    std::shared_ptr<XMLGeneratorSharedData> aInputSharedData,
    std::shared_ptr<XMLGeneratorSharedData> aOutputSharedData);

    void addStageOperation(std::shared_ptr<XMLGeneratorOperation> aOperation);
    void write(pugi::xml_document& aDocument);
};

}