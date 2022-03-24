/*
 * XMLGeneratorOperationsFileUtilities.cpp
 *
 *  Created on: March 21, 2022
 */

#include "pugixml.hpp"

#include "XMLGeneratorUtilities.hpp"
#include "XMLGeneratorDataStruct.hpp"
#include "XMLGeneratorGemmaUtilities.hpp"
#include "XMLGeneratorServiceUtilities.hpp"
#include "XMLGeneratorOperationsFileUtilities.hpp"
#include "XMLGeneratorOperationsDataStructures.hpp"

namespace XMLGen
{

void append_evaluation_subdirectories
(XMLGen::OperationMetaData& aOperationMetaData)
{
    auto tConcurrentEvals = aOperationMetaData.is_defined("concurrent_evaluations") && !aOperationMetaData.front("concurrent_evaluations").empty() 
        ? aOperationMetaData.front("concurrent_evaluations") : "1";
    auto tNumConcurrentEvals = std::stoi(tConcurrentEvals);

    auto tBaseSubDirName = aOperationMetaData.front("base_subdirectory_name");
    for(decltype(tNumConcurrentEvals) tEvalIndex = 0; tEvalIndex < tNumConcurrentEvals; tEvalIndex++)
    {
        auto tDirectoryName = tBaseSubDirName + "_" + std::to_string(tEvalIndex) + std::string("/");
        aOperationMetaData.append("subdirectories", tDirectoryName);
    }
}
// function append_evaluation_subdirectories

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

void write_aprepro_system_call_operation
(const std::vector<XMLGen::OperationArgument>& aInputs,
 const XMLGen::OperationMetaData& aOperationMetaData,
 pugi::xml_document& aDocument)
{
    auto tOptions = aOperationMetaData.get("options");
    auto tArguments = aOperationMetaData.get("arguments");
    auto tAppendInput = aInputs.empty() ? "false" : "true";

    auto tNumConcurrentEvals = std::stoi(aOperationMetaData.get("concurrent_evaluations").front());
    for(decltype(tNumConcurrentEvals) tEvalIndex = 0; tEvalIndex < tNumConcurrentEvals; tEvalIndex++)
    {
        auto tSuffix = std::string("_") + std::to_string(tEvalIndex);
        auto tOperation = aDocument.append_child("Operation");

        auto tFuncName = std::string("aprepro") + tSuffix;
        std::vector<std::string> tKeys = {"Function", "Name", "Command", "ChDir", "OnChange", "AppendInput"};
        std::vector<std::string> tVals = {aOperationMetaData.get("functions")[tEvalIndex], 
                                          aOperationMetaData.get("names")[tEvalIndex], 
                                          aOperationMetaData.get("commands")[tEvalIndex], 
                                          aOperationMetaData.get("subdirectories")[tEvalIndex], 
                                          "true", tAppendInput};
        XMLGen::append_children(tKeys, tVals, tOperation);
        XMLGen::append_children(std::vector<std::string>(tArguments.size(), "Argument"), tArguments, tOperation);
        XMLGen::append_children(std::vector<std::string>(tOptions.size(), "Option"), tOptions, tOperation);
        XMLGen::append_shared_data_argument_to_operation(aInputs[tEvalIndex], tOperation);
    }
}
// function write_aprepro_system_call_operation

std::string set_descriptor_value
(const std::string& aTargetDescriptor,
 const std::string& aTargetDescriptorValue,
 const std::vector<std::string>& aDescriptors)
{
    auto tOutput = std::find(aDescriptors.begin(), aDescriptors.end(), aTargetDescriptor) == aDescriptors.end() ? aTargetDescriptorValue : std::string("{") + aTargetDescriptor + "}";
    return tOutput;
}
// function set_descriptor_value

void set_operation_option_from_service_metadata
(const std::string& aTargetKey,
 const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData)
{
    auto tLowerTargetKey = XMLGen::to_lower(aTargetKey);
    auto tLowerTargetCode = XMLGen::to_lower(aOperationMetaData.back("code"));
    auto tValues = XMLGen::get_values_from_service_metadata(tLowerTargetCode, aTargetKey, aInputMetaData);
    auto tNumConcurrentEvals = std::stoi(aOperationMetaData.front("concurrent_evaluations"));
    if( tNumConcurrentEvals > tValues.size() && tValues.size() == 1u )
    {
        aOperationMetaData.set( tLowerTargetKey, std::vector<std::string>(tNumConcurrentEvals, tValues.front()) );
    }
    else if( tNumConcurrentEvals == tValues.size() )
    {
        aOperationMetaData.set( tLowerTargetKey, tValues );
    }
    else
    {
        THROWERR(std::string("The number of services with code name '") + tLowerTargetCode 
            + "' should be equal to one or match the number of concurrent evaluations, which is set to '" 
            + std::to_string(tNumConcurrentEvals) + "'.")
    }
}
// function set_operation_option_from_service_metadata

void check_app_service_type(const std::string& aType)
{
    auto tLowerType = XMLGen::to_lower(aType);
    if( tLowerType == "plato_app" )
    {
        THROWERR("The 'run system call' function cannot be used with service of type 'plato_app'. The 'system call' function can only be used with services of type 'web_app' and 'system_call'.")
    }
}
// function check_app_service_type

void append_operation_options_based_on_app_type
(const std::string& aKey,
 const std::unordered_map<std::string, std::string>& aMap,
 XMLGen::OperationMetaData& aOperationMetaData)
{
    for(auto& tType : aOperationMetaData.get("type"))
    {
        XMLGen::check_app_service_type(tType);
        auto tItr = aMap.find(tType);
        if(tItr == aMap.end())
        {
            THROWERR("Did not find key '" + tType + "' in associative map.")
        }
        aOperationMetaData.append(aKey, tItr->second);
    }
}
// function append_operation_options_based_on_app_type

}
// namespace XMLGen