/*
 * XMLGeneratorOperation.hpp
 *
 *  Created on: April 12, 2022
 */
#pragma once

#include <vector>
#include <string>
#include <memory>
#include <regex>

#include "pugixml.hpp"

#include "XMLGeneratorSharedData.hpp"
#include "XMLGeneratorPerformer.hpp"

namespace XMLGen
{

    struct XMLGeneratorInputOutput
    {
        std::string mLayout;
        std::string mSize;
        std::shared_ptr<XMLGeneratorSharedData> mSharedData;
    };

class XMLGeneratorOperation : public XMLGeneratorFileObject
{

protected:
    std::string mFunction;
    std::shared_ptr<XMLGeneratorPerformer> mPerformer;
    bool mChDir;
    bool mOnChange;
    
    void appendCommonChildren(pugi::xml_node &aOperationNode,
                              std::string aEvaluationString);

public:
    XMLGeneratorOperation(const std::string& aName,
                          const std::string& aFunction,
                          std::shared_ptr<XMLGeneratorPerformer> aPerformer,
                          int aConcurrentEvaluations);
    virtual void write_definition(pugi::xml_document& aDocument, 
                                  std::string aEvaluationString = "") = 0;
    virtual void write_interface(pugi::xml_node& aNode, 
                                 std::string aEvaluationString = "") = 0;
};

class XMLGeneratorOperationWait : public XMLGeneratorOperation
{
private:
    std::string mCommand; 
    
public:
    XMLGeneratorOperationWait(const std::string& aName,
                              const std::string& aFile,
                              std::shared_ptr<XMLGeneratorPerformer> aPerformer,
                              int aConcurrentEvaluations);
    void write_definition(pugi::xml_document& aDocument, 
                          std::string aEvaluationString = "") override;
    void write_interface(pugi::xml_node& aNode, 
                         std::string aEvaluationString = "") override;
};

class XMLGeneratorOperationGemmaMPISystemCall : public XMLGeneratorOperation
{
private:
    std::string mCommand; 
    std::string mNumRanks;
    std::string mArgument;
    
public:
    XMLGeneratorOperationGemmaMPISystemCall(const std::string& aInputDeck,
                                            const std::string& aNumRanks, 
                                            std::shared_ptr<XMLGeneratorPerformer> aPerformer,
                                            int aConcurrentEvaluations);
    void write_definition(pugi::xml_document& aDocument, 
                          std::string aEvaluationString = "") override;
    void write_interface(pugi::xml_node& aNode, 
                         std::string aEvaluationString = "") override;
};

class XMLGeneratorOperationAprepro : public XMLGeneratorOperation
{
private:
    std::string mCommand; 
    std::vector<std::string> mArgument;
    std::vector<std::string> mOptions;
    XMLGeneratorInputOutput mInput;

public:
    XMLGeneratorOperationAprepro(const std::string& aInputDeck,
                                 const std::vector<std::string>& aOptions, 
                                 std::shared_ptr<XMLGeneratorSharedData> aSharedData, 
                                 std::shared_ptr<XMLGeneratorPerformer> aPerformer,
                                 int aConcurrentEvaluations);
    void write_definition(pugi::xml_document& aDocument, 
                          std::string aEvaluationString = "") override;
    void write_interface(pugi::xml_node& aNode, 
                         std::string aEvaluationString = "") override;
};

class XMLGeneratorOperationHarvestDataFunction : public XMLGeneratorOperation
{
private:
    std::string mFile;
    std::string mOperation;
    std::string mColumn;
    XMLGeneratorInputOutput mOutput;
public:
    XMLGeneratorOperationHarvestDataFunction(const std::string& aFile,
                                             const std::string& aMathOperation,
                                             const std::string& aDataColumn,
                                             std::shared_ptr<XMLGeneratorSharedData> aSharedData,
                                             std::shared_ptr<XMLGeneratorPerformer> aPerformer,
                                             int aConcurrentEvaluations);
    void write_definition(pugi::xml_document& aDocument, 
                          std::string aEvaluationString = "") override;
    void write_interface(pugi::xml_node& aNode, 
                         std::string aEvaluationString = "") override;
};

class XMLGeneratorOperationCubitTet10Conversion : public XMLGeneratorOperation
{
private:
    std::string mCommand; 
    std::vector<std::string> mArgument;
    std::vector<std::string> mOptions;
    XMLGeneratorInputOutput mInput;

    void writeTet10ConversionFile(std::string aExodusFileName,
                                    std::vector<std::string> aBlockList);
public:
    XMLGeneratorOperationCubitTet10Conversion(const std::string& aExodusFileName,
                                 const std::vector<std::string>& aBlockList, 
                                 std::shared_ptr<XMLGeneratorSharedData> aSharedData, 
                                 std::shared_ptr<XMLGeneratorPerformer> aPerformer,
                                 int aConcurrentEvaluations);
    void write_definition(pugi::xml_document& aDocument, 
                          std::string aEvaluationString = "") override;
    void write_interface(pugi::xml_node& aNode, 
                         std::string aEvaluationString = "") override;
};

class XMLGeneratorOperationCubitSubBlock : public XMLGeneratorOperation
{
private:
    std::string mCommand; 
    std::vector<std::string> mArgument;
    std::vector<std::string> mOptions;
    XMLGeneratorInputOutput mInput;

    void writeSubBlockFile(std::string aExodusFileName,
                                    std::vector<std::string> aBoundingBox);
public:
    XMLGeneratorOperationCubitSubBlock(const std::string& aExodusFileName,
                                 const std::vector<std::string>& aBoundingBox, 
                                 std::shared_ptr<XMLGeneratorSharedData> aSharedData, 
                                 std::shared_ptr<XMLGeneratorPerformer> aPerformer,
                                 int aConcurrentEvaluations);
    void write_definition(pugi::xml_document& aDocument, 
                          std::string aEvaluationString = "") override;
    void write_interface(pugi::xml_node& aNode, 
                         std::string aEvaluationString = "") override;
};


class XMLGeneratorOperationDecomp : public XMLGeneratorOperation
{
private:
    std::string mCommand; 
    std::vector<std::string> mArgument;
    
public:
    XMLGeneratorOperationDecomp(const std::string& aExodusFileName,
                                 int aNumberOfPartitions, 
                                 std::shared_ptr<XMLGeneratorPerformer> aPerformer,
                                 int aConcurrentEvaluations);
    void write_definition(pugi::xml_document& aDocument, 
                          std::string aEvaluationString = "") override;
    void write_interface(pugi::xml_node& aNode, 
                         std::string aEvaluationString = "") override;
};


}