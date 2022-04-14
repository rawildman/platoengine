/*
 * XMLGeneratorPerformer.hpp
 *
 *  Created on: April 12, 2022
 */
#pragma once

#include <vector>
#include <string>

#include "pugixml.hpp"

namespace XMLGen
{

class XMLGeneratorPerformer
{
    
private:
    std::string mPerformerID;
    std::string mName;
    std::string mCode;
    
public:
    XMLGeneratorPerformer(const std::string& aPerformerID,
                          const std::string& aName,
                          const std::string& aCode);
    void write(pugi::xml_document& aDocument);
    std::string name();
};

}