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
    const std::string& aSize,
    std::shared_ptr<XMLGeneratorPerformer> aOwnerPerformer,
    const std::vector<std::shared_ptr<XMLGeneratorPerformer>>& aUserPerformers,
    int aConcurrentEvaluations) :
    mSize(aSize),
    mConcurrentEvaluations(aConcurrentEvaluations)
    {
        mName = aName;
        mOwnerPerformer = aOwnerPerformer;
        mUserPerformers = aUserPerformers;
        mEvaluationTag = "{E}";
        mTagExpression = "\\{E\\}";
        
        if(mConcurrentEvaluations!=0)
        {
            mName += "_" + mEvaluationTag;
        }
    }
    void XMLGeneratorSharedData::write(pugi::xml_document& aDocument,std::string aEvaluationNumber)
    {
        std::string tEvaluationTag;
        if(aEvaluationNumber == "")
            tEvaluationTag = mEvaluationTag;
        else
            tEvaluationTag = aEvaluationNumber;

        auto tSharedDataNode = aDocument.append_child("SharedData");
        addChild(tSharedDataNode, "Name", name(aEvaluationNumber));
        addChild(tSharedDataNode, "Type", "Scalar");
        addChild(tSharedDataNode, "Layout", "Global");
        addChild(tSharedDataNode, "Size", mSize);
        addChild(tSharedDataNode, "OwnerName", std::regex_replace (mOwnerPerformer->name(aEvaluationNumber),mTagExpression,tEvaluationTag));
        for(unsigned int iUserPerformer = 0 ; iUserPerformer < mUserPerformers.size(); iUserPerformer++)
            addChild(tSharedDataNode, "UserName", mUserPerformers[iUserPerformer]->name(aEvaluationNumber));
    }
    
    
    std::string XMLGeneratorSharedData::name(std::string aEvaluationNumber)
    {
        if(aEvaluationNumber=="")
            return mName;
        else
            return std::regex_replace (mName,mTagExpression,aEvaluationNumber) ;
    }
}