/*
 * XMLGeneratorSharedData.cpp
 *
 *  Created on: April 14, 2022
 */

#include "XMLGeneratorSharedData.hpp"
#include "XMLGeneratorUtilities.hpp"
#include "XMLGeneratorPlatoMainOperationFileUtilities.hpp"

namespace XMLGen
{
    XMLGeneratorSharedData::XMLGeneratorSharedData
    (const std::string& aName,
     std::shared_ptr<XMLGeneratorPerformer> aOwnerPerformer,
     const std::vector<std::shared_ptr<XMLGeneratorPerformer>>& aUserPerformers,
     int aConcurrentEvaluations) :
     XMLGeneratorFileObject(aName,aConcurrentEvaluations)
    {
        mOwnerPerformer = aOwnerPerformer;
        mUserPerformers = aUserPerformers;
    }
    
    XMLGeneratorSharedDataGlobal::XMLGeneratorSharedDataGlobal
    (const std::string& aName,
     const std::string& aSize,
     std::shared_ptr<XMLGeneratorPerformer> aOwnerPerformer,
     const std::vector<std::shared_ptr<XMLGeneratorPerformer>>& aUserPerformers,
     int aConcurrentEvaluations) :
     XMLGeneratorSharedData(aName, aOwnerPerformer, aUserPerformers, aConcurrentEvaluations),
     mSize(aSize)
    {
    }

    void XMLGeneratorSharedDataGlobal::write_interface
    (pugi::xml_node& aNode,
     std::string aEvaluationString)
    {
        auto tSharedDataNode = aNode.append_child("SharedData");
        addChild(tSharedDataNode, "Name", name(aEvaluationString));
        addChild(tSharedDataNode, "Type", "Scalar");
        addChild(tSharedDataNode, "Layout", "Global");
        addChild(tSharedDataNode, "Size", mSize);   
        addChild(tSharedDataNode, "OwnerName", mOwnerPerformer->name(aEvaluationString) );
        for(unsigned int iUserPerformer = 0 ; iUserPerformer < mUserPerformers.size(); iUserPerformer++)
            addChild(tSharedDataNode, "UserName", mUserPerformers[iUserPerformer]->name(aEvaluationString));
    }

    XMLGeneratorSharedDataNodalField::XMLGeneratorSharedDataNodalField
    (const std::string& aName,
     std::shared_ptr<XMLGeneratorPerformer> aOwnerPerformer,
     const std::vector<std::shared_ptr<XMLGeneratorPerformer>>& aUserPerformers,
     int aConcurrentEvaluations) :
     XMLGeneratorSharedData(aName, aOwnerPerformer, aUserPerformers, aConcurrentEvaluations)
    {
    }

    void XMLGeneratorSharedDataNodalField::write_interface
    (pugi::xml_node& aNode,
     std::string aEvaluationString)
    {
        auto tSharedDataNode = aNode.append_child("SharedData");
        addChild(tSharedDataNode, "Name", name(aEvaluationString));
        addChild(tSharedDataNode, "Type", "Scalar");
        addChild(tSharedDataNode, "Layout", "Nodal Field");   
        addChild(tSharedDataNode, "OwnerName", mOwnerPerformer->name(aEvaluationString) );
        for(unsigned int iUserPerformer = 0 ; iUserPerformer < mUserPerformers.size(); iUserPerformer++)
            addChild(tSharedDataNode, "UserName", mUserPerformers[iUserPerformer]->name(aEvaluationString));
    }


}