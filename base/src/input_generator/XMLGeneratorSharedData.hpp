/*
 * XMLGeneratorSharedData.hpp
 *
 *  Created on: April 12, 2022
 */
#pragma once

#include <vector>
#include <string>

#include "pugixml.hpp"

namespace XMLGen
{

class XMLGeneratorSharedData
{
    
private:
    
    std::string mName;
    std::string mSize;
    std::string mOwnerName;
    std::vector<std::string> mUserName;
    
public:
    XMLGeneratorSharedData(const std::string& aName,
                          const std::string& aSize,
                          const std::string& aOwnerName,
                          const std::vector<std::string>& aUserName);
    void write(pugi::xml_document& aDocument);
    std::string name();
};

}