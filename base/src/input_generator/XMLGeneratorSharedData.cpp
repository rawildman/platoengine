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
    const std::string& aOwnerName,
    const std::vector<std::string>& aUserName,
    int aConcurrentEvaluations) :
    mSize(aSize),
    mConcurrentEvaluations(aConcurrentEvaluations)
    {
        mName = aName;
        mOwnerName = aOwnerName;
        mUserName = aUserName;
        mEvaluationTag = "{E}";
        mTagExpression = "\\{E\\}";
        
        if(mConcurrentEvaluations!=0)
        {
            mName += "_" + mEvaluationTag;
            if(mOwnerName == "platomain_1")
            {
                for(unsigned int iUserName = 1 ; iUserName < mUserName.size(); iUserName++)
                    mUserName[iUserName] += "_" + mEvaluationTag;
            }
            else
                mOwnerName  += "_" + mEvaluationTag;
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
        addChild(tSharedDataNode, "OwnerName", std::regex_replace (mOwnerName,mTagExpression,tEvaluationTag));
        for(unsigned int iUserName = 0 ; iUserName < mUserName.size(); iUserName++)
            addChild(tSharedDataNode, "UserName", std::regex_replace (mUserName[iUserName],mTagExpression,tEvaluationTag));
    }
    
    
    std::string XMLGeneratorSharedData::name(std::string aEvaluationNumber)
    {
        if(aEvaluationNumber=="")
            return mName;
        else
            return std::regex_replace (mName,mTagExpression,aEvaluationNumber) ;
    }
}