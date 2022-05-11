/*
 * Operation.hpp
 *
 *  Created on: April 12, 2022
 */
#pragma once

#include <vector>
#include <string>
#include <memory>
#include <regex>

#include "pugixml.hpp"

#include "SharedData.hpp"
#include "Performer.hpp"

namespace PDir
{

struct InputOutput
{
    std::string mLayout;
    std::string mSize;
    std::shared_ptr<SharedData> mSharedData;
};

class Operation : public FileObject
{

protected:
    std::string mFunction;
    std::shared_ptr<Performer> mPerformer;
    bool mChDir;
    bool mOnChange;
    
    void appendCommonChildren(pugi::xml_node &aOperationNode,
                              std::string aEvaluationString);

public:
    Operation(const std::string& aName,
                          const std::string& aFunction,
                          std::shared_ptr<Performer> aPerformer,
                          int aConcurrentEvaluations);
    virtual void write_definition(pugi::xml_document& aDocument, 
                                  std::string aEvaluationString = "") = 0;
    virtual void write_interface(pugi::xml_node& aNode, 
                                 std::string aEvaluationString = "") = 0;

    pugi::xml_node forNode(pugi::xml_node& aNode, 
                           const std::string& aXMLLoopVectorName);
};

class OperationWait : public Operation
{
private:
    std::string mCommand; 
    
public:
    OperationWait(const std::string& aName,
                              const std::string& aFile,
                              std::shared_ptr<Performer> aPerformer,
                              int aConcurrentEvaluations);
    void write_definition(pugi::xml_document& aDocument, 
                          std::string aEvaluationString = "") override;
    void write_interface(pugi::xml_node& aNode, 
                         std::string aEvaluationString = "") override;
};

class OperationGemmaMPISystemCall : public Operation
{
private:
    std::string mCommand; 
    std::string mNumRanks;
    std::string mArgument;
    std::string mPath;
    
public:
    OperationGemmaMPISystemCall(const std::string& aInputDeck,
                                            const std::string& aPath,
                                            const std::string& aNumRanks, 
                                            std::shared_ptr<Performer> aPerformer,
                                            int aConcurrentEvaluations);
    void write_definition(pugi::xml_document& aDocument, 
                          std::string aEvaluationString = "") override;
    void write_interface(pugi::xml_node& aNode, 
                         std::string aEvaluationString = "") override;
};

class OperationAprepro : public Operation
{
private:
    std::string mCommand; 
    std::vector<std::string> mArgument;
    std::vector<std::string> mOptions;
    InputOutput mInput;

public:
    OperationAprepro(const std::string& aInputDeck,
                                 const std::vector<std::string>& aOptions, 
                                 std::shared_ptr<SharedData> aSharedData, 
                                 std::shared_ptr<Performer> aPerformer,
                                 int aConcurrentEvaluations);
    void write_definition(pugi::xml_document& aDocument, 
                          std::string aEvaluationString = "") override;
    void write_interface(pugi::xml_node& aNode, 
                         std::string aEvaluationString = "") override;
};

class OperationHarvestDataFunction : public Operation
{
private:
    std::string mFile;
    std::string mOperation;
    std::string mColumn;
    InputOutput mOutput;
public:
    OperationHarvestDataFunction(const std::string& aFile,
                                             const std::string& aMathOperation,
                                             const std::string& aDataColumn,
                                             std::shared_ptr<SharedData> aSharedData,
                                             std::shared_ptr<Performer> aPerformer,
                                             int aConcurrentEvaluations);
    void write_definition(pugi::xml_document& aDocument, 
                          std::string aEvaluationString = "") override;
    void write_interface(pugi::xml_node& aNode, 
                         std::string aEvaluationString = "") override;
};

class OperationCubitTet10Conversion : public Operation
{
private:
    std::string mCommand; 
    std::vector<std::string> mArgument;
    std::vector<std::string> mOptions;
    InputOutput mInput;

    void writeTet10ConversionFile(std::string aExodusFileName,
                                  std::vector<std::string> aBlockList);
public:
    OperationCubitTet10Conversion(const std::string& aExodusFileName,
                                              const std::vector<std::string>& aBlockList, 
                                              std::shared_ptr<SharedData> aSharedData, 
                                              std::shared_ptr<Performer> aPerformer,
                                              int aConcurrentEvaluations);
    void write_definition(pugi::xml_document& aDocument, 
                          std::string aEvaluationString = "") override;
    void write_interface(pugi::xml_node& aNode, 
                         std::string aEvaluationString = "") override;
};

class OperationCubitSubBlock : public Operation
{
private:
    std::string mCommand; 
    std::vector<std::string> mArgument;
    std::vector<std::string> mOptions;
    InputOutput mInput;

    void writeSubBlockFile(std::string aExodusFileName,
                           std::vector<std::string> aBoundingBox);
public:
    OperationCubitSubBlock(const std::string& aExodusFileName,
                                       const std::vector<std::string>& aBoundingBox, 
                                       std::shared_ptr<SharedData> aSharedData, 
                                       std::shared_ptr<Performer> aPerformer,
                                       int aConcurrentEvaluations);
    void write_definition(pugi::xml_document& aDocument, 
                          std::string aEvaluationString = "") override;
    void write_interface(pugi::xml_node& aNode, 
                         std::string aEvaluationString = "") override;
};

class OperationDecomp : public Operation
{
private:
    std::string mCommand; 
    std::vector<std::string> mArgument;
    
public:
    OperationDecomp(const std::string& aExodusFileName,
                                int aNumberOfPartitions, 
                                std::shared_ptr<Performer> aPerformer,
                                int aConcurrentEvaluations);
    void write_definition(pugi::xml_document& aDocument, 
                          std::string aEvaluationString = "") override;
    void write_interface(pugi::xml_node& aNode, 
                         std::string aEvaluationString = "") override;
};

}