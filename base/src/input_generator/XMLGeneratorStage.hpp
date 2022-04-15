/*
 * XMLGeneratorStage.hpp
 *
 *  Created on: April 14, 2022
 */
#pragma once

#include <vector>
#include <string>
#include <bits/stdc++.h>
#include "pugixml.hpp"
#include "XMLGeneratorOperation.hpp"

namespace XMLGen
{

typedef std::pair<int,std::shared_ptr<XMLGeneratorOperation>> mPriorityOperation;

class XMLGeneratorStage
{
    
private:
    std::string mName;
    std::string mInputSharedData;
    std::string mOutputSharedData;
    std::priority_queue<mPriorityOperation, std::vector<mPriorityOperation>, std::greater<mPriorityOperation> > mOperationQueue;

public:
    XMLGeneratorStage
    (const std::string& aName,
    const std::string& aInputSharedData,
    const std::string& aOutputSharedData);

    void addStageOperation(std::shared_ptr<XMLGeneratorOperation> aOperation, int aPriority);
    void write(pugi::xml_document& aDocument);
};

}