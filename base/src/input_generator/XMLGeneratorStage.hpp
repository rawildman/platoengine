/*
 * XMLGeneratorStage.hpp
 *
 *  Created on: April 14, 2022
 */
#pragma once

#include <vector>
#include <string>
#include "pugixml.hpp"

#include "XMLGeneratorFileObject.hpp"
#include "XMLGeneratorOperation.hpp"
#include "XMLGeneratorSharedData.hpp"

namespace XMLGen
{

class XMLGeneratorStage : public XMLGeneratorFileObject
{
    
private:
    
    std::shared_ptr<XMLGeneratorSharedData> mInputSharedData;
    std::shared_ptr<XMLGeneratorSharedData> mOutputSharedData;
    std::vector<std::shared_ptr<XMLGeneratorOperation>> mOperations;

    void appendInput(pugi::xml_node& aNode,
                     const std::string& aTag = "");
    void appendOutput(pugi::xml_node& aNode);

public:
    XMLGeneratorStage(const std::string& aName,
                      const std::vector<std::shared_ptr<XMLGeneratorOperation>>& aOperations,
                      std::shared_ptr<XMLGeneratorSharedData> aInputSharedData = nullptr,
                      std::shared_ptr<XMLGeneratorSharedData> aOutputSharedData = nullptr);

    void write(pugi::xml_document& aDocument);
    void write_dakota(pugi::xml_node& aDocument,
                      const std::string& aStageTag);

    std::string inputSharedDataName();
    std::string outputSharedDataName();
};

}