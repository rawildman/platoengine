/*
 * XMLGeneratorSharedData.hpp
 *
 *  Created on: April 12, 2022
 */
#pragma once

#include <vector>
#include <string>

#include "pugixml.hpp"

#include "XMLGeneratorFileObject.hpp"
#include "XMLGeneratorPerformer.hpp"

namespace XMLGen
{

class XMLGeneratorSharedData : public XMLGeneratorFileObject
{
    
protected:
    std::shared_ptr<XMLGeneratorPerformer> mOwnerPerformer;
    std::vector< std::shared_ptr<XMLGeneratorPerformer>> mUserPerformers;
    
public:
    XMLGeneratorSharedData(const std::string& aName,
                           std::shared_ptr<XMLGeneratorPerformer> aOwnerPerformer,
                           const std::vector<std::shared_ptr<XMLGeneratorPerformer>>& aUserPerformers,
                           int aConcurrentEvaluations = 0);
    virtual void write_interface(pugi::xml_node& aNode,
                                 std::string aEvaluationString = "") = 0;
    
};

class XMLGeneratorSharedDataGlobal : public XMLGeneratorSharedData
{
    
private:
    std::string mSize;
    
public:
    XMLGeneratorSharedDataGlobal(const std::string& aName,
                                 const std::string& aSize,
                                 std::shared_ptr<XMLGeneratorPerformer> aOwnerPerformer,
                                 const std::vector<std::shared_ptr<XMLGeneratorPerformer>>& aUserPerformers,
                                 int aConcurrentEvaluations = 0);
    void write_interface(pugi::xml_node& aNode,
                         std::string aEvaluationString = "") override;
    
};

class XMLGeneratorSharedDataNodalField : public XMLGeneratorSharedData
{
    
public:
    XMLGeneratorSharedDataNodalField(const std::string& aName,
                                     std::shared_ptr<XMLGeneratorPerformer> aOwnerPerformer,
                                     const std::vector<std::shared_ptr<XMLGeneratorPerformer>>& aUserPerformers,
                                     int aConcurrentEvaluations = 0);
    void write_interface(pugi::xml_node& aNode,
                         std::string aEvaluationString = "") override;
    
};



}