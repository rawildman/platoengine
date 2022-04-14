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
        std::string mSharedDataName;
    };

class XMLGeneratorOperation
{
    
protected:
    std::string mFunction;
    std::string mName;
    std::string mPerformerName;
    bool mChDir;
    bool mOnChange;
    int mConcurrentEvaluations;

    void appendCommonChildren(pugi::xml_node &aOperationNode);

public:
    XMLGeneratorOperation(const std::string& aName,
                          const std::string& aFunction);
    virtual void write(pugi::xml_document& aDocument) = 0;
    std::string name();
};

class XMLGeneratorOperationWait : public XMLGeneratorOperation
{
private:
    std::string mCommand; 
    std::string mEvaluation;
public:
    XMLGeneratorOperationWait(const std::string& aName,
                              const std::string& aFile,
                              const std::string& aEvaluation);
    void write(pugi::xml_document& aDocument) override;
};

class XMLGeneratorOperationGemmaMPISystemCall : public XMLGeneratorOperation
{
private:
    std::string mCommand; 
    std::string mNumRanks;
    std::string mArgument;
    std::string mEvaluation;
public:
    XMLGeneratorOperationGemmaMPISystemCall(const std::string& aInputDeck,
                                            const std::string& aNumRanks, 
                                            const std::string& aEvaluation);
    void write(pugi::xml_document& aDocument) override;

};

class XMLGeneratorOperationAprepro : public XMLGeneratorOperation
{
private:
    std::string mCommand; 
    std::vector<std::string> mArgument;
    std::vector<std::string> mOptions;
    std::string mEvaluation;
    XMLGeneratorInputOutput mInput;
public:
    XMLGeneratorOperationAprepro(const std::string& aInputDeck,
                                 const std::vector<std::string>& aOptions, 
                                 const std::string& aEvaluation);
    void write(pugi::xml_document& aDocument) override;

};

class XMLGeneratorOperationHarvestDataFunction : public XMLGeneratorOperation
{
private:
    std::string mFile;
    std::string mOperation;
    std::string mColumn;
    std::string mEvaluation;
    std::string mCriterion;
    XMLGeneratorInputOutput mOutput;
public:
    XMLGeneratorOperationHarvestDataFunction(const std::string& aFile,
                                             const std::string& aMathOperation,
                                             const std::string& aDataColumn,
                                             const std::string& aEvaluation,
                                             const  std::string& aCriterion);
    void write(pugi::xml_document& aDocument) override;
};

}