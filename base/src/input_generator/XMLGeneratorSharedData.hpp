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

#include "XMLGeneratorFileObject.hpp"
#include "XMLGeneratorPerformer.hpp"

namespace XMLGen
{

class XMLGeneratorSharedData : public XMLGeneratorFileObject
{
    
private:
    std::string mSize;
    std::shared_ptr<XMLGeneratorPerformer> mOwnerPerformer;
    std::vector< std::shared_ptr<XMLGeneratorPerformer>> mUserPerformers;
    
public:
    XMLGeneratorSharedData(const std::string& aName,
                          const std::string& aSize,
                          std::shared_ptr<XMLGeneratorPerformer> aOwnerPerformer,
                          const std::vector<std::shared_ptr<XMLGeneratorPerformer>>& aUserPerformers,
                          int aConcurrentEvaluations);
    void write_interface(pugi::xml_node& aNode,std::string aEvaluationString = "");
    
};

}