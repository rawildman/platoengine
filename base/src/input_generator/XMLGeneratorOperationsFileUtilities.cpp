/*
 * XMLGeneratorOperationsFileUtilities.cpp
 *
 *  Created on: March 21, 2022
 */

#include "XMLGeneratorUtilities.hpp"
#include "XMLGeneratorDataStruct.hpp"
#include "XMLGeneratorGemmaUtilities.hpp"
#include "XMLGeneratorFlaskUtilities.hpp"
#include "XMLGeneratorServiceUtilities.hpp"
#include "XMLGeneratorCriteriaUtilities.hpp"
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

void set_general_run_app_via_system_call_options
(const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData)
{
    auto tNumConcurrentEvals = aInputMetaData.optimization_parameters().concurrent_evaluations();
    aOperationMetaData.set("concurrent_evaluations", {tNumConcurrentEvals});
    XMLGen::set_operation_option_from_service_metadata("type", aInputMetaData, aOperationMetaData);
    XMLGen::set_operation_option_from_service_metadata("number_processors", aInputMetaData, aOperationMetaData);
}
// function set_general_run_app_via_system_call_options

size_t get_web_port_number
(const size_t aEvalIndex,
 const XMLGen::OperationMetaData& aOperationMetaData,
 const size_t& tDefaultBaseWebPortNum)
{
    auto tBaseWebPortNumber = aOperationMetaData.is_defined("base_web_port_number") ? std::stoi(aOperationMetaData.front("base_web_port_number")) : tDefaultBaseWebPortNum;
    auto tWebPortNumber = tBaseWebPortNumber + aEvalIndex;
    return tWebPortNumber;
}
// function get_web_port_number

void append_run_system_call_shared_data_arguments
(XMLGen::OperationMetaData& aOperationMetaData)
{
    XMLGen::gemma::Files tFileFormats; 
    for(auto& tType : aOperationMetaData.get("type"))
    {
        XMLGen::check_app_service_type(tType);
        if(tType == "system_call")
        {
            auto tInputFileName = tFileFormats.filename(aOperationMetaData.front("algorithm"), "input");
            aOperationMetaData.append("arguments", tInputFileName);
        }
        else
        {
            auto tConcurrentEvalIndex = &tType - &aOperationMetaData.get("type")[0];
            auto tWebPortNum = XMLGen::get_web_port_number(tConcurrentEvalIndex, aOperationMetaData);
            auto tArgument = std::string("http://localhost:") + std::to_string(tWebPortNum) + "/run_" 
                + aOperationMetaData.get("executables")[tConcurrentEvalIndex] + "/";
            aOperationMetaData.append("arguments", tArgument);
        }
    }
}
// function append_run_system_call_shared_data_arguments

void write_run_app_via_system_call_operation_to_file
(const XMLGen::OperationMetaData& aOperationMetaData,
 pugi::xml_document& aDocument)
{
    auto tNumConcurrentEvals = std::stoi(aOperationMetaData.front("concurrent_evaluations"));
    for(decltype(tNumConcurrentEvals) tIndex = 0; tIndex < tNumConcurrentEvals; tIndex++)
    {
        auto tSuffix = std::string("_") + std::to_string(tIndex);
        auto tOperation = aDocument.append_child("Operation");

        auto tFuncName = std::string("aprepro") + tSuffix;
        std::vector<std::string> tKeys = {"Function", "Name", "Command", "OnChange", "AppendInput", "ChDir", "Argument"};
        std::vector<std::string> tVals = {aOperationMetaData.get("functions")[tIndex], 
                                          aOperationMetaData.get("names")[tIndex], 
                                          aOperationMetaData.get("commands")[tIndex], 
                                          "true", 
                                          
                                          "false",
                                          aOperationMetaData.get("subdirectories")[tIndex],
                                          aOperationMetaData.get("arguments")[tIndex]};
        XMLGen::append_children(tKeys, tVals, tOperation);
    }
}
// write_run_app_via_system_call_operation_to_file

std::string get_data_group_type(const std::string& aDataGroup)
{
    auto tDataGroupType = XMLGen::to_lower( aDataGroup.substr(0, aDataGroup.find_first_of("_")) );
    tDataGroupType = tDataGroupType == "column" ? "Column" : "Row";
    return tDataGroupType;
}
// function get_data_group_type

std::string get_data_group_index(const std::string& aDataGroup)
{
    auto tDataGroupIndex = aDataGroup.substr(aDataGroup.find_first_of("_")+1);
    return tDataGroupIndex;
}
// function get_data_group_index

void set_file_names
(const std::string& aKey, 
 XMLGen::OperationMetaData& aOperationMetaData)
{
    if( !aOperationMetaData.is_defined(aKey) )
    {
        return;
    }

    std::vector<std::string> tFileNames;
    auto tProposedAppFileName = aOperationMetaData.front("base_output_file");
    for(auto& tFileName : aOperationMetaData.get(aKey))
    {
        for(auto& tDirectoryName : aOperationMetaData.get("subdirectories"))
        {
            auto tProposedAppFilePath = std::string("./") + tDirectoryName + tProposedAppFileName;
            auto tMyFileName = tFileName.empty() ? tProposedAppFilePath : tFileName;
            tFileNames.push_back(tMyFileName);
        }
    }
    aOperationMetaData.set(aKey, tFileNames);
}
// function set_file_names

void write_harvest_objective_data_from_file_operation
(const std::vector<XMLGen::OperationArgument>& aOutputs,
 const XMLGen::OperationMetaData& aOperationMetaData,
 pugi::xml_document& aDocument)
{
    if(!aOperationMetaData.is_defined("objective_data_files"))
    {
        return;
    }
    auto tNumCriteria = aOperationMetaData.get("objective_criteria_ids").size();
    auto tNumConcurrentEvals = std::stoi(aOperationMetaData.front("concurrent_evaluations"));
    for(decltype(tNumCriteria) tCriteriaIndex = 0; tCriteriaIndex < tNumCriteria; tCriteriaIndex++)
    {
        auto tObjectiveCriterionID = aOperationMetaData.get("objective_criteria_ids")[tCriteriaIndex];
        for(decltype(tNumConcurrentEvals) tEvalIndex = 0; tEvalIndex < tNumConcurrentEvals; tEvalIndex++)
        {
            auto tDataGroupType = XMLGen::get_data_group_type(aOperationMetaData.get("objective_data_groups")[tCriteriaIndex]);
            std::vector<std::string> tKeys = {"Function", "Name", "File", "Operation", tDataGroupType};

            auto tFileIndex = (tNumCriteria * tCriteriaIndex) + tEvalIndex;
            auto tDataGroupIndex = XMLGen::get_data_group_index(aOperationMetaData.get("objective_data_groups")[tCriteriaIndex]);
            auto tFuncName = std::string("harvest_objective_criterion_id_") + tObjectiveCriterionID + "_eval_" + std::to_string(tEvalIndex);
            std::vector<std::string> tVals = { "HarvestDataFromFile", tFuncName, 
                                               aOperationMetaData.get("objective_data_files")[tFileIndex],
                                               aOperationMetaData.get("objective_data_extraction_operations")[tCriteriaIndex],
                                               tDataGroupIndex };

            auto tOperation = aDocument.append_child("Operation");
            XMLGen::append_children(tKeys, tVals, tOperation);
            XMLGen::append_shared_data_argument_to_operation(aOutputs[tEvalIndex], tOperation);
        }
    }
}
// function write_harvest_objective_data_from_file_operation

void write_harvest_constraint_data_from_file_operation
(const std::vector<XMLGen::OperationArgument>& aOutputs,
 const XMLGen::OperationMetaData& aOperationMetaData,
 pugi::xml_document& aDocument)
{
    if(!aOperationMetaData.is_defined("constraint_data_files"))
    {
        return;
    }
    auto tNumCriteria = aOperationMetaData.get("constraint_criteria_ids").size();
    auto tNumConcurrentEvals = std::stoi(aOperationMetaData.front("concurrent_evaluations"));
    for(decltype(tNumCriteria) tCriteriaIndex = 0; tCriteriaIndex < tNumCriteria; tCriteriaIndex++)
    {
        auto tConstraintCriterionID = aOperationMetaData.get("constraint_criteria_ids")[tCriteriaIndex];
        for(decltype(tNumConcurrentEvals) tEvalIndex = 0; tEvalIndex < tNumConcurrentEvals; tEvalIndex++)
        {
            auto tDataGroupType = XMLGen::get_data_group_type(aOperationMetaData.get("constraint_data_groups")[tCriteriaIndex]);
            std::vector<std::string> tKeys = {"Function", "Name", "File", "Operation", tDataGroupType};
            
            auto tFileIndex = (tNumCriteria * tCriteriaIndex) + tEvalIndex;
            auto tDataGroupIndex = XMLGen::get_data_group_index(aOperationMetaData.get("constraint_data_groups")[tCriteriaIndex]);
            auto tFuncName = std::string("harvest_constraint_criterion_id_") + tConstraintCriterionID + "_eval_" + std::to_string(tEvalIndex);
            std::vector<std::string> tVals = { "HarvestDataFromFile", tFuncName, 
                                               aOperationMetaData.get("constraint_data_files")[tFileIndex],
                                               aOperationMetaData.get("constraint_data_extraction_operations")[tCriteriaIndex],
                                               tDataGroupIndex };

            auto tOperation = aDocument.append_child("Operation");
            XMLGen::append_children(tKeys, tVals, tOperation);
            XMLGen::append_shared_data_argument_to_operation(aOutputs[tEvalIndex], tOperation);
        }
    }
}
// function write_harvest_constraint_data_from_file_operation

void append_system_call_constraint_criteria_options
(const std::vector<std::string>& aCriteriaIDs,
 const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData) 
{
   for(auto tConstraint : aInputMetaData.constraints)
   {
       auto tItr = std::find(aCriteriaIDs.begin(), aCriteriaIDs.end(), tConstraint.criterion());
       if(tItr != aCriteriaIDs.end())
       {
           auto tCriterionID = tConstraint.criterion();
           auto tCriterion = aInputMetaData.criterion(tCriterionID);
           aOperationMetaData.append("constraint_criteria_ids", tCriterionID);
           aOperationMetaData.append("constraint_data_files", tCriterion.value("data_file"));
           aOperationMetaData.append("constraint_data_groups", tCriterion.value("data_group"));
           aOperationMetaData.append("constraint_data_extraction_operations", tCriterion.value("data_extraction_operation"));
       }
   }
}
// function append_system_call_constraint_criteria_options

void append_system_call_objective_criteria_options
(const std::vector<std::string>& aCriteriaIDs,
 const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData)
{
    for(auto tCriterionID : aInputMetaData.objective.criteriaIDs)
    {
        auto tItr = std::find(aCriteriaIDs.begin(), aCriteriaIDs.end(), tCriterionID);
        if(tItr != aCriteriaIDs.end())
        {
            auto tCriterion = aInputMetaData.criterion(tCriterionID);
            aOperationMetaData.append("objective_criteria_ids", tCriterionID);
            aOperationMetaData.append("objective_data_files", tCriterion.value("data_file"));
            aOperationMetaData.append("objective_data_groups", tCriterion.value("data_group"));
            aOperationMetaData.append("objective_data_extraction_operations", tCriterion.value("data_extraction_operation"));
        }
    }
}
// function append_system_call_objective_criteria_options

void append_general_harvest_data_from_file_operation_options
(const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData)
{
    auto tNumConcurrentEvals = aInputMetaData.optimization_parameters().concurrent_evaluations();
    aOperationMetaData.append("concurrent_evaluations", tNumConcurrentEvals);
    XMLGen::append_concurrent_evaluation_index_to_option("names", "harvest_data_", aOperationMetaData);
}
// function append_general_harvest_data_from_file_operation_options

std::vector<XMLGen::OperationArgument>
set_input_args_metadata_for_aprepro_operation
(const XMLGen::OperationMetaData& aOperationMetaData)
{
    auto tSize = aOperationMetaData.get("descriptors").size();
    if(tSize <= 0)
    {
        THROWERR("Number of parameters can not be less or equal than zero.")
    }

    std::vector<XMLGen::OperationArgument> tSharedDataArgs;
    auto tNumConcurrentEvals = std::stoi(aOperationMetaData.front("concurrent_evaluations"));
    for(decltype(tNumConcurrentEvals) tIndex = 0; tIndex < tNumConcurrentEvals; tIndex++)
    {
        XMLGen::OperationArgument tSharedDataArgument;
        auto tName = std::string("parameters_") + std::to_string(tIndex);
        tSharedDataArgument.set("name", tName);
        tSharedDataArgument.set("type", "input");
        tSharedDataArgument.set("layout", "scalar");
        auto tSrSize = std::to_string(tSize);
        tSharedDataArgument.set("size", tSrSize);
        tSharedDataArgs.push_back(tSharedDataArgument);
    }
    return tSharedDataArgs;
}
// function set_input_args_metadata_for_aprepro_operation

void are_aprepro_input_options_defined
(const XMLGen::OperationMetaData& aOperationMetaData)
{
    std::vector<std::string> tValidKeys = {"concurrent_evaluations", "names", "options", "arguments", "commands", "functions"};
    for(auto& tValidKey : tValidKeys)
    {
        if(!aOperationMetaData.is_defined(tValidKey))
        {
            auto tDefinedKeys = aOperationMetaData.keys();
            auto tMsg = std::string("Did not find aprepro system call option '") + tValidKey + "' for matched power balance use case. " 
                + "The following keys are defined in the aprepro operation metadata cabinet: \n";
            for(auto tKey : tDefinedKeys)
            {
                tMsg = tMsg + "  " + tKey + "\n";
            }
            THROWERR(tMsg)
        }
    }
}
// function are_aprepro_input_options_defined

std::vector<XMLGen::OperationArgument>
get_criterion_value_argument_metadata
(const std::string& aCriterionType,
 const XMLGen::OperationMetaData& aOperationMetaData)
{
    auto tLowerCriterionType = XMLGen::to_lower(aCriterionType);
    auto tGetFuncArgument = tLowerCriterionType == "objective" ? "objective_criteria_ids" : "constraint_criteria_ids";
    if(!aOperationMetaData.is_defined(tGetFuncArgument))
    {
        return std::vector<XMLGen::OperationArgument>();
    }

    std::vector<XMLGen::OperationArgument> tSharedDataArguments;
    auto tNumCriteria = aOperationMetaData.get(tGetFuncArgument).size();
    auto tNumConcurrentEvals = std::stoi(aOperationMetaData.front("concurrent_evaluations"));
    for(decltype(tNumCriteria) tCriteriaIndex = 0; tCriteriaIndex < tNumCriteria; tCriteriaIndex++)
    {
        auto tCriterionID = aOperationMetaData.get(tGetFuncArgument)[tCriteriaIndex];
        for(decltype(tNumConcurrentEvals) tIndex = 0; tIndex < tNumConcurrentEvals; tIndex++)
        {
            XMLGen::OperationArgument tSharedDataArgument;
            auto tArgumentName = tLowerCriterionType + " value for criterion " + tCriterionID 
                + std::string(" and evaluation ") + std::to_string(tIndex);
            tSharedDataArgument.set("name", tArgumentName);
            tSharedDataArgument.set("type", "output");
            tSharedDataArgument.set("layout", "scalar");
            tSharedDataArgument.set("size", "1");
            tSharedDataArguments.push_back(tSharedDataArgument);
        }
    }

    return tSharedDataArguments;
}
// function get_criterion_value_argument_metadata

void append_web_port_numbers(XMLGen::OperationMetaData& aOperationMetaData)
{
    auto tNumConcurrentEvals = std::stoi(aOperationMetaData.front("concurrent_evaluations"));
    for(decltype(tNumConcurrentEvals) tIndex = 0; tIndex < tNumConcurrentEvals; tIndex++)
    {
        auto tWebPortNum = std::to_string(XMLGen::get_web_port_number(tIndex, aOperationMetaData));
        aOperationMetaData.append("web_port_numbers", tWebPortNum);
    }
}
// function append_web_port_numbers

void append_app_options
(const XMLGen::AppOptions& aAppOptions,
 XMLGen::OperationMetaData& aOperationMetaData)
{
    auto tOuterTags = aAppOptions.otags();
    for(auto& tTag : tOuterTags)
    {
        XMLGen::append_operation_options_based_on_app_type(tTag, aAppOptions.get(tTag), aOperationMetaData);
    }
}
// function append_app_options

void append_general_aprepro_operation_options
(const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData)
{
    auto tNumConcurrentEvals = aInputMetaData.optimization_parameters().concurrent_evaluations();
    aOperationMetaData.append("concurrent_evaluations", tNumConcurrentEvals);
    auto tDescriptors = aInputMetaData.optimization_parameters().descriptors();
    aOperationMetaData.set("descriptors", tDescriptors);
    auto tSize = tDescriptors.size();
    if(tSize <= 0)
    {
        THROWERR("Number of parameters can not be less or equal than zero.")
    }
}
// function append_general_aprepro_operation_options

void write_aprepro_operation
(const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData aOperationMetaData,
 pugi::xml_document& tDocument)
{
    XMLGen::append_general_aprepro_operation_options(aInputMetaData, aOperationMetaData);
    XMLGen::append_evaluation_subdirectories(aOperationMetaData);
    XMLGen::set_aprepro_system_call_options(aOperationMetaData);
    auto tSharedDataArgs = XMLGen::set_input_args_metadata_for_aprepro_operation(aOperationMetaData);

    XMLGen::are_aprepro_input_options_defined(aOperationMetaData);
    XMLGen::write_aprepro_system_call_operation(tSharedDataArgs, aOperationMetaData, tDocument);
}
// function write_aprepro_operation

void write_run_app_via_system_call_operation
(const XMLGen::InputData& aInputMetaData,
 const XMLGen::AppOptions& aAppOptions,
 XMLGen::OperationMetaData& aOperationMetaData,
 pugi::xml_document& aDocument)
{
    XMLGen::set_general_run_app_via_system_call_options(aInputMetaData, aOperationMetaData);
    XMLGen::append_app_options(aAppOptions, aOperationMetaData);
    XMLGen::append_evaluation_subdirectories(aOperationMetaData);
    auto tCodeNamePlusDelimiter = aOperationMetaData.front("code") + "_";
    XMLGen::append_concurrent_evaluation_index_to_option("names", tCodeNamePlusDelimiter, aOperationMetaData);
    XMLGen::append_run_system_call_shared_data_arguments(aOperationMetaData);
    XMLGen::write_run_app_via_system_call_operation_to_file(aOperationMetaData, aDocument);
}
// function write_run_app_via_system_call_operation

void write_harvest_data_from_file_operation
(const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData,
 pugi::xml_document& aDocument)
{
    XMLGen::append_general_harvest_data_from_file_operation_options(aInputMetaData, aOperationMetaData);
    XMLGen::append_evaluation_subdirectories(aOperationMetaData);

    auto tCriteriaIDs = XMLGen::get_criteria_ids("system_call", aInputMetaData);
    XMLGen::append_system_call_objective_criteria_options(tCriteriaIDs, aInputMetaData, aOperationMetaData);
    XMLGen::append_system_call_constraint_criteria_options(tCriteriaIDs, aInputMetaData, aOperationMetaData);

    XMLGen::set_file_names("objective_data_files", aOperationMetaData);
    auto tObjectiveSharedDataArgs = XMLGen::get_criterion_value_argument_metadata("objective", aOperationMetaData);
    XMLGen::write_harvest_objective_data_from_file_operation(tObjectiveSharedDataArgs, aOperationMetaData, aDocument);
    
    XMLGen::set_file_names("constraint_data_files", aOperationMetaData);
    auto tConstraintSharedDataArgs = XMLGen::get_criterion_value_argument_metadata("constraint", aOperationMetaData);
    XMLGen::write_harvest_constraint_data_from_file_operation(tConstraintSharedDataArgs, aOperationMetaData, aDocument);
}
// function write_harvest_data_from_file_operation

void write_web_app_input_deck
(XMLGen::OperationMetaData& aOperationMetaData)
{
    XMLGen::append_web_port_numbers(aOperationMetaData);
    XMLGen::flask::write_web_app_executable("run_web_app", ".py", aOperationMetaData);
    for(auto& tFileName : aOperationMetaData.get("run_app_files"))
    {
         auto tIndex = &tFileName - &aOperationMetaData.get("run_app_files")[0];
        XMLGen::move_file_to_subdirectory(tFileName, aOperationMetaData.get("subdirectories")[tIndex]);
        auto tCommand = std::string("rm -f ") + tFileName;
        Plato::system(tCommand.c_str());
    }    
}
// function write_web_app_input_deck

}
// namespace XMLGen