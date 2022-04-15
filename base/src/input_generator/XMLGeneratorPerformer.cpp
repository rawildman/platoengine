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
        mEvaluationTag = "{E}";
        mTagExpression = "\\{E\\}"; 

        if(mConcurrentEvaluations!=0)
        {
            mName += "_" + mEvaluationTag;
            mPerformerID = "{E+1}";
        }
    }

    void XMLGeneratorPerformer::write(pugi::xml_document& aDocument,std::string aEvaluationNumber)
    {
       
        std::string tEvaluationTag; 
        if(aEvaluationNumber == "")
            tEvaluationTag = mEvaluationTag;
        else
            tEvaluationTag = std::to_string(std::stoi(aEvaluationNumber)+1);

        auto tPerformerNode = aDocument.append_child("Perfomer");
        addChild(tPerformerNode, "PerformerID", std::regex_replace (mPerformerID,mTagExpression,tEvaluationTag));
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
}