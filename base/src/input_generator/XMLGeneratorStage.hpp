/*
 * XMLGeneratorStage.hpp
 *
 *  Created on: April 14, 2022
 */
#pragma once

#include <vector>
#include <string>

namespace XMLGen
{

class XMLGeneratorStage
{
    
private:
    std::string mName;
    std::string mInputSharedData;
    std::string mOutputSharedData;
    std::vector<std::string> mOperationNames;

public:
    XMLGeneratorStage(const std::string& aName,
                      const std::string& aInputSharedData,
                      const std::string& aOutputSharedData);
    void write(pugi::xml_document& aDocument,
               const std::vector<XMLGen:Operation>& aOperations);
};

}