/*
 * XMLGeneratorOperationsFileUtilities.cpp
 *
 *  Created on: March 21, 2022
 */

#include "pugixml.hpp"

#include "XMLGeneratorUtilities.hpp"
#include "XMLGeneratorDataStruct.hpp"
#include "XMLGeneratorOperationsFileUtilities.hpp"
#include "XMLGeneratorOperationsDataStructures.hpp"

namespace XMLGen
{

void append_evaluation_subdirectory_names
(const std::string& aSubDirBaseName, 
 XMLGen::OperationMetaData& aOperationMetaData)
{
    auto tConcurrentEvals = aOperationMetaData.is_defined("concurrent_evaluations") && !aOperationMetaData.front("concurrent_evaluations").empty() 
        ? aOperationMetaData.front("concurrent_evaluations") : "1";
    auto tNumConcurrentEvals = std::stoi(tConcurrentEvals);
    for(decltype(tNumConcurrentEvals) tEvalIndex = 0; tEvalIndex < tNumConcurrentEvals; tEvalIndex++)
    {
        auto tDirectoryName = aSubDirBaseName + "_" + std::to_string(tEvalIndex) + std::string("/");
        aOperationMetaData.append("subdirectories", tDirectoryName);
    }
}
// function append_evaluation_subdirectory_names

void append_concurrent_evaluation_index_to_option
(const std::string& aKey,
 const std::string& aValue,
 XMLGen::OperationMetaData& aOperationMetaData)
{
    auto tNumConcurrentEvals = std::stoi(aOperationMetaData.get("concurrent_evaluations").front());
    for(decltype(tNumConcurrentEvals) tIndex = 0; tIndex < tNumConcurrentEvals; tIndex++)
    {
        auto tStrIndex = std::to_string(tIndex);
        auto tConcatanetedValue = aValue + tStrIndex;
        aOperationMetaData.append(aKey, tConcatanetedValue);
    }    
}
// fuction append_concurrent_evaluation_index_to_option

void set_aprepro_system_call_options
(XMLGen::OperationMetaData& aOperationMetaData)
{
    auto tNumConcurrentEvals = std::stoi(aOperationMetaData.get("concurrent_evaluations").front());
    aOperationMetaData.set("functions", std::vector<std::string>(tNumConcurrentEvals, "SystemCall"));
    aOperationMetaData.set("commands", std::vector<std::string>(tNumConcurrentEvals, "aprepro"));
    XMLGen::append_concurrent_evaluation_index_to_option("names", "aprepro_", aOperationMetaData); 

    auto tDescriptors = aOperationMetaData.get("descriptors");
    aOperationMetaData.set("options", tDescriptors);
    auto tInputFileName = aOperationMetaData.get("input_file").front();
    auto tTemplateFileName = aOperationMetaData.get("template_file").front();
    std::vector<std::string> tArguments = {"-q", tTemplateFileName, tInputFileName};
    aOperationMetaData.set("arguments", tArguments);
}
// function set_aprepro_system_call_options

void append_shared_data_argument_to_operation
(const XMLGen::OperationArgument& aArgument,
 pugi::xml_node& aOperation)
{
    auto tLowerType = XMLGen::to_lower(aArgument.get("type"));
    if( !(tLowerType == "input") && !(tLowerType == "output") )
    {
        THROWERR(std::string("OperationArgument of type '") + tLowerType + "' is not supported. Supported types are 'input' and 'output'.")
    }
    auto tDataType = tLowerType == "input" ? "Input" : "Output";
    auto tInputNode = aOperation.append_child(tDataType);
    XMLGen::append_children({"ArgumentName", "Layout", "Size"}, {aArgument.get("name"), aArgument.get("layout"), aArgument.get("size")}, tInputNode);
}
// function append_shared_data_argument_to_operation

}
// namespace XMLGen