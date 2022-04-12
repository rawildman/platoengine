/*
 * XMLGeneratorOperation.hpp
 *
 *  Created on: April 12, 2022
 */
#pragma once

#include <vector>
#include <string>

#include "pugixml.hpp"

namespace XMLGen
{

    struct XMLGeneratorInputOutput
    {
        std::string mArgumentName;
        std::string mLayout;
        std::string mSize;
    };

class XMLGeneratorOperation
{
    
protected:
    std::string mFunction;
    std::string mName;
    bool mChDir;
    std::string mCommand; 
    bool mOnChange;

    void appendCommonChildren(pugi::xml_node &aOperationNode);

public:
    XMLGeneratorOperation();
    void write(pugi::xml_document& aDocument);
};

class XMLGeneratorOperationWait : public XMLGeneratorOperation
{
private:
    std::string mEvaluation;
public:
    XMLGeneratorOperationWait
    (std::string aName,
    std::string aFile,
    std::string aEvaluation);
    void write(pugi::xml_document& aDocument);
};


class XMLGeneratorOperationGemmaMPISystemCall : public XMLGeneratorOperation
{
private:
    std::string mNumRanks;
    std::string mArgument;
    std::string mEvaluation;
public:
    XMLGeneratorOperationGemmaMPISystemCall(std::string aInputDeck, std::string aNumRanks, std::string aEvaluation);
    void write(pugi::xml_document& aDocument);

};

class XMLGeneratorOperationAprepro : public XMLGeneratorOperation
{
private:
    std::vector<std::string> mArgument;
    std::vector<std::string> mOptions;
    std::string mEvaluation;
    XMLGeneratorInputOutput mInput;
public:
    XMLGeneratorOperationAprepro(std::string aInputDeck, std::vector<std::string> aOptions , std::string aEvaluation);
    void write(pugi::xml_document& aDocument);

};



/*
class XMLGeneratorOperationHarvestDataFunction : public XMLGeneratorOperation
{
private:
    int mColumn;
    std::string mOperation;
    bool mAppendInput;
    XMLGeneratorInputOutput mPut;
public:
    XMLGeneratorOperationHarvestDataFunction(const XMLGen::InputData& aInputMetaData, int aCriteria, int aEvaluation);
    void write(pugi::xml_document& aDocument);
};

class XMLGeneratorOperationAprepro : public XMLGeneratorOperation
{
private:
    std::vector<std::string> mArguments;
    std::vector<std::string> mOptions;
public:
    XMLGeneratorOperationAprepro(const XMLGen::InputData& aInputMetaData, int aCriteria, int aEvaluation);
    void write(pugi::xml_document& aDocument);
};

*/
}