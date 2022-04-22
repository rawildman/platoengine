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
    int aNumberRanks,
    int aConcurrentEvaluations) : 
    XMLGeneratorFileObject(aName,aConcurrentEvaluations),
    mCode(aCode),
    mNumberRanks(aNumberRanks)
    {
        if(name() == "platomain") ///NEED Singleton for main 
        {
            mPerformerID = "0";
            mCode = name();
        }
        else
        {
            if(evaluations()!=0)
                mPerformerID = "{E+1}" ;
            else
                mPerformerID = 1;
        }

    }

    void XMLGeneratorPerformer::write_interface
    (pugi::xml_document& aDocument,
     int aIDOffset,
     std::string aEvaluationString)
    {
        auto tPerformerNode = aDocument.append_child("Performer");
        addChild(tPerformerNode, "PerformerID", ID(aIDOffset, aEvaluationString));
        addChild(tPerformerNode, "Name", name(aEvaluationString));
        addChild(tPerformerNode, "Code", mCode);
    }

    std::string XMLGeneratorPerformer::ID(int aIDOffset,
                                          std::string aEvaluationString)
    {
        ///ID tag is off by 1 from evaluation number because PlatoMain has special role
        // if(name() == "platomain" )
        //     return std::string("0");
        if(aEvaluationString == "")
           return  std::to_string(aIDOffset);
        else
            // return std::to_string(std::stoi(aEvaluationString) + aIDOffset);
            return  std::string("{E+") +std::to_string(aIDOffset) + "}";
    }
}