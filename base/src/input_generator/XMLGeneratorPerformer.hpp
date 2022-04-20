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
    int mNumberRanks;
       
public:
    XMLGeneratorPerformer(const std::string& aName,
                          const std::string& aCode,
                          int aNumberRanks = 1,
                          int aConcurrentEvalutions = 0);

    void write_interface(pugi::xml_document& aDocument,
                int aIDOffset = 1,
               std::string aEvaluationString ="");
    
    int numberRanks(){return mNumberRanks;}
    
    std::string ID(int aIDOffset = 1,
                    std::string aEvaluationString = "");
};

}