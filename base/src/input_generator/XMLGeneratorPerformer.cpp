/*
 * XMLGeneratorPerformer.cpp
 *
 *  Created on: April 14, 2022
 */

#include "XMLGeneratorPerformer.hpp"
#include "XMLGeneratorUtilities.hpp"
#include "XMLGeneratorPlatoMainOperationFileUtilities.hpp"

namespace XMLGen
{
    XMLGeneratorPerformer::XMLGeneratorPerformer
    (
    const std::string& aName,
    const std::string& aCode,
    int aConcurrentEvaluations) :
    mName(aName),
    mCode(aCode),
    mConcurrentEvaluations(aConcurrentEvaluations)
    {
        if(mName == "platomain")
        {
            mPerformerID = "0";
            mCode = mName;
            mConcurrentEvaluations = 0;
        }
        else
        {
            mEvaluationTag = "{E}";
            mTagExpression = "\\{E\\}"; 
            mPerformerEvaluationTag = "{E+1}";
           

            if(mConcurrentEvaluations!=0)
            {
                mName += "_" + mEvaluationTag;
                mPerformerID = mPerformerEvaluationTag ;
            }
            else
            {
                mPerformerID = 1;

            }
        }

    }

    void XMLGeneratorPerformer::write(pugi::xml_document& aDocument,std::string aEvaluationNumber)
    {
       
        std::string tEvaluationTag; 
        std::string tPerformerEvaluationTag;
        if(aEvaluationNumber == "")
        {
            tEvaluationTag = mEvaluationTag;
            tPerformerEvaluationTag = mPerformerEvaluationTag;
        }
        else
        {
            tEvaluationTag = aEvaluationNumber;
            tPerformerEvaluationTag = std::to_string(std::stoi(aEvaluationNumber)+1);
        }
        if(mName == "platomain")
            tPerformerEvaluationTag = "0";

        auto tPerformerNode = aDocument.append_child("Performer");
        addChild(tPerformerNode, "PerformerID", tPerformerEvaluationTag);
        addChild(tPerformerNode, "Name",name(aEvaluationNumber));
        addChild(tPerformerNode, "Code", mCode);
    }

    std::string XMLGeneratorPerformer::name(std::string aEvaluationNumber)
    {
        if(aEvaluationNumber=="")
            return mName;
        else
            return std::regex_replace (mName,mTagExpression,aEvaluationNumber) ;
    }
    std::string XMLGeneratorPerformer::ID(std::string aEvaluationNumber)
    {
        return std::regex_replace (mName,mTagExpression,aEvaluationNumber) ;
    }
}