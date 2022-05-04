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
    
    int evaluations(){return mConcurrentEvaluations;}
    std::string name(std::string aEvaluationString = "");
    std::string tag(std::string aEvaluationString = "");

    std::string loopVar(){return "E";}
    pugi::xml_node forNode(pugi::xml_node& aNode,
                           std::string aXMLLoopVectorName);
};

}