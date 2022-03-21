 /*
 * XMLGeneratorPlatoGemma_UnitTester.cpp
 *
 *  Created on: Feb 28, 2022
 */

#include <gtest/gtest.h>

#include "pugixml.hpp"

#include "XMLGeneratorUtilities.hpp"
#include "XMLGeneratorDataStruct.hpp"
#include "XMLGeneratorParserUtilities.hpp"
#include "XMLGeneratorOperationsDataStructures.hpp"

namespace XMLGen
{

namespace gemma
{

struct FileFormats
{
private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> mMap;

public:
    FileFormats()
    {
        this->build();
    }
    std::string get(const std::string& aSolverType, const std::string& aFileFormat) const
    {
        auto tOuterItr = mMap.find(aSolverType);
        if(tOuterItr == mMap.end())
        {
            auto tMsg = std::string("Gemma solver '") + aSolverType + "' is not supported by Plato. Supported Gemma solvers are:\n" ;
            for(auto tPair : mMap)
            {
                tMsg = tMsg + "  " + tPair.first + "\n";
            }
            THROWERR(tMsg)
        }

        auto tInnerItr = tOuterItr->second.find(aFileFormat);
        if(tInnerItr == tOuterItr->second.end())
        {
            auto tMsg = std::string("Gemma file format '") + aFileFormat + "' is not supported by Plato. Supported Gemma file formats are:\n" ;
            for(auto tPair : tOuterItr->second)
            {
                tMsg = tMsg + "  " + tPair.first + "\n";
            }
            THROWERR(tMsg)
        }
        return tInnerItr->second;
    }

private:
    void build()
    {
        mMap["matched_power_balance"]["output"] = std::string("matched_power_balance.dat");
        mMap["matched_power_balance"]["input"] = std::string("gemma_matched_power_balance_input_deck.yaml");

        mMap["unmatched_power_balance"]["output"] = std::string("unmatched_power_balance.dat");
        mMap["unmatched_power_balance"]["input"] = std::string("gemma_unmatched_power_balance_input_deck.yaml");
    }
};

struct ServiceTypeBasedOptions
{
private:
    std::unordered_map<std::string, std::unordered_map<std::string,std::vector<std::string>>> mMap;

public:
    explicit ServiceTypeBasedOptions(const size_t& aNumConcurrentEvals)
    {
        this->build(aNumConcurrentEvals);
    }

    const std::vector<std::string>& get(const std::string& aOuterKey, const std::string& aInnerKey) const
    {
        auto tOuterItr = mMap.find(aOuterKey);
        if(tOuterItr == mMap.end())
        {
            THROWERR(std::string("Did not find outer key '") + aOuterKey + "' in map.")
        }
        auto tInnerItr = tOuterItr->second.find(aInnerKey);
        if(tInnerItr == tOuterItr->second.end())
        {
            THROWERR(std::string("Did not find inner key '") + aInnerKey + "' in map.")
        }
        return tInnerItr->second;
    }

    void set(const std::string& aOuterKey, const std::string& aInnerKey, const std::vector<std::string>& aValues)
    {
        mMap[aOuterKey][aInnerKey] = aValues;
    }

private:
    void build(const size_t& aNumConcurrentEvals)
    {
        // command defaults
        mMap["commands"]["web_app"] = std::vector<std::string>(aNumConcurrentEvals, "curl");
        mMap["commands"]["plato_app"] = std::vector<std::string>(aNumConcurrentEvals, "");
        mMap["commands"]["system_call"] = std::vector<std::string>(aNumConcurrentEvals, "gemma");
    
        // functions defaults
        mMap["functions"]["web_app"] = std::vector<std::string>(aNumConcurrentEvals, "SystemCall");
        mMap["functions"]["plato_app"] = std::vector<std::string>(aNumConcurrentEvals, "");
        mMap["functions"]["system_call"] = std::vector<std::string>(aNumConcurrentEvals, "SystemCallMPI");
    }
};

}
// namespace gemma

void set_evaluation_subdirectory_names
(const std::string& aSubDirBaseName, 
 const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData)
{
    auto tConcurrentEvals = aInputMetaData.optimization_parameters().concurrent_evaluations();
    tConcurrentEvals = tConcurrentEvals.empty() ? "1" : tConcurrentEvals;
    auto tNumConcurrentEvals = std::stoi(tConcurrentEvals);
    for(decltype(tNumConcurrentEvals) tEvalIndex = 0; tEvalIndex < tNumConcurrentEvals; tEvalIndex++)
    {
        auto tDirectoryName = aSubDirBaseName + "_" + std::to_string(tEvalIndex) + std::string("/");
        aOperationMetaData.append("subdirectories", tDirectoryName);
    }
}

void append_integer_to_option
(const std::string& aKey,
 const std::string& aValue,
 XMLGen::OperationMetaData& aOperationMetaDataOptions)
{
    auto tNumConcurrentEvals = std::stoi(aOperationMetaDataOptions.get("concurrent_evaluations").front());
    for(decltype(tNumConcurrentEvals) tIndex = 0; tIndex < tNumConcurrentEvals; tIndex++)
    {
        auto tStrIndex = std::to_string(tIndex);
        auto tConcatanetedValue = aValue + tStrIndex;
        aOperationMetaDataOptions.append(aKey, tConcatanetedValue);
    }    
}

void set_aprepro_system_call_options
(XMLGen::OperationMetaData& aOperationMetaDataOptions)
{
    auto tNumConcurrentEvals = std::stoi(aOperationMetaDataOptions.get("concurrent_evaluations").front());
    aOperationMetaDataOptions.set("functions", std::vector<std::string>(tNumConcurrentEvals, "SystemCall"));
    aOperationMetaDataOptions.set("commands", std::vector<std::string>(tNumConcurrentEvals, "aprepro"));
    XMLGen::append_integer_to_option("names", "aprepro_", aOperationMetaDataOptions); 

    auto tDescriptors = aOperationMetaDataOptions.get("descriptors");
    aOperationMetaDataOptions.set("options", tDescriptors);
    auto tInputFileName = aOperationMetaDataOptions.get("input_file").front();
    auto tTemplateFileName = aOperationMetaDataOptions.get("template_file").front();
    std::vector<std::string> tArguments = {"-q", tTemplateFileName, tInputFileName};
    aOperationMetaDataOptions.set("arguments", tArguments);
}

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

std::vector<std::string> 
get_unique_scenario_ids
(const std::string& aTargetCodeName,
 const XMLGen::InputData& aInputMetaData)
{
    std::vector<std::string> tUniqueScenarioIDs;
    auto tLowerTargetCodeName = XMLGen::to_lower(aTargetCodeName);

    for(auto& tID : aInputMetaData.objective.criteriaIDs)
    {
        auto tIndex = &tID - &aInputMetaData.objective.criteriaIDs[0];
        auto tServiceID = aInputMetaData.objective.serviceIDs[tIndex];
        auto tLowerCode = XMLGen::to_lower(aInputMetaData.service(tServiceID).code());
        if(tLowerCode == tLowerTargetCodeName)
        {
            auto tScenarioID = aInputMetaData.objective.scenarioIDs[tIndex];
            if(std::find(tUniqueScenarioIDs.begin(), tUniqueScenarioIDs.end(), tScenarioID) == tUniqueScenarioIDs.end())
            {
                tUniqueScenarioIDs.push_back(tScenarioID);   
            }
        }
    }

    return tUniqueScenarioIDs;
}

std::string set_descriptor_value
(const std::string& aTargetDescriptor,
 const std::string& aTargetDescriptorValue,
 const std::vector<std::string>& aDescriptors)
{
    auto tOutput = std::find(aDescriptors.begin(), aDescriptors.end(), aTargetDescriptor) == aDescriptors.end() ? aTargetDescriptorValue : std::string("{") + aTargetDescriptor + "}";
    return tOutput;
}

std::vector<size_t> 
get_service_indices
(const std::string& aTargetCodeName,
 const XMLGen::InputData& aInputMetaData)
{
    std::vector<size_t> tTargetServiceIndices;
    auto tLowerTargetCodeName = XMLGen::to_lower(aTargetCodeName);
    for(auto& tService : aInputMetaData.services())
    {
        auto tLowerCode = XMLGen::to_lower(tService.code());
        if(tLowerCode == tLowerTargetCodeName)
        {
            size_t tIndex = &tService - &aInputMetaData.services()[0];
            tTargetServiceIndices.push_back(tIndex);
        }
    }
    return tTargetServiceIndices;
}

std::vector<std::string> 
get_values_from_service
(const std::string& aCodeName,
 const std::string& aTargetValue,
 const XMLGen::InputData& aInputMetaData)
{
    std::vector<std::string> tOutput;
    auto tLowerCodeName = XMLGen::to_lower(aCodeName);
    auto tLowerTargetValue = XMLGen::to_lower(aTargetValue);
    auto tServiceIndices = XMLGen::get_service_indices(tLowerCodeName, aInputMetaData);
    for(auto tIndex : tServiceIndices)
    {
        auto tValue = aInputMetaData.service(tIndex).value(tLowerTargetValue);
        tOutput.push_back(tValue);
    }
    return tOutput;
}

void set_system_call_option_from_service
(const std::string& aTargetValue,
 const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData)
{
    auto tLowerTargetValue = XMLGen::to_lower(aTargetValue);
    auto tLowerTargetCode = XMLGen::to_lower(aOperationMetaData.back("code"));
    auto tValues = XMLGen::get_values_from_service(tLowerTargetCode, aTargetValue, aInputMetaData);
    auto tNumConcurrentEvals = std::stoi(aOperationMetaData.front("concurrent_evaluations"));
    if( tNumConcurrentEvals > tValues.size() && tValues.size() == 1u )
    {
        aOperationMetaData.set( tLowerTargetValue, std::vector<std::string>(tNumConcurrentEvals, tValues.front()) );
    }
    else if( tNumConcurrentEvals == tValues.size() )
    {
        aOperationMetaData.set( tLowerTargetValue, tValues );
    }
    else
    {
        THROWERR(std::string("The number of services with code name '") + tLowerTargetCode 
            + "' should be equal to one or match the number of concurrent evaluations, which is set to '" 
            + std::to_string(tNumConcurrentEvals) + "'.")
    }
}

void check_plato_app_service_type(const std::string& aType)
{
    if( aType == "plato_app" )
    {
        THROWERR("The 'run system call' function cannot be used with service of type 'plato_app'. The 'run system call' function can only be used with services of type 'web_app' and 'system_call'.")
    }
}

void set_run_system_call_option
(const std::string& aOuterKey,
 XMLGen::OperationMetaData& aOperationMetaData)
{
    auto tNumConcurrentEvals = std::stoi(aOperationMetaData.back("concurrent_evaluations"));
    XMLGen::gemma::ServiceTypeBasedOptions tOptions(tNumConcurrentEvals);
    for(auto& tType : aOperationMetaData.get("type"))
    {
        XMLGen::check_plato_app_service_type(tType);
        auto tIndex = &tType - &aOperationMetaData.get("type")[0];
        aOperationMetaData.append(aOuterKey, tOptions.get(aOuterKey, tType)[tIndex]);
    }
}

void pre_process_general_run_system_call_options
(const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData)
{
    auto tNumConcurrentEvals = aInputMetaData.optimization_parameters().concurrent_evaluations();
    aOperationMetaData.append("concurrent_evaluations", tNumConcurrentEvals);
      
    XMLGen::set_system_call_option_from_service("type", aInputMetaData, aOperationMetaData);
    XMLGen::set_system_call_option_from_service("number_processors", aInputMetaData, aOperationMetaData);

    XMLGen::set_run_system_call_option("commands", aOperationMetaData);
    XMLGen::set_run_system_call_option("functions", aOperationMetaData);
}

void set_run_system_call_shared_data_arguments
(XMLGen::OperationMetaData& aOperationMetaData)
{
    XMLGen::gemma::FileFormats tFileFormats; 
    for(auto& tType : aOperationMetaData.get("type"))
    {
        XMLGen::check_plato_app_service_type(tType);
        if(tType == "system_call")
        {
            auto tInputFileName = tFileFormats.get(aOperationMetaData.front("algorithm"), "input");
            aOperationMetaData.append("arguments", tInputFileName);
        }
        else
        {
            auto tLOCAL_HOST_NUMBER_BASE = 7000;
            auto tHostNumber = tLOCAL_HOST_NUMBER_BASE + (&tType - &aOperationMetaData.get("type")[0]);
            aOperationMetaData.append("arguments", std::string("http://localhost:") + std::to_string(tHostNumber) + "/rungemma/");
        }
    }
}

void write_run_system_call_operation
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
                                          "true", "false",
                                          aOperationMetaData.get("subdirectories")[tIndex],
                                          aOperationMetaData.get("arguments")[tIndex]};
        XMLGen::append_children(tKeys, tVals, tOperation);
    }
}

std::string get_data_group_type(const std::string& aDataGroup)
{
    auto tDataGroupType = XMLGen::to_lower( aDataGroup.substr(0, aDataGroup.find_first_of("_")) );
    tDataGroupType = tDataGroupType == "column" ? "Column" : "Row";
    return tDataGroupType;
}

std::string get_data_group_index(const std::string& aDataGroup)
{
    auto tDataGroupIndex = aDataGroup.substr(aDataGroup.find_first_of("_")+1);
    return tDataGroupIndex;
}

void check_if_data_file_names_are_defined
(const std::string& aKey, 
 const std::string& aAlgo,
 const std::string& aFileType,
 XMLGen::OperationMetaData& aOperationMetaData)
{
    if( !aOperationMetaData.is_defined(aKey) )
    {
        return;
    }
    XMLGen::gemma::FileFormats tFileFormats;
    auto tProposedAppFileName = tFileFormats.get(aAlgo, aFileType);

    std::vector<std::string> tFileNames;
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

std::vector<std::string> 
get_criteria_ids
(const std::string& aType,
 const XMLGen::InputData& aInputMetaData)
{
    std::vector<std::string> tOutput;
    auto tLowerType = XMLGen::to_lower(aType);
    for(auto& tCriterion : aInputMetaData.criteria())
    {
        if(tLowerType == tCriterion.type())
        {
            tOutput.push_back(tCriterion.id());
        }
    }
    return tOutput;
}

void set_system_call_constraint_criteria_ids
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

void set_system_call_objective_criteria_ids
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

void pre_process_general_harvest_data_from_file_operation_options
(const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData)
{
    auto tNumConcurrentEvals = aInputMetaData.optimization_parameters().concurrent_evaluations();
    aOperationMetaData.append("concurrent_evaluations", tNumConcurrentEvals);
    XMLGen::append_integer_to_option("names", "harvest_data_", aOperationMetaData);
}

void pre_process_aprepro_system_call_inputs
(const XMLGen::OperationMetaData& aOperationMetaData,
 std::vector<XMLGen::OperationArgument>& aSharedDataArgs)
{
    auto tSize = aOperationMetaData.get("descriptors").size();
    if(tSize <= 0)
    {
        THROWERR("Number of parameters can not be less or equal than zero.")
    }

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
        aSharedDataArgs.push_back(tSharedDataArgument);
    }
}

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

std::vector<XMLGen::OperationArgument>
pre_process_criterion_value_argument_data
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

namespace matched_power_balance
{

void preprocess_aprepro_system_call_options
(const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData)
{
    XMLGen::gemma::FileFormats tFileFormats; 
    auto tInputFileName = tFileFormats.get("matched_power_balance", "input");
    aOperationMetaData.append("input_file", tInputFileName);
    aOperationMetaData.append("template_file", tInputFileName + ".template");
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

void write_aprepro_system_call_operation
(const std::string& aSubDirBaseName, 
 const XMLGen::InputData& aInputMetaData,
 pugi::xml_document& tDocument)
{
    XMLGen::OperationMetaData tOperationMetaData;
    XMLGen::set_evaluation_subdirectory_names(aSubDirBaseName, aInputMetaData, tOperationMetaData);
    XMLGen::matched_power_balance::preprocess_aprepro_system_call_options(aInputMetaData, tOperationMetaData);
    XMLGen::set_aprepro_system_call_options(tOperationMetaData);
    
    std::vector<XMLGen::OperationArgument> tSharedDataArgs;
    XMLGen::pre_process_aprepro_system_call_inputs(tOperationMetaData, tSharedDataArgs);
    
    XMLGen::are_aprepro_input_options_defined(tOperationMetaData);
    XMLGen::write_aprepro_system_call_operation(tSharedDataArgs, tOperationMetaData, tDocument);
}

void are_inputs_defined(const std::unordered_map<std::string,std::string>& aKeyValuePairs)
{
    std::vector<std::string> tValidKeys = 
        {"cavity_radius", "cavity_height", "conductivity", "slot_length", "slot_width", "slot_depth", "frequency_min", "frequency_max", "frequency_step"};
    for(auto& tKey : tValidKeys)
    {
        auto tItr = aKeyValuePairs.find(tKey);
        if(tItr == aKeyValuePairs.end() || tItr->second.empty())
        {
            THROWERR(std::string("Input key '") + tKey + "' was not properly defined by the user in the plato input deck. " 
                + "Either the parameter was not defined in the input deck or its value is empty.")
        }
    }
}

void write_input_deck_to_file
(const std::string& aFileName, 
 const std::unordered_map<std::string,std::string>& aKeyValuePairs)
{
    XMLGen::matched_power_balance::are_inputs_defined(aKeyValuePairs);

    std::ofstream tOutFile;
    tOutFile.open(aFileName, std::ofstream::out | std::ofstream::trunc);
    tOutFile << "%YAML 1.1\n";
    tOutFile << "---\n\n";

    tOutFile << "Gemma-dynamic:\n\n";

    tOutFile << "  Global:\n";
    tOutFile << "    Description: Higgins cylinder\n";
    tOutFile << "    Solution type: power balance\n\n";

    tOutFile << "  Power balance: \n";
    tOutFile << "    Algorithm: matched bound\n";
    tOutFile << "    Radius: " << aKeyValuePairs.at("cavity_radius") << "\n";
    tOutFile << "    Height: " << aKeyValuePairs.at("cavity_height") << "\n";
    tOutFile << "    Conductivity: " << aKeyValuePairs.at("conductivity") << "\n";
    tOutFile << "    Slot length: " << aKeyValuePairs.at("slot_length") << "\n";
    tOutFile << "    Slot width: " << aKeyValuePairs.at("slot_width") << "\n";
    tOutFile << "    Slot depth: " << aKeyValuePairs.at("slot_depth") << "\n";
    tOutFile << "    Start frequency range: " << aKeyValuePairs.at("frequency_min") << "\n";
    tOutFile << "    End frequency range: " << aKeyValuePairs.at("frequency_max") << "\n";
    tOutFile << "    Frequency interval size: " << aKeyValuePairs.at("frequency_step") << "\n\n";;

    tOutFile << "...\n";
    tOutFile.close();
}

std::unordered_map<std::string,std::string> 
get_input_key_value_pairs
(const XMLGen::Scenario& aScenario,
 const XMLGen::Material& aMaterial,
 const std::vector<std::string>& aDescriptors)
{
    std::unordered_map<std::string,std::string> tMap;
    tMap["slot_width"] = XMLGen::set_descriptor_value("slot_width", aScenario.getValue("slot_width"), aDescriptors);
    tMap["slot_depth"] = XMLGen::set_descriptor_value("slot_depth", aScenario.getValue("slot_depth"), aDescriptors);
    tMap["slot_length"] = XMLGen::set_descriptor_value("slot_length", aScenario.getValue("slot_length"), aDescriptors);
    tMap["cavity_radius"] = XMLGen::set_descriptor_value("cavity_radius", aScenario.getValue("cavity_radius"), aDescriptors);
    tMap["cavity_height"] = XMLGen::set_descriptor_value("cavity_height", aScenario.getValue("cavity_height"), aDescriptors);
    tMap["frequency_min"] = XMLGen::set_descriptor_value("frequency_min", aScenario.getValue("frequency_min"), aDescriptors);
    tMap["frequency_max"] = XMLGen::set_descriptor_value("frequency_max", aScenario.getValue("frequency_max"), aDescriptors);
    tMap["frequency_step"] = XMLGen::set_descriptor_value("frequency_step", aScenario.getValue("frequency_step"), aDescriptors);
    tMap["conductivity"] = XMLGen::set_descriptor_value("conductivity", aMaterial.property("conductivity"), aDescriptors);
    return tMap;
}

void write_input_deck
(const XMLGen::InputData& aInputMetaData,
 const XMLGen::OperationMetaData& aOperationMetaData)
{
    XMLGen::gemma::FileFormats tFileFormats; 
    auto tInputFileName = tFileFormats.get("matched_power_balance", "input");
    auto tDescriptors = aInputMetaData.optimization_parameters().descriptors();
    auto tUniqueGemmaScenarioIDs = XMLGen::get_unique_scenario_ids("gemma", aInputMetaData);
    for(auto& tScenarioID : tUniqueGemmaScenarioIDs)
    {
        auto tScenario = aInputMetaData.scenario(tScenarioID);
        auto tMaterialID = tScenario.material();
        auto tMaterial = aInputMetaData.material(tMaterialID);
        auto tInputKeyValuePairs = XMLGen::matched_power_balance::get_input_key_value_pairs(tScenario, tMaterial, tDescriptors);
        auto tFileName = tDescriptors.empty() ? tInputFileName : tInputFileName + ".template";
        XMLGen::matched_power_balance::write_input_deck_to_file(tFileName, tInputKeyValuePairs);

        for(auto& tSubDirName : aOperationMetaData.get("subdirectories"))
        {
            auto tCommand = std::string("mkdir -p ") + tSubDirName;
            Plato::system(tCommand.c_str());
            tCommand = std::string("cp ") + tFileName + " " + tSubDirName;
            Plato::system(tCommand.c_str());
        }

        auto tCommand = std::string("rm -f ") + tFileName;
        Plato::system(tCommand.c_str());
    }
}

void write_run_system_call_operation
(const std::string& aSubDirBaseName, 
 const XMLGen::InputData& aInputMetaData,
 pugi::xml_document& tDocument)
{
    XMLGen::OperationMetaData tOperationMetaData;
    XMLGen::set_evaluation_subdirectory_names(aSubDirBaseName, aInputMetaData, tOperationMetaData);

    tOperationMetaData.append("code", "gemma");
    tOperationMetaData.append("algorithm", "matched_power_balance");
    XMLGen::pre_process_general_run_system_call_options(aInputMetaData, tOperationMetaData);
    XMLGen::append_integer_to_option("names", "gemma_", tOperationMetaData);
    XMLGen::set_run_system_call_shared_data_arguments(tOperationMetaData);
    XMLGen::write_run_system_call_operation(tOperationMetaData, tDocument);
}

void set_app_data_file_name_for_harvest_data_operation
(const XMLGen::InputData& aInputMetaData,
 const std::vector<std::string>& aSystemCallCriteriaIDs, 
 XMLGen::OperationMetaData& aOperationMetaData)
{
    XMLGen::gemma::FileFormats tFileFormats;
    auto tProposedAppFileName = tFileFormats.get("matched_power_balance", "output");
    for(auto tID : aSystemCallCriteriaIDs)
    {
        auto tIndex = &tID - &aSystemCallCriteriaIDs[0];
        auto tProposedAppFilePath = std::string("./") + aOperationMetaData.get("subdirectories")[tIndex] + "/" + tProposedAppFileName;
        auto tAppFilePath = aInputMetaData.criterion(tID).value("data_file").empty() ? tProposedAppFilePath : aInputMetaData.criterion(tID).value("data_file");
        aOperationMetaData.append("app_data_file_paths", tAppFilePath);
    }
}

void write_harvest_data_from_file_operation
(const std::string& aSubDirBaseName, 
 const XMLGen::InputData& aInputMetaData,
 pugi::xml_document& aDocument)
{
    XMLGen::OperationMetaData tOperationMetaData;
    XMLGen::set_evaluation_subdirectory_names(aSubDirBaseName, aInputMetaData, tOperationMetaData);

    auto tCriteriaIDs = XMLGen::get_criteria_ids("system_call", aInputMetaData);
    XMLGen::set_system_call_objective_criteria_ids(tCriteriaIDs, aInputMetaData, tOperationMetaData);
    XMLGen::set_system_call_constraint_criteria_ids(tCriteriaIDs, aInputMetaData, tOperationMetaData);
    XMLGen::pre_process_general_harvest_data_from_file_operation_options(aInputMetaData, tOperationMetaData);

    XMLGen::check_if_data_file_names_are_defined("objective_data_files", "matched_power_balance", "output", tOperationMetaData);
    auto tObjectiveSharedDataArgs = XMLGen::pre_process_criterion_value_argument_data("objective", tOperationMetaData);
    XMLGen::write_harvest_objective_data_from_file_operation(tObjectiveSharedDataArgs, tOperationMetaData, aDocument);
    
    XMLGen::check_if_data_file_names_are_defined("constraint_data_files", "matched_power_balance", "output", tOperationMetaData);
    auto tConstraintSharedDataArgs = XMLGen::pre_process_criterion_value_argument_data("constraint", tOperationMetaData);
    XMLGen::write_harvest_constraint_data_from_file_operation(tConstraintSharedDataArgs, tOperationMetaData, aDocument);
}

}
// namespace matched_power_balance

namespace gemma
{

void write_input_deck(const XMLGen::InputData& aInputMetaData)
{
    XMLGen::OperationMetaData tOperationMetaData;
    XMLGen::set_evaluation_subdirectory_names("gemma_evaluation", aInputMetaData, tOperationMetaData);
    XMLGen::matched_power_balance::write_input_deck(aInputMetaData, tOperationMetaData);
}

void write_system_call_app_operation_files(const XMLGen::InputData& aInputMetaData)
{
    pugi::xml_document tDocument;
    const std::string tSubDirBaseName("gemma_evaluation");
    XMLGen::matched_power_balance::write_aprepro_system_call_operation(tSubDirBaseName, aInputMetaData, tDocument);
    XMLGen::matched_power_balance::write_run_system_call_operation(tSubDirBaseName, aInputMetaData, tDocument);
    XMLGen::matched_power_balance::write_harvest_data_from_file_operation(tSubDirBaseName, aInputMetaData, tDocument);
    tDocument.save_file("plato_gemma_app_operation_file.xml", "  ");
}

void write_operation_file(const XMLGen::InputData& aInputMetaData)
{
    XMLGen::gemma::write_system_call_app_operation_files(aInputMetaData);
}

}
// namespace gemma

}
// namespace XMLGen

namespace PlatoTestXMLGenerator
{

TEST(PlatoTestXMLGenerator, write_operation_file_gemma_usecase_system_call)
{
    // use case: gemma call is defined by a system call mpi operation
    XMLGen::InputData tInputMetaData;
    // define criterion
    XMLGen::Criterion tCriterionOne;
    tCriterionOne.id("1");
    tCriterionOne.type("volume");
    tInputMetaData.append(tCriterionOne);
    XMLGen::Criterion tCriterionTwo;
    tCriterionTwo.id("2");
    tCriterionTwo.type("system_call");
    tCriterionTwo.append("data_group", "column_1");
    tCriterionTwo.append("data_extraction_operation", "max");
    tCriterionTwo.append("data_file", "matched_power_balance.dat");
    tInputMetaData.append(tCriterionTwo);
    XMLGen::Criterion tCriterionThree;
    tCriterionThree.id("3");
    tCriterionThree.type("system_call");
    tCriterionThree.append("data_group", "column_2");
    tCriterionThree.append("data_extraction_operation", "max");
    tInputMetaData.append(tCriterionThree);
    // define objective
    XMLGen::Objective tObjective;
    tObjective.criteriaIDs.push_back("1");
    tObjective.criteriaIDs.push_back("2");
    tInputMetaData.objective = tObjective;
    // define constraints
    XMLGen::Constraint tConstraint;
    tConstraint.criterion("3");
    tInputMetaData.constraints.push_back(tConstraint);
    // define optimization parameters 
    XMLGen::OptimizationParameters tOptParams;
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    tOptParams.descriptors(tDescriptors);
    tOptParams.append("concurrent_evaluations", "2");
    tInputMetaData.set(tOptParams);
    // service parameters
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tServiceOne.append("type", "system_call");
    tServiceOne.append("number_processors", "1");
    tInputMetaData.append(tServiceOne);
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "platomain");
    tServiceTwo.append("id", "2");
    tServiceTwo.append("type", "plato_app");
    tServiceTwo.append("number_processors", "2");
    tInputMetaData.append(tServiceTwo);
    
    // write files
    XMLGen::gemma::write_operation_file(tInputMetaData);
    auto tReadData = XMLGen::read_data_from_file("plato_gemma_app_operation_file.xml");
    auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>SystemCall</Function><Name>aprepro_0</Name><Command>aprepro</Command><ChDir>gemma_evaluation_0/</ChDir><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>gemma_matched_power_balance_input_deck.yaml.template</Argument><Argument>gemma_matched_power_balance_input_deck.yaml</Argument><Option>slot_length</Option><Option>slot_width</Option><Option>slot_depth</Option><Input><ArgumentName>parameters_0</ArgumentName><Layout>scalar</Layout><Size>3</Size></Input></Operation><Operation><Function>SystemCall</Function><Name>aprepro_1</Name><Command>aprepro</Command><ChDir>gemma_evaluation_1/</ChDir><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>gemma_matched_power_balance_input_deck.yaml.template</Argument><Argument>gemma_matched_power_balance_input_deck.yaml</Argument><Option>slot_length</Option><Option>slot_width</Option><Option>slot_depth</Option><Input><ArgumentName>parameters_1</ArgumentName><Layout>scalar</Layout><Size>3</Size></Input></Operation><Operation><Function>SystemCallMPI</Function><Name>gemma_0</Name><Command>gemma</Command><OnChange>true</OnChange><AppendInput>false</AppendInput><ChDir>gemma_evaluation_0/</ChDir><Argument>gemma_matched_power_balance_input_deck.yaml</Argument></Operation><Operation><Function>SystemCallMPI</Function><Name>gemma_1</Name><Command>gemma</Command><OnChange>true</OnChange><AppendInput>false</AppendInput><ChDir>gemma_evaluation_1/</ChDir><Argument>gemma_matched_power_balance_input_deck.yaml</Argument></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_objective_criterion_id_2_eval_0</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>objectivevalueforcriterion2andevaluation0</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_objective_criterion_id_2_eval_1</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>objectivevalueforcriterion2andevaluation1</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_constraint_criterion_id_3_eval_0</Name><File>./gemma_evaluation_0/matched_power_balance.dat</File><Operation>max</Operation><Column>2</Column><Output><ArgumentName>constraintvalueforcriterion3andevaluation0</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_constraint_criterion_id_3_eval_1</Name><File>./gemma_evaluation_1/matched_power_balance.dat</File><Operation>max</Operation><Column>2</Column><Output><ArgumentName>constraintvalueforcriterion3andevaluation1</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation>");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
    Plato::system("rm -f plato_gemma_app_operation_file.xml");
}

TEST(PlatoTestXMLGenerator, write_operation_file_gemma_usecase_web_app)
{
    // use case: gemma call is defined by a web app
    XMLGen::InputData tInputMetaData;
    // define criterion
    XMLGen::Criterion tCriterionOne;
    tCriterionOne.id("1");
    tCriterionOne.type("volume");
    tInputMetaData.append(tCriterionOne);
    XMLGen::Criterion tCriterionTwo;
    tCriterionTwo.id("2");
    tCriterionTwo.type("system_call");
    tCriterionTwo.append("data_group", "column_1");
    tCriterionTwo.append("data_extraction_operation", "max");
    tCriterionTwo.append("data_file", "matched_power_balance.dat");
    tInputMetaData.append(tCriterionTwo);
    XMLGen::Criterion tCriterionThree;
    tCriterionThree.id("3");
    tCriterionThree.type("system_call");
    tCriterionThree.append("data_group", "column_2");
    tCriterionThree.append("data_extraction_operation", "max");
    tInputMetaData.append(tCriterionThree);
    // define objective
    XMLGen::Objective tObjective;
    tObjective.criteriaIDs.push_back("1");
    tObjective.criteriaIDs.push_back("2");
    tInputMetaData.objective = tObjective;
    // define constraints
    XMLGen::Constraint tConstraint;
    tConstraint.criterion("3");
    tInputMetaData.constraints.push_back(tConstraint);
    // define optimization parameters 
    XMLGen::OptimizationParameters tOptParams;
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    tOptParams.descriptors(tDescriptors);
    tOptParams.append("concurrent_evaluations", "2");
    tInputMetaData.set(tOptParams);
    // service parameters
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tServiceOne.append("type", "web_app");
    tServiceOne.append("number_processors", "1");
    tInputMetaData.append(tServiceOne);
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "platomain");
    tServiceTwo.append("id", "2");
    tServiceTwo.append("type", "plato_app");
    tServiceTwo.append("number_processors", "2");
    tInputMetaData.append(tServiceTwo);
    
    // write files
    XMLGen::gemma::write_operation_file(tInputMetaData);
    auto tReadData = XMLGen::read_data_from_file("plato_gemma_app_operation_file.xml");
    auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>SystemCall</Function><Name>aprepro_0</Name><Command>aprepro</Command><ChDir>gemma_evaluation_0/</ChDir><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>gemma_matched_power_balance_input_deck.yaml.template</Argument><Argument>gemma_matched_power_balance_input_deck.yaml</Argument><Option>slot_length</Option><Option>slot_width</Option><Option>slot_depth</Option><Input><ArgumentName>parameters_0</ArgumentName><Layout>scalar</Layout><Size>3</Size></Input></Operation><Operation><Function>SystemCall</Function><Name>aprepro_1</Name><Command>aprepro</Command><ChDir>gemma_evaluation_1/</ChDir><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>gemma_matched_power_balance_input_deck.yaml.template</Argument><Argument>gemma_matched_power_balance_input_deck.yaml</Argument><Option>slot_length</Option><Option>slot_width</Option><Option>slot_depth</Option><Input><ArgumentName>parameters_1</ArgumentName><Layout>scalar</Layout><Size>3</Size></Input></Operation><Operation><Function>SystemCall</Function><Name>gemma_0</Name><Command>curl</Command><OnChange>true</OnChange><AppendInput>false</AppendInput><ChDir>gemma_evaluation_0/</ChDir><Argument>http://localhost:7000/rungemma/</Argument></Operation><Operation><Function>SystemCall</Function><Name>gemma_1</Name><Command>curl</Command><OnChange>true</OnChange><AppendInput>false</AppendInput><ChDir>gemma_evaluation_1/</ChDir><Argument>http://localhost:7001/rungemma/</Argument></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_objective_criterion_id_2_eval_0</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>objectivevalueforcriterion2andevaluation0</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_objective_criterion_id_2_eval_1</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>objectivevalueforcriterion2andevaluation1</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_constraint_criterion_id_3_eval_0</Name><File>./gemma_evaluation_0/matched_power_balance.dat</File><Operation>max</Operation><Column>2</Column><Output><ArgumentName>constraintvalueforcriterion3andevaluation0</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_constraint_criterion_id_3_eval_1</Name><File>./gemma_evaluation_1/matched_power_balance.dat</File><Operation>max</Operation><Column>2</Column><Output><ArgumentName>constraintvalueforcriterion3andevaluation1</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation>");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
    Plato::system("rm -f plato_gemma_app_operation_file.xml");
}

TEST(PlatoTestXMLGenerator, write_harvest_data_from_file_operation_matched_power_balance_usecase_1)
{
    // use case: only objective functions are defined
    XMLGen::InputData tInputMetaData;
    // define criterion
    XMLGen::Criterion tCriterionOne;
    tCriterionOne.id("1");
    tCriterionOne.type("volume");
    tInputMetaData.append(tCriterionOne);
    XMLGen::Criterion tCriterionTwo;
    tCriterionTwo.id("2");
    tCriterionTwo.type("system_call");
    tCriterionTwo.append("data_group", "column_1");
    tCriterionTwo.append("data_extraction_operation", "max");
    tCriterionTwo.append("data_file", "matched_power_balance.dat");
    tInputMetaData.append(tCriterionTwo);
    XMLGen::Criterion tCriterionThree;
    tCriterionThree.id("3");
    tCriterionThree.type("mass");
    tInputMetaData.append(tCriterionThree);
    // define objective
    XMLGen::Objective tObjective;
    tObjective.criteriaIDs.push_back("1");
    tObjective.criteriaIDs.push_back("2");
    tObjective.criteriaIDs.push_back("3");
    tInputMetaData.objective = tObjective;
    // define optimization parameters 
    XMLGen::OptimizationParameters tOptParams;
    tOptParams.append("concurrent_evaluations", "2");
    tInputMetaData.set(tOptParams);

    // test function 
    pugi::xml_document tDocument;
    XMLGen::matched_power_balance::write_harvest_data_from_file_operation("gemma_evaluation", tInputMetaData, tDocument);
    tDocument.save_file("dummy.txt", " ");

    auto tReadData = XMLGen::read_data_from_file("dummy.txt");
    auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>HarvestDataFromFile</Function><Name>harvest_objective_criterion_id_2_eval_0</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>objectivevalueforcriterion2andevaluation0</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_objective_criterion_id_2_eval_1</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>objectivevalueforcriterion2andevaluation1</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation>");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
    Plato::system("rm -f dummy.txt");
}

TEST(PlatoTestXMLGenerator, write_harvest_data_from_file_operation_matched_power_balance_usecase_2)
{
    // use case: only constraint functions are defined
    XMLGen::InputData tInputMetaData;
    // define criterion
    XMLGen::Criterion tCriterionOne;
    tCriterionOne.id("1");
    tCriterionOne.type("volume");
    tInputMetaData.append(tCriterionOne);
    XMLGen::Criterion tCriterionTwo;
    tCriterionTwo.id("2");
    tCriterionTwo.type("system_call");
    tCriterionTwo.append("data_group", "column_1");
    tCriterionTwo.append("data_extraction_operation", "max");
    tCriterionTwo.append("data_file", "matched_power_balance.dat");
    tInputMetaData.append(tCriterionTwo);
    XMLGen::Criterion tCriterionThree;
    tCriterionThree.id("3");
    tCriterionThree.type("mass");
    tInputMetaData.append(tCriterionThree);
    // define constraints
    XMLGen::Constraint tConstraintOne;
    tConstraintOne.criterion("1");
    tInputMetaData.constraints.push_back(tConstraintOne);
    XMLGen::Constraint tConstraintTwo;
    tConstraintTwo.criterion("2");
    tInputMetaData.constraints.push_back(tConstraintTwo);
    XMLGen::Constraint tConstraintThree;
    tConstraintThree.criterion("3");
    tInputMetaData.constraints.push_back(tConstraintThree);
    // define optimization parameters 
    XMLGen::OptimizationParameters tOptParams;
    tOptParams.append("concurrent_evaluations", "2");
    tInputMetaData.set(tOptParams);

    // test function 
    pugi::xml_document tDocument;
    XMLGen::matched_power_balance::write_harvest_data_from_file_operation("gemma_evaluation", tInputMetaData, tDocument);
    tDocument.save_file("dummy.txt", " ");

    auto tReadData = XMLGen::read_data_from_file("dummy.txt");
    auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>HarvestDataFromFile</Function><Name>harvest_constraint_criterion_id_2_eval_0</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>constraintvalueforcriterion2andevaluation0</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_constraint_criterion_id_2_eval_1</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>constraintvalueforcriterion2andevaluation1</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation>");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
    Plato::system("rm -f dummy.txt");
}

TEST(PlatoTestXMLGenerator, write_harvest_data_from_file_operation_matched_power_balance_usecase_3)
{
    // use case: constraint and objective functions defined
    XMLGen::InputData tInputMetaData;
    // define criterion
    XMLGen::Criterion tCriterionOne;
    tCriterionOne.id("1");
    tCriterionOne.type("volume");
    tInputMetaData.append(tCriterionOne);
    XMLGen::Criterion tCriterionTwo;
    tCriterionTwo.id("2");
    tCriterionTwo.type("system_call");
    tCriterionTwo.append("data_group", "column_1");
    tCriterionTwo.append("data_extraction_operation", "max");
    tCriterionTwo.append("data_file", "matched_power_balance.dat");
    tInputMetaData.append(tCriterionTwo);
    XMLGen::Criterion tCriterionThree;
    tCriterionThree.id("3");
    tCriterionThree.type("system_call");
    tCriterionThree.append("data_group", "column_2");
    tCriterionThree.append("data_extraction_operation", "max");
    tInputMetaData.append(tCriterionThree);
    // define objective
    XMLGen::Objective tObjective;
    tObjective.criteriaIDs.push_back("1");
    tObjective.criteriaIDs.push_back("2");
    tInputMetaData.objective = tObjective;
    // define constraints
    XMLGen::Constraint tConstraint;
    tConstraint.criterion("3");
    tInputMetaData.constraints.push_back(tConstraint);
    // define optimization parameters 
    XMLGen::OptimizationParameters tOptParams;
    tOptParams.append("concurrent_evaluations", "2");
    tInputMetaData.set(tOptParams);

    // test function 
    pugi::xml_document tDocument;
    XMLGen::matched_power_balance::write_harvest_data_from_file_operation("gemma_evaluation", tInputMetaData, tDocument);
    tDocument.save_file("dummy.txt", " ");

    auto tReadData = XMLGen::read_data_from_file("dummy.txt");
    auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>HarvestDataFromFile</Function><Name>harvest_objective_criterion_id_2_eval_0</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>objectivevalueforcriterion2andevaluation0</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_objective_criterion_id_2_eval_1</Name><File>matched_power_balance.dat</File><Operation>max</Operation><Column>1</Column><Output><ArgumentName>objectivevalueforcriterion2andevaluation1</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_constraint_criterion_id_3_eval_0</Name><File>./gemma_evaluation_0/matched_power_balance.dat</File><Operation>max</Operation><Column>2</Column><Output><ArgumentName>constraintvalueforcriterion3andevaluation0</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation><Operation><Function>HarvestDataFromFile</Function><Name>harvest_constraint_criterion_id_3_eval_1</Name><File>./gemma_evaluation_1/matched_power_balance.dat</File><Operation>max</Operation><Column>2</Column><Output><ArgumentName>constraintvalueforcriterion3andevaluation1</ArgumentName><Layout>scalar</Layout><Size>1</Size></Output></Operation>");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
    Plato::system("rm -f dummy.txt");
}

TEST(PlatoTestXMLGenerator, pre_process_criterion_value_argument_data)
{
    XMLGen::InputData tInputMetaData;

    XMLGen::OperationMetaData tOperationMetaData;
    tOperationMetaData.append("concurrent_evaluations", "2");
    tOperationMetaData.append("objective_criteria_ids", "10");

    auto tSharedDataArgs = XMLGen::pre_process_criterion_value_argument_data("objective", tOperationMetaData);
    EXPECT_EQ(2u, tSharedDataArgs.size());
    EXPECT_STREQ("1", tSharedDataArgs[0].get("size").c_str());
    EXPECT_STREQ("objective value for criterion 10 and evaluation 0", tSharedDataArgs[0].get("name").c_str());
    EXPECT_STREQ("output", tSharedDataArgs[0].get("type").c_str());
    EXPECT_STREQ("scalar", tSharedDataArgs[0].get("layout").c_str());

    EXPECT_STREQ("1", tSharedDataArgs[1].get("size").c_str());
    EXPECT_STREQ("objective value for criterion 10 and evaluation 1", tSharedDataArgs[1].get("name").c_str());
    EXPECT_STREQ("output", tSharedDataArgs[1].get("type").c_str());
    EXPECT_STREQ("scalar", tSharedDataArgs[1].get("layout").c_str());
}

TEST(PlatoTestXMLGenerator, get_criteria_ids)
{
    XMLGen::InputData tInputMetaData;
    // define criterion
    XMLGen::Criterion tCriterionOne;
    tCriterionOne.id("1");
    tCriterionOne.type("volume");
    tInputMetaData.append(tCriterionOne);
    XMLGen::Criterion tCriterionTwo;
    tCriterionTwo.id("2");
    tCriterionTwo.type("system_call");
    tInputMetaData.append(tCriterionTwo);
    XMLGen::Criterion tCriterionThree;
    tCriterionThree.id("3");
    tCriterionThree.type("mass");
    tInputMetaData.append(tCriterionThree);

    auto tIDs = XMLGen::get_criteria_ids("system_call", tInputMetaData);
    EXPECT_EQ(1u, tIDs.size());
    EXPECT_STREQ("2", tIDs.front().c_str());
}

TEST(PlatoTestXMLGenerator, set_system_call_objective_criteria_ids)
{
    XMLGen::InputData tInputMetaData;
    // define criterion
    XMLGen::Criterion tCriterionOne;
    tCriterionOne.id("1");
    tCriterionOne.type("volume");
    tInputMetaData.append(tCriterionOne);
    XMLGen::Criterion tCriterionTwo;
    tCriterionTwo.id("2");
    tCriterionTwo.type("system_call");
    tCriterionTwo.append("data_group", "column_1");
    tCriterionTwo.append("data_extraction_operation", "max");
    tCriterionTwo.append("data_file", "matched_power_balance.dat");
    tInputMetaData.append(tCriterionTwo);
    XMLGen::Criterion tCriterionThree;
    tCriterionThree.id("3");
    tCriterionThree.type("mass");
    tInputMetaData.append(tCriterionThree);
    // define objective
    XMLGen::Objective tObjective;
    tObjective.criteriaIDs.push_back("1");
    tObjective.criteriaIDs.push_back("2");
    tObjective.criteriaIDs.push_back("3");
    tInputMetaData.objective = tObjective;
    
    XMLGen::OperationMetaData tOperationMetaData;
    auto tSystemCallIDs = XMLGen::get_criteria_ids("system_call", tInputMetaData);
    XMLGen::set_system_call_objective_criteria_ids(tSystemCallIDs, tInputMetaData, tOperationMetaData);
    EXPECT_STREQ("2", tOperationMetaData.get("objective_criteria_ids").front().c_str());
    EXPECT_STREQ("column_1", tOperationMetaData.get("objective_data_groups").front().c_str());
    EXPECT_STREQ("max", tOperationMetaData.get("objective_data_extraction_operations").front().c_str());
    EXPECT_STREQ("matched_power_balance.dat", tOperationMetaData.get("objective_data_files").front().c_str());
}

TEST(PlatoTestXMLGenerator, set_system_call_constraint_criteria_ids)
{
    XMLGen::InputData tInputMetaData;
    // define criterion
    XMLGen::Criterion tCriterionOne;
    tCriterionOne.id("1");
    tCriterionOne.type("system_call");
    tCriterionOne.append("data_group", "column_2");
    tCriterionOne.append("data_extraction_operation", "min");
    tCriterionOne.append("data_file", "matched_power_balance.dat");
    tInputMetaData.append(tCriterionOne);
    XMLGen::Criterion tCriterionTwo;
    tCriterionTwo.id("2");
    tCriterionTwo.type("volume");
    tInputMetaData.append(tCriterionTwo);
    XMLGen::Criterion tCriterionThree;
    tCriterionThree.id("3");
    tCriterionThree.type("mass");
    tInputMetaData.append(tCriterionThree);
    // define constraints
    XMLGen::Constraint tConstraintOne;
    tConstraintOne.criterion("1");
    tInputMetaData.constraints.push_back(tConstraintOne);
    XMLGen::Constraint tConstraintTwo;
    tConstraintTwo.criterion("2");
    tInputMetaData.constraints.push_back(tConstraintTwo);
    XMLGen::Constraint tConstraintThree;
    tConstraintThree.criterion("3");
    tInputMetaData.constraints.push_back(tConstraintThree);
    
    XMLGen::OperationMetaData tOperationMetaData;
    auto tSystemCallIDs = XMLGen::get_criteria_ids("system_call", tInputMetaData);
    XMLGen::set_system_call_constraint_criteria_ids(tSystemCallIDs, tInputMetaData, tOperationMetaData);
    EXPECT_STREQ("1", tOperationMetaData.get("constraint_criteria_ids").front().c_str());
    EXPECT_STREQ("column_2", tOperationMetaData.get("constraint_data_groups").front().c_str());
    EXPECT_STREQ("min", tOperationMetaData.get("constraint_data_extraction_operations").front().c_str());
    EXPECT_STREQ("matched_power_balance.dat", tOperationMetaData.get("constraint_data_files").front().c_str());
}

TEST(PlatoTestXMLGenerator, pre_process_general_harvest_data_from_file_operation_options)
{
    XMLGen::InputData tMetaData;
    // define optimization parameters 
    XMLGen::OptimizationParameters tOptParams;
    tOptParams.append("concurrent_evaluations", "2");
    tMetaData.set(tOptParams);
    // call function
    XMLGen::OperationMetaData tOperationMetaData;
    XMLGen::pre_process_general_harvest_data_from_file_operation_options(tMetaData, tOperationMetaData);
    EXPECT_STREQ("2", tOperationMetaData.get("concurrent_evaluations").front().c_str());
    EXPECT_EQ(2u, tOperationMetaData.get("names").size());
    EXPECT_STREQ("harvest_data_0", tOperationMetaData.get("names")[0].c_str());
    EXPECT_STREQ("harvest_data_1", tOperationMetaData.get("names")[1].c_str());
}

TEST(PlatoTestXMLGenerator, write_run_system_call_operation_matched_power_balance)
{
    XMLGen::InputData tMetaData;
    // define service
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tServiceOne.append("type", "system_call");
    tServiceOne.append("number_processors", "1");
    tMetaData.append(tServiceOne);
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "platomain");
    tServiceTwo.append("id", "2");
    tServiceTwo.append("type", "plato_app");
    tServiceTwo.append("number_processors", "2");
    tMetaData.append(tServiceTwo);
    // define optimization parameters 
    XMLGen::OptimizationParameters tOptParams;
    tOptParams.append("concurrent_evaluations", "2");
    tMetaData.set(tOptParams);

    pugi::xml_document tDocument;
    XMLGen::matched_power_balance::write_run_system_call_operation("gemma_evaluation", tMetaData, tDocument);
    tDocument.save_file("dummy.txt", " ");

    auto tReadData = XMLGen::read_data_from_file("dummy.txt");
    auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>SystemCallMPI</Function><Name>gemma_0</Name><Command>gemma</Command><OnChange>true</OnChange><AppendInput>false</AppendInput><ChDir>gemma_evaluation_0/</ChDir><Argument>gemma_matched_power_balance_input_deck.yaml</Argument></Operation><Operation><Function>SystemCallMPI</Function><Name>gemma_1</Name><Command>gemma</Command><OnChange>true</OnChange><AppendInput>false</AppendInput><ChDir>gemma_evaluation_1/</ChDir><Argument>gemma_matched_power_balance_input_deck.yaml</Argument></Operation>");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
    Plato::system("rm -f dummy.txt");
}

TEST(PlatoTestXMLGenerator, write_run_system_call_operation)
{
    pugi::xml_document tDocument;
    XMLGen::OperationMetaData tOperationMetaData;
    
    tOperationMetaData.append("concurrent_evaluations", "2");
    tOperationMetaData.append("names", "gemma_0");
    tOperationMetaData.append("names", "gemma_1");
    tOperationMetaData.append("commands", "curl");
    tOperationMetaData.append("commands", "curl");
    tOperationMetaData.append("functions", "SystemCall");
    tOperationMetaData.append("functions", "SystemCall");
    tOperationMetaData.append("subdirectories", "evaluation_0");
    tOperationMetaData.append("subdirectories", "evaluation_1");
    tOperationMetaData.append("arguments", "http://localhost:7000/rungemma/");
    tOperationMetaData.append("arguments", "http://localhost:7001/rungemma/");

    XMLGen::write_run_system_call_operation(tOperationMetaData, tDocument);
    tDocument.save_file("dummy.txt", " ");

    auto tReadData = XMLGen::read_data_from_file("dummy.txt");
    auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>SystemCall</Function><Name>gemma_0</Name><Command>curl</Command><OnChange>true</OnChange><AppendInput>false</AppendInput><ChDir>evaluation_0</ChDir><Argument>http://localhost:7000/rungemma/</Argument></Operation><Operation><Function>SystemCall</Function><Name>gemma_1</Name><Command>curl</Command><OnChange>true</OnChange><AppendInput>false</AppendInput><ChDir>evaluation_1</ChDir><Argument>http://localhost:7001/rungemma/</Argument></Operation>");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
    Plato::system("rm -f dummy.txt");
}

TEST(PlatoTestXMLGenerator, set_run_system_call_shared_data_arguments)
{
    XMLGen::OperationMetaData tOperationMetaDataOne;
    tOperationMetaDataOne.append("algorithm", "matched_power_balance");
    tOperationMetaDataOne.append("type", "web_app");
    tOperationMetaDataOne.append("type", "web_app");
    XMLGen::set_run_system_call_shared_data_arguments(tOperationMetaDataOne);
    // test web_app arguments
    EXPECT_EQ(2u, tOperationMetaDataOne.get("arguments").size());
    EXPECT_STREQ("http://localhost:7000/rungemma/", tOperationMetaDataOne.get("arguments").front().c_str());
    EXPECT_STREQ("http://localhost:7001/rungemma/", tOperationMetaDataOne.get("arguments").back().c_str());
}

TEST(PlatoTestXMLGenerator, pre_process_general_run_system_call_options)
{
    XMLGen::InputData tMetaData;
    // define service
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tServiceOne.append("type", "web_app");
    tServiceOne.append("number_processors", "1");
    tMetaData.append(tServiceOne);
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "platomain");
    tServiceTwo.append("id", "2");
    tServiceTwo.append("type", "plato_app");
    tServiceTwo.append("number_processors", "2");
    tMetaData.append(tServiceTwo);
    // define optimization parameters 
    XMLGen::OptimizationParameters tOptParams;
    tOptParams.append("concurrent_evaluations", "3");
    tMetaData.set(tOptParams);

    XMLGen::OperationMetaData tOperationMetaData;
    tOperationMetaData.append("code", "gemma");
    EXPECT_NO_THROW(XMLGen::pre_process_general_run_system_call_options(tMetaData, tOperationMetaData));

    // test output
    auto tKeys = tOperationMetaData.keys();
    EXPECT_EQ(6u, tKeys.size());
    std::vector<std::string> tGoldKeys = {"functions", "number_processors", "commands", "type", "code", "concurrent_evaluations"};
    for(auto tKey : tKeys)
    {
        EXPECT_TRUE(std::find(tGoldKeys.begin(), tGoldKeys.end(), tKey) != tGoldKeys.end());
    }
    // test concurrent_evaluations
    EXPECT_EQ(1u, tOperationMetaData.get("concurrent_evaluations").size());
    EXPECT_STREQ("3", tOperationMetaData.get("concurrent_evaluations").front().c_str());    
    // test functions
    EXPECT_EQ(3u, tOperationMetaData.get("functions").size());
    EXPECT_STREQ("SystemCall", tOperationMetaData.get("functions")[0].c_str());
    EXPECT_STREQ("SystemCall", tOperationMetaData.get("functions")[1].c_str());
    EXPECT_STREQ("SystemCall", tOperationMetaData.get("functions")[2].c_str());
    // test number processors
    EXPECT_EQ(3u, tOperationMetaData.get("number_processors").size());
    EXPECT_STREQ("1", tOperationMetaData.get("number_processors")[0].c_str());
    EXPECT_STREQ("1", tOperationMetaData.get("number_processors")[1].c_str());
    EXPECT_STREQ("1", tOperationMetaData.get("number_processors")[2].c_str());
    // test commands
    EXPECT_EQ(3u, tOperationMetaData.get("commands").size());
    EXPECT_STREQ("curl", tOperationMetaData.get("commands")[0].c_str());
    EXPECT_STREQ("curl", tOperationMetaData.get("commands")[1].c_str());
    EXPECT_STREQ("curl", tOperationMetaData.get("commands")[2].c_str());
    // test type
    EXPECT_EQ(3u, tOperationMetaData.get("type").size());
    EXPECT_STREQ("web_app", tOperationMetaData.get("type")[0].c_str());
    EXPECT_STREQ("web_app", tOperationMetaData.get("type")[1].c_str());
    EXPECT_STREQ("web_app", tOperationMetaData.get("type")[2].c_str());
}

TEST(PlatoTestXMLGenerator, set_run_system_call_option)
{
    XMLGen::OperationMetaData tOperationMetaDataOne;
    tOperationMetaDataOne.append("concurrent_evaluations", "3");

    // test one: web_app options
    tOperationMetaDataOne.set("type", std::vector<std::string>(3, "web_app"));
    XMLGen::set_run_system_call_option("commands", tOperationMetaDataOne);
    EXPECT_EQ(3u, tOperationMetaDataOne.get("commands").size());
    EXPECT_STREQ("curl", tOperationMetaDataOne.get("commands")[0].c_str());
    EXPECT_STREQ("curl", tOperationMetaDataOne.get("commands")[1].c_str());
    EXPECT_STREQ("curl", tOperationMetaDataOne.get("commands")[2].c_str());

    XMLGen::set_run_system_call_option("functions", tOperationMetaDataOne);
    EXPECT_EQ(3u, tOperationMetaDataOne.get("functions").size());
    EXPECT_STREQ("SystemCall", tOperationMetaDataOne.get("functions")[0].c_str());
    EXPECT_STREQ("SystemCall", tOperationMetaDataOne.get("functions")[1].c_str());
    EXPECT_STREQ("SystemCall", tOperationMetaDataOne.get("functions")[2].c_str());
    
    EXPECT_THROW(XMLGen::set_run_system_call_option("ranks", tOperationMetaDataOne), std::runtime_error); // error: key not defined

    // test two: system_call options
    XMLGen::OperationMetaData tOperationMetaDataTwo;
    tOperationMetaDataTwo.append("concurrent_evaluations", "3");
    tOperationMetaDataTwo.set("type", std::vector<std::string>(3, "system_call"));
    XMLGen::set_run_system_call_option("commands", tOperationMetaDataTwo);
    EXPECT_EQ(3u, tOperationMetaDataTwo.get("commands").size());
    EXPECT_STREQ("gemma", tOperationMetaDataTwo.get("commands")[0].c_str());
    EXPECT_STREQ("gemma", tOperationMetaDataTwo.get("commands")[1].c_str());
    EXPECT_STREQ("gemma", tOperationMetaDataTwo.get("commands")[2].c_str());

    XMLGen::set_run_system_call_option("functions", tOperationMetaDataTwo);
    EXPECT_EQ(3u, tOperationMetaDataTwo.get("functions").size());
    EXPECT_STREQ("SystemCallMPI", tOperationMetaDataTwo.get("functions")[0].c_str());
    EXPECT_STREQ("SystemCallMPI", tOperationMetaDataTwo.get("functions")[1].c_str());
    EXPECT_STREQ("SystemCallMPI", tOperationMetaDataTwo.get("functions")[2].c_str());
}

TEST(PlatoTestXMLGenerator, check_plato_app_service_type)
{
    EXPECT_THROW(XMLGen::check_plato_app_service_type("plato_app"), std::runtime_error);
    EXPECT_NO_THROW(XMLGen::check_plato_app_service_type("web_app"));
    EXPECT_NO_THROW(XMLGen::check_plato_app_service_type("system_call"));
}

TEST(PlatoTestXMLGenerator, set_system_call_option_from_service)
{
    XMLGen::InputData tMetaData;
    // define service
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tServiceOne.append("type", "web_app");
    tServiceOne.append("number_processors", "1");
    tMetaData.append(tServiceOne);
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "platomain");
    tServiceTwo.append("id", "2");
    tServiceTwo.append("type", "plato_app");
    tServiceTwo.append("number_processors", "2");
    tMetaData.append(tServiceTwo);

    XMLGen::OperationMetaData tOperationMetaData;
    tOperationMetaData.append("code", "gemma");
    tOperationMetaData.append("concurrent_evaluations", "3");

    // test one: one service defined 
    XMLGen::set_system_call_option_from_service("type", tMetaData, tOperationMetaData);
    EXPECT_EQ(3u, tOperationMetaData.get("type").size());
    EXPECT_STREQ("web_app", tOperationMetaData.get("type")[0].c_str());
    EXPECT_STREQ("web_app", tOperationMetaData.get("type")[1].c_str());
    EXPECT_STREQ("web_app", tOperationMetaData.get("type")[2].c_str());

    XMLGen::set_system_call_option_from_service("number_processors", tMetaData, tOperationMetaData);
    EXPECT_EQ(3u, tOperationMetaData.get("number_processors").size());
    EXPECT_STREQ("1", tOperationMetaData.get("number_processors")[0].c_str());
    EXPECT_STREQ("1", tOperationMetaData.get("number_processors")[1].c_str());
    EXPECT_STREQ("1", tOperationMetaData.get("number_processors")[2].c_str());

    // test two: number of services mathc the number of concurrent evaluations
    XMLGen::Service tServiceThree;
    tServiceThree.append("code", "gemma");
    tServiceThree.append("id", "3");
    tServiceThree.append("type", "web_app");
    tServiceThree.append("number_processors", "1");
    tMetaData.append(tServiceThree);
    XMLGen::Service tServiceFour;
    tServiceFour.append("code", "gemma");
    tServiceFour.append("id", "4");
    tServiceFour.append("type", "system_call");
    tServiceFour.append("number_processors", "2");
    tMetaData.append(tServiceFour);

    XMLGen::set_system_call_option_from_service("type", tMetaData, tOperationMetaData);
    EXPECT_EQ(3u, tOperationMetaData.get("type").size());
    EXPECT_STREQ("web_app", tOperationMetaData.get("type")[0].c_str());
    EXPECT_STREQ("web_app", tOperationMetaData.get("type")[1].c_str());
    EXPECT_STREQ("system_call", tOperationMetaData.get("type")[2].c_str());

    XMLGen::set_system_call_option_from_service("number_processors", tMetaData, tOperationMetaData);
    EXPECT_EQ(3u, tOperationMetaData.get("number_processors").size());
    EXPECT_STREQ("1", tOperationMetaData.get("number_processors")[0].c_str());
    EXPECT_STREQ("1", tOperationMetaData.get("number_processors")[1].c_str());
    EXPECT_STREQ("2", tOperationMetaData.get("number_processors")[2].c_str());

    // test three: error -> the number of services exceeds the number of concurrent evaluations
    XMLGen::Service tServiceFive;
    tServiceFive.append("code", "gemma");
    tServiceFive.append("id", "5");
    tServiceFive.append("type", "system_call");
    tServiceFive.append("number_processors", "2");
    tMetaData.append(tServiceFive);

    EXPECT_THROW(XMLGen::set_system_call_option_from_service("type", tMetaData, tOperationMetaData), std::runtime_error);
}

TEST(PlatoTestXMLGenerator, get_values_from_service)
{
    XMLGen::InputData tMetaData;
    // define service
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tServiceOne.append("type", "web_app");
    tServiceOne.append("number_processors", "1");
    tMetaData.append(tServiceOne);
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "platomain");
    tServiceTwo.append("id", "2");
    tServiceTwo.append("type", "plato_app");
    tServiceTwo.append("number_processors", "2");
    tMetaData.append(tServiceTwo);

    // test gemma 
    auto tValues = XMLGen::get_values_from_service("gemma", "number_processors", tMetaData);
    EXPECT_EQ(1u, tValues.size());
    EXPECT_STREQ("1", tValues.front().c_str());
    tValues = XMLGen::get_values_from_service("GEMMA", "TYPE", tMetaData);
    EXPECT_EQ(1u, tValues.size());
    EXPECT_STREQ("web_app", tValues.front().c_str());

    // test platomain 
    tValues = XMLGen::get_values_from_service("platomain", "number_processors", tMetaData);
    EXPECT_EQ(1u, tValues.size());
    EXPECT_STREQ("2", tValues.front().c_str());
    tValues = XMLGen::get_values_from_service("Platomain", "Type", tMetaData);
    EXPECT_EQ(1u, tValues.size());
    EXPECT_STREQ("plato_app", tValues.front().c_str());
}

TEST(PlatoTestXMLGenerator, get_service_indices)
{
    XMLGen::InputData tMetaData;
    // define service
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tMetaData.append(tServiceOne);
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "platomain");
    tServiceTwo.append("id", "2");
    tMetaData.append(tServiceTwo);

    // test gemma indices
    auto tIndices = XMLGen::get_service_indices("gemma", tMetaData);
    EXPECT_EQ(1u, tIndices.size());
    EXPECT_EQ(0, tIndices.front());

    // test plato indices
    tIndices = XMLGen::get_service_indices("PLATOMAIN", tMetaData);
    EXPECT_EQ(1u, tIndices.size());
    EXPECT_EQ(1, tIndices.front());
}

TEST(PlatoTestXMLGenerator, aprepro_system_call_matched_power_balance)
{
    XMLGen::InputData tMetaData;
    // define optmization parameters
    XMLGen::OptimizationParameters tOptParams;
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    tOptParams.descriptors(tDescriptors);
    tOptParams.append("concurrent_evaluations", "3");
    tMetaData.set(tOptParams);

    pugi::xml_document tDocument;
    XMLGen::matched_power_balance::write_aprepro_system_call_operation("gemma_evaluation", tMetaData, tDocument);
    tDocument.save_file("dummy.txt");

    auto tReadData = XMLGen::read_data_from_file("dummy.txt");
    auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>SystemCall</Function><Name>aprepro_0</Name><Command>aprepro</Command><ChDir>gemma_evaluation_0/</ChDir><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>gemma_matched_power_balance_input_deck.yaml.template</Argument><Argument>gemma_matched_power_balance_input_deck.yaml</Argument><Option>slot_length</Option><Option>slot_width</Option><Option>slot_depth</Option><Input><ArgumentName>parameters_0</ArgumentName><Layout>scalar</Layout><Size>3</Size></Input></Operation><Operation><Function>SystemCall</Function><Name>aprepro_1</Name><Command>aprepro</Command><ChDir>gemma_evaluation_1/</ChDir><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>gemma_matched_power_balance_input_deck.yaml.template</Argument><Argument>gemma_matched_power_balance_input_deck.yaml</Argument><Option>slot_length</Option><Option>slot_width</Option><Option>slot_depth</Option><Input><ArgumentName>parameters_1</ArgumentName><Layout>scalar</Layout><Size>3</Size></Input></Operation><Operation><Function>SystemCall</Function><Name>aprepro_2</Name><Command>aprepro</Command><ChDir>gemma_evaluation_2/</ChDir><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>gemma_matched_power_balance_input_deck.yaml.template</Argument><Argument>gemma_matched_power_balance_input_deck.yaml</Argument><Option>slot_length</Option><Option>slot_width</Option><Option>slot_depth</Option><Input><ArgumentName>parameters_2</ArgumentName><Layout>scalar</Layout><Size>3</Size></Input></Operation>");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
    Plato::system("rm -f dummy.txt");
}

TEST(PlatoTestXMLGenerator, are_aprepro_input_options_defined)
{
    XMLGen::OperationMetaData tOperationMetaData1;
    tOperationMetaData1.append("concurrent_evaluations", "2");
    tOperationMetaData1.set("names", {"aprepro_0", "aprepro_1"});
    tOperationMetaData1.set("options", {"length=", "depth=", "width="});
    tOperationMetaData1.set("commands", std::vector<std::string>(2u, "aprepro"));
    tOperationMetaData1.set("functions", std::vector<std::string>(2u, "SystemCall"));
    tOperationMetaData1.set("arguments", {"-q", "matched.yaml.template", "matched.yaml"});
    EXPECT_NO_THROW(XMLGen::are_aprepro_input_options_defined(tOperationMetaData1));

    XMLGen::OperationMetaData tOperationMetaData2;
    tOperationMetaData2.set("names", {"aprepro_0", "aprepro_1"});
    tOperationMetaData2.set("options", {"length=", "depth=", "width="});
    tOperationMetaData2.set("commands", std::vector<std::string>(2u, "aprepro"));
    tOperationMetaData2.set("functions", std::vector<std::string>(2u, "SystemCall"));
    tOperationMetaData2.set("arguments", {"-q", "matched.yaml.template", "matched.yaml"});
    EXPECT_THROW(XMLGen::are_aprepro_input_options_defined(tOperationMetaData2), std::runtime_error);
}

TEST(PlatoTestXMLGenerator, write_aprepro_system_call_operation)
{
    XMLGen::OperationMetaData tOperationMetaData;
    tOperationMetaData.append("concurrent_evaluations", "2");
    tOperationMetaData.set("names", {"aprepro_0", "aprepro_1"});
    tOperationMetaData.set("options", {"length=", "depth=", "width="});
    tOperationMetaData.set("subdirectories", {"evaluation_0", "evaluation_1"});
    tOperationMetaData.set("commands", std::vector<std::string>(2u, "aprepro"));
    tOperationMetaData.set("functions", std::vector<std::string>(2u, "SystemCall"));
    tOperationMetaData.set("arguments", {"-q", "matched.yaml.template", "matched.yaml"});

    std::vector<XMLGen::OperationArgument> tInputs;
    XMLGen::OperationArgument tInputOne;
    tInputOne.set("size", "3");
    tInputOne.set("type", "input");
    tInputOne.set("layout", "scalars");
    tInputOne.set("name", "parameters_0");
    tInputs.push_back(tInputOne);
    XMLGen::OperationArgument tInputTwo;
    tInputTwo.set("size", "3");
    tInputTwo.set("type", "input");
    tInputTwo.set("layout", "scalars");
    tInputTwo.set("name", "parameters_1");
    tInputs.push_back(tInputTwo);

    pugi::xml_document tDocument;
    XMLGen::write_aprepro_system_call_operation(tInputs, tOperationMetaData, tDocument);
    tDocument.save_file("dummy.txt");

    auto tReadData = XMLGen::read_data_from_file("dummy.txt");
    auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>SystemCall</Function><Name>aprepro_0</Name><Command>aprepro</Command><ChDir>evaluation_0</ChDir><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>matched.yaml.template</Argument><Argument>matched.yaml</Argument><Option>length=</Option><Option>depth=</Option><Option>width=</Option><Input><ArgumentName>parameters_0</ArgumentName><Layout>scalars</Layout><Size>3</Size></Input></Operation><Operation><Function>SystemCall</Function><Name>aprepro_1</Name><Command>aprepro</Command><ChDir>evaluation_1</ChDir><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>matched.yaml.template</Argument><Argument>matched.yaml</Argument><Option>length=</Option><Option>depth=</Option><Option>width=</Option><Input><ArgumentName>parameters_1</ArgumentName><Layout>scalars</Layout><Size>3</Size></Input></Operation>");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
    Plato::system("rm -f dummy.txt");
}

TEST(PlatoTestXMLGenerator, pre_process_aprepro_system_call_inputs)
{
    // define optmization parameters
    XMLGen::OperationMetaData tOperationMetaData;
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    tOperationMetaData.set("descriptors", tDescriptors);
    tOperationMetaData.append("concurrent_evaluations", "3");

    std::vector<XMLGen::OperationArgument> tData;
    XMLGen::pre_process_aprepro_system_call_inputs(tOperationMetaData, tData);
    EXPECT_EQ(3u, tData.size());

    EXPECT_STREQ("3", tData[0].get("size").c_str());
    EXPECT_STREQ("input", tData[0].get("type").c_str());
    EXPECT_STREQ("scalar", tData[0].get("layout").c_str());
    EXPECT_STREQ("parameters_0", tData[0].get("name").c_str());

    EXPECT_STREQ("3", tData[1].get("size").c_str());
    EXPECT_STREQ("input", tData[1].get("type").c_str());
    EXPECT_STREQ("scalar", tData[1].get("layout").c_str());
    EXPECT_STREQ("parameters_1", tData[1].get("name").c_str());

    EXPECT_STREQ("3", tData[2].get("size").c_str());
    EXPECT_STREQ("input", tData[2].get("type").c_str());
    EXPECT_STREQ("scalar", tData[2].get("layout").c_str());
    EXPECT_STREQ("parameters_2", tData[2].get("name").c_str());
}

TEST(PlatoTestXMLGenerator, append_integer_to_option)
{
    XMLGen::OperationMetaData tOperationMetaData;
    tOperationMetaData.append("concurrent_evaluations", "3");
    XMLGen::append_integer_to_option("names", "aprepro_", tOperationMetaData);

    auto tOutput = tOperationMetaData.get("names");
    EXPECT_EQ(3u, tOutput.size());
    EXPECT_STREQ("aprepro_0", tOutput[0].c_str());
    EXPECT_STREQ("aprepro_1", tOutput[1].c_str());
    EXPECT_STREQ("aprepro_2", tOutput[2].c_str());
}

TEST(PlatoTestXMLGenerator, preprocess_aprepro_system_call_options_matched_power_balance)
{
    XMLGen::InputData tMetaData;
    // define optmization parameters
    XMLGen::OptimizationParameters tOptParams;
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    tOptParams.descriptors(tDescriptors);
    tOptParams.append("concurrent_evaluations", "3");
    tMetaData.set(tOptParams);

    XMLGen::OperationMetaData tOperationMetaData;
    XMLGen::matched_power_balance::preprocess_aprepro_system_call_options(tMetaData, tOperationMetaData);

    auto tMyDescriptors = tOperationMetaData.get("descriptors");
    EXPECT_EQ(3u, tMyDescriptors.size());
    EXPECT_STREQ(tDescriptors[0].c_str(), tMyDescriptors[0].c_str());
    EXPECT_STREQ(tDescriptors[1].c_str(), tMyDescriptors[1].c_str());
    EXPECT_STREQ(tDescriptors[2].c_str(), tMyDescriptors[2].c_str());

    EXPECT_STREQ("3", tOperationMetaData.get("concurrent_evaluations").front().c_str());
    EXPECT_STREQ("gemma_matched_power_balance_input_deck.yaml", tOperationMetaData.get("input_file").front().c_str());
    EXPECT_STREQ("gemma_matched_power_balance_input_deck.yaml.template", tOperationMetaData.get("template_file").front().c_str());
}

TEST(PlatoTestXMLGenerator, write_gemma_input_deck_matched_power_balance)
{
    XMLGen::InputData tInputMetaData;
    // define optmization parameters
    XMLGen::OptimizationParameters tOptParams;
    tOptParams.append("concurrent_evaluations", "2");
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    tOptParams.descriptors(tDescriptors);
    tInputMetaData.set(tOptParams);

    // define objective 
    XMLGen::Objective tObjective;
    tObjective.type = "weighted_sum";
    tObjective.criteriaIDs.push_back("1");
    tObjective.criteriaIDs.push_back("2");
    tObjective.serviceIDs.push_back("1");
    tObjective.serviceIDs.push_back("2");
    tObjective.scenarioIDs.push_back("1");
    tObjective.scenarioIDs.push_back("2");
    tInputMetaData.objective = tObjective;

    // define service
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tInputMetaData.append(tServiceOne);
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "platomain");
    tServiceTwo.append("id", "2");
    tInputMetaData.append(tServiceTwo);

    // define scenario
    XMLGen::Scenario tScenario;
    tScenario.append("id", "1");
    tScenario.append("material", "1");
    tScenario.append("frequency_min", "10");
    tScenario.append("frequency_max", "100");
    tScenario.append("frequency_step", "5");
    tScenario.append("cavity_radius", "0.1016");
    tScenario.append("cavity_height", "0.1018");
    tScenario.append("physics", "electromagnetics");
    tInputMetaData.append(tScenario);

    // define material
    XMLGen::Material tMaterial;
    tMaterial.id("1");
    tMaterial.property("conductivity", "1e6");
    tInputMetaData.append(tMaterial);

    // TEST 1: WRITE TEMPLATE FILE NEEDED FOR APREPRO OPERATION
    XMLGen::gemma::write_input_deck(tInputMetaData);
    auto tReadDataEvalDirZero = XMLGen::read_data_from_file("./gemma_evaluation_0/gemma_matched_power_balance_input_deck.yaml.template");
    auto tReadDataEvalDirOne = XMLGen::read_data_from_file("./gemma_evaluation_1/gemma_matched_power_balance_input_deck.yaml.template");
    std::string tGoldString = std::string("%YAML1.1---Gemma-dynamic:Global:Description:HigginscylinderSolutiontype:powerbalancePowerbalance:Algorithm:matchedboundRadius:0.1016Height:0.1018Conductivity:1e6Slotlength:{slot_length}Slotwidth:{slot_width}Slotdepth:{slot_depth}Startfrequencyrange:10Endfrequencyrange:100Frequencyintervalsize:5...");
    ASSERT_STREQ(tGoldString.c_str(), tReadDataEvalDirZero.str().c_str());
    ASSERT_STREQ(tGoldString.c_str(), tReadDataEvalDirOne.str().c_str());
    Plato::system("rm -rf gemma_evaluation_0/ gemma_evaluation_1/");

    // TEST 2: ERROR - PARAMETER VALUE WAS NOT DEFINED
    tDescriptors.clear();
    tOptParams.descriptors(tDescriptors);
    tInputMetaData.set(tOptParams);
    ASSERT_THROW(XMLGen::gemma::write_input_deck(tInputMetaData), std::runtime_error);

    // TEST 3: WRITE REGULAR INPUT DECK
    tScenario.append("slot_width", "0.0118");
    tScenario.append("slot_depth", "0.00218");
    tScenario.append("slot_length", "0.0116");
    std::vector<XMLGen::Scenario> tScenarios;
    tScenarios.push_back(tScenario);
    tInputMetaData.set(tScenarios);
    tOptParams.set("concurrent_evaluations", {""} );
    tInputMetaData.set(tOptParams);
    XMLGen::gemma::write_input_deck(tInputMetaData);
    tReadDataEvalDirZero = XMLGen::read_data_from_file("./gemma_evaluation_0/gemma_matched_power_balance_input_deck.yaml");
    tGoldString = std::string("%YAML1.1---Gemma-dynamic:Global:Description:HigginscylinderSolutiontype:powerbalancePowerbalance:Algorithm:matchedboundRadius:0.1016Height:0.1018Conductivity:1e6Slotlength:0.0116Slotwidth:0.0118Slotdepth:0.00218Startfrequencyrange:10Endfrequencyrange:100Frequencyintervalsize:5...");
    ASSERT_STREQ(tGoldString.c_str(), tReadDataEvalDirZero.str().c_str());
    Plato::system("rm -rf gemma_evaluation_0/");
}

TEST(PlatoTestXMLGenerator, matched_power_balance_write_input_deck_to_file)
{
    std::unordered_map<std::string,std::string> tMap;
    tMap["slot_width"] = "0.1";
    tMap["slot_depth"] = "0.2";
    tMap["slot_length"] = "0.3";
    tMap["cavity_radius"] = "1";
    tMap["cavity_height"] = "2";
    tMap["frequency_min"] = "10";
    tMap["frequency_max"] = "100";
    tMap["frequency_step"] = "5";
    tMap["conductivity"] = "11";
    XMLGen::matched_power_balance::write_input_deck_to_file("input_deck.yaml",tMap);
    auto tReadData = XMLGen::read_data_from_file("input_deck.yaml");
    std::string tGoldString = std::string("%YAML1.1---Gemma-dynamic:Global:Description:HigginscylinderSolutiontype:powerbalancePowerbalance:Algorithm:matchedboundRadius:1Height:2Conductivity:11Slotlength:0.3Slotwidth:0.1Slotdepth:0.2Startfrequencyrange:10Endfrequencyrange:100Frequencyintervalsize:5...");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
    Plato::system("rm -f input_deck.yaml");
}

TEST(PlatoTestXMLGenerator, matched_power_balance_get_input_key_value_pairs)
{
    XMLGen::Scenario tScenario;
    tScenario.append("frequency_min", "10");
    tScenario.append("frequency_max", "100");
    tScenario.append("frequency_step", "5");
    tScenario.append("cavity_radius", "0.1016");
    tScenario.append("cavity_height", "0.1018");

    XMLGen::Material tMaterial;
    tMaterial.property("conductivity", "1e6");

    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    auto tPairs = XMLGen::matched_power_balance::get_input_key_value_pairs(tScenario, tMaterial, tDescriptors);
    EXPECT_EQ(9u, tPairs.size());
    EXPECT_STREQ("10", tPairs.at("frequency_min").c_str());
    EXPECT_STREQ("5", tPairs.at("frequency_step").c_str());
    EXPECT_STREQ("1e6", tPairs.at("conductivity").c_str());
    EXPECT_STREQ("100", tPairs.at("frequency_max").c_str());
    EXPECT_STREQ("0.1016", tPairs.at("cavity_radius").c_str());
    EXPECT_STREQ("0.1018", tPairs.at("cavity_height").c_str());
    EXPECT_STREQ("{slot_width}", tPairs.at("slot_width").c_str());
    EXPECT_STREQ("{slot_depth}", tPairs.at("slot_depth").c_str());
    EXPECT_STREQ("{slot_length}", tPairs.at("slot_length").c_str());
}

TEST(PlatoTestXMLGenerator, set_descriptor_value)
{
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_height"};
    auto tOutput = XMLGen::set_descriptor_value("slot_length", "0.01", tDescriptors);
    EXPECT_STREQ("{slot_length}", tOutput.c_str());

    tOutput = XMLGen::set_descriptor_value("slot_width", "0.01", tDescriptors);
    EXPECT_STREQ("{slot_width}", tOutput.c_str());

    tOutput = XMLGen::set_descriptor_value("slot_height", "0.01", tDescriptors);
    EXPECT_STREQ("{slot_height}", tOutput.c_str());

    tOutput = XMLGen::set_descriptor_value("conductivity", "0.01", tDescriptors);
    EXPECT_STREQ("0.01", tOutput.c_str());
}

TEST(PlatoTestXMLGenerator, get_unique_scenario_ids)
{
    XMLGen::InputData tMetaData;

    // TEST 1: target scenario defined
    // define objective
    XMLGen::Objective tObjective;
    tObjective.type = "weighted_sum";
    tObjective.criteriaIDs.push_back("1");
    tObjective.serviceIDs.push_back("1");
    tObjective.scenarioIDs.push_back("1");
    tMetaData.objective = tObjective;

    // define service
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tMetaData.append(tServiceOne);

    // call get_unique_scenario_ids
    auto tUniqueGemmaScenarioIDs = XMLGen::get_unique_scenario_ids("gemma", tMetaData);
    EXPECT_EQ(1u, tUniqueGemmaScenarioIDs.size());
    EXPECT_STREQ("1", tUniqueGemmaScenarioIDs.front().c_str());
    auto tUniquePAScenarioIDs = XMLGen::get_unique_scenario_ids("plato_analyze", tMetaData);
    EXPECT_TRUE(tUniquePAScenarioIDs.empty());

    // TEST 2: one target scenario and two distinct criteria, scenarios & services defined 
    // define objective: add new criteria, scenario, and service info to objective
    tMetaData.objective.criteriaIDs.push_back("2");
    tMetaData.objective.serviceIDs.push_back("2");
    tMetaData.objective.scenarioIDs.push_back("2");
    EXPECT_EQ(2u, tMetaData.objective.criteriaIDs.size());

    // define objective
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "plato_analyze");
    tServiceTwo.append("id", "2");
    tMetaData.append(tServiceTwo);
    EXPECT_EQ(2u, tMetaData.services().size());

    // call get_unique_scenario_ids
    tUniqueGemmaScenarioIDs = XMLGen::get_unique_scenario_ids("gemma", tMetaData);
    EXPECT_EQ(1u, tUniqueGemmaScenarioIDs.size());
    EXPECT_STREQ("1", tUniqueGemmaScenarioIDs.front().c_str());
    tUniquePAScenarioIDs = XMLGen::get_unique_scenario_ids("plato_analyze", tMetaData);
    EXPECT_EQ(1u, tUniquePAScenarioIDs.size());
    EXPECT_STREQ("2", tUniquePAScenarioIDs.front().c_str());

    // TEST 3: one target scenario, two distinct services and three criterion defined
    tMetaData.objective.criteriaIDs.push_back("3");
    tMetaData.objective.serviceIDs.push_back("1");
    tMetaData.objective.scenarioIDs.push_back("1");
    EXPECT_EQ(3u, tMetaData.objective.criteriaIDs.size());
    EXPECT_EQ(2u, tMetaData.services().size());

    // call get_unique_scenario_ids
    tUniqueGemmaScenarioIDs = XMLGen::get_unique_scenario_ids("gemma", tMetaData);
    EXPECT_EQ(1u, tUniqueGemmaScenarioIDs.size());
    EXPECT_STREQ("1", tUniqueGemmaScenarioIDs.front().c_str());
    tUniquePAScenarioIDs = XMLGen::get_unique_scenario_ids("plato_analyze", tMetaData);
    EXPECT_EQ(1u, tUniquePAScenarioIDs.size());
    EXPECT_STREQ("2", tUniquePAScenarioIDs.front().c_str());

    // TEST 4: two distinct services, four criterion and three scenarios defined
    tMetaData.objective.criteriaIDs.push_back("4");
    tMetaData.objective.serviceIDs.push_back("1");
    tMetaData.objective.scenarioIDs.push_back("3");
    EXPECT_EQ(4u, tMetaData.objective.criteriaIDs.size());
    EXPECT_EQ(2u, tMetaData.services().size());

    // call get_unique_scenario_ids
    tUniqueGemmaScenarioIDs = XMLGen::get_unique_scenario_ids("gemma", tMetaData);
    EXPECT_EQ(2u, tUniqueGemmaScenarioIDs.size());
    EXPECT_STREQ("1", tUniqueGemmaScenarioIDs.front().c_str());
    EXPECT_STREQ("3", tUniqueGemmaScenarioIDs.back().c_str());
    tUniquePAScenarioIDs = XMLGen::get_unique_scenario_ids("plato_analyze", tMetaData);
    EXPECT_EQ(1u, tUniquePAScenarioIDs.size());
    EXPECT_STREQ("2", tUniquePAScenarioIDs.front().c_str());
}

TEST(PlatoTestXMLGenerator, ParseOperationMetaDataCriterion)
{

    std::string tStringInput =
        "begin optimization_parameters\n"
        "  optimization_type dakota\n"
        "  dakota_workflow mdps\n"
        "  concurrent_evaluations 3\n"
        "  lower_bounds       0.01       0.001       0.002\n"
        "  upper_bounds       0.10       0.010       0.020\n"
        "  descriptors     slot_length slot_width slot_height\n"
        "  mdps_partitions     2           3          4\n"
        "  mdps_response_functions 1\n"
        "end optimization_parameters\n"
        "begin criterion MyOperationMetaData\n"
        "  type system_call\n"
        "  aprepro_template_name input.yaml.template"
        "  command gemma\n"
        "  arguments input.yaml\n"
        "  data_file output.dat\n"
        "  data column_1\n"
        "  data_name shielding_effectiveness\n"
        "  data_extraction_operation max\n"
        "end criterion\n";
    std::istringstream tInputSS;
    tInputSS.str(tStringInput);
}

}
// PlatoTestXMLGenerator