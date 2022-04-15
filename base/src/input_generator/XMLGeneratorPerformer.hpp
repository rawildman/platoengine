/*
 * XMLGeneratorPerformer.hpp
 *
 *  Created on: April 12, 2022
 */
#pragma once

#include <vector>
#include <string>
#include <regex>

#include "pugixml.hpp"

namespace XMLGen
{

class XMLGeneratorPerformer
{
    
private:
    std::string mPerformerID;
    std::string mName;
    std::string mCode;
    std::regex mTagExpression;
    std::string mEvaluationTag;

    int mConcurrentEvaluations;
    
public:
    XMLGeneratorPerformer(
                          const std::string& aName,
                          const std::string& aCode,
                          int aConcurrentEvalutions);
    void write(pugi::xml_document& aDocument,std::string aEvaluationNumber ="");
    std::string name(std::string aEvaluationNumber ="");
};

}