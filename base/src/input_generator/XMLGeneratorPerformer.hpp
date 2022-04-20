/*
 * XMLGeneratorPerformer.hpp
 *
 *  Created on: April 12, 2022
 */
#pragma once

#include <vector>
#include <string>
#include <regex>

#include "XMLGeneratorFileObject.hpp"

#include "pugixml.hpp"


namespace XMLGen
{

class XMLGeneratorPerformer : public XMLGeneratorFileObject
{
    
private:
    std::string mPerformerID;
    std::string mCode;
    
    
public:
    XMLGeneratorPerformer(
                          const std::string& aName,
                          const std::string& aCode,
                          int aConcurrentEvalutions = 0);
    void write(pugi::xml_document& aDocument,std::string aEvaluationString ="");
    
    std::string ID(std::string aEvaluationString = "");
};

}