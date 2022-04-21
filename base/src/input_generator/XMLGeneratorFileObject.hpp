/*
 * XMLGeneratorFileObject.hpp
 *
 *  Created on: April 20, 2022
 */
#pragma once

#include <vector>
#include <string>
#include <memory>
#include <regex>

#include "pugixml.hpp"

namespace XMLGen
{

class XMLGeneratorFileObject
{
private:
    std::string mName;
    int mConcurrentEvaluations;
    std::string mEvaluationTag;
    std::regex mTagExpression;

public:
    XMLGeneratorFileObject(const std::string& aName,
                          int aConcurrentEvaluations = 0);
    
    //virtual void write_definition(pugi::xml_document& aDocument, std::string aEvaluationString = "") = 0;
    //virtual void write_interface(pugi::xml_node& aNode, std::string aEvaluationString = "") = 0;
    
    //void for_write_definition(pugi::xml_document& aDocument, std::string aLoopMaxVar);
    
    int evaluations(){return mConcurrentEvaluations;}
    std::string name(std::string aEvaluationString = "");
    std::string tag(std::string aEvaluationString = "");

    std::string loopVar(){return "E";}
    pugi::xml_node forNode(pugi::xml_node& aNode, std::string aXMLLoopVectorName);

};

}