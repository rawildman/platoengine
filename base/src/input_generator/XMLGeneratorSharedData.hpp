/*
 * XMLGeneratorSharedData.hpp
 *
 *  Created on: April 12, 2022
 */
#pragma once

#include <vector>
#include <string>
#include <regex>

#include "pugixml.hpp"

#include "XMLGeneratorPerformer.hpp"

namespace XMLGen
{

class XMLGeneratorSharedData
{
    
private:
    
    std::string mName;
    std::string mSize;
    std::shared_ptr<XMLGeneratorPerformer> mOwnerPerformer;
    std::vector< std::shared_ptr<XMLGeneratorPerformer>> mUserPerformers;
    int mConcurrentEvaluations;
    
    std::string mEvaluationTag;
    std::regex mTagExpression;
public:
    XMLGeneratorSharedData(const std::string& aName,
                          const std::string& aSize,
                          std::shared_ptr<XMLGeneratorPerformer> aOwnerPerformer,
                          const std::vector<std::shared_ptr<XMLGeneratorPerformer>>& aUserPerformers,
                          int aConcurrentEvaluations);
    void write(pugi::xml_document& aDocument,std::string aEvaluationNumber = "");
    
    std::string name(std::string aEvaluationNumber = "");
};

}