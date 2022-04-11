/*
 * XMLGeneratorGemmaUtilities.hpp
 *
 *  Created on: March 23, 2022
 */

#include "XMLG_Macros.hpp"
#include "XMLGeneratorDataStruct.hpp"
#include "XMLGeneratorGemmaUtilities.hpp"
#include "XMLGeneratorOperationsFileUtilities.hpp"
#include "XMLGeneratorGemmaMatchedPowerBalanceUtilities.hpp"

namespace XMLGen
{

namespace gemma
{

/*****************************************************************************************************/
/*********************************************** Files ***********************************************/
/*****************************************************************************************************/

Files::Files()
{
    this->build();
}

std::string Files::filename(const std::string& aSolverType, const std::string& aFileFormat) const
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

void Files::build()
{
    mMap["matched_power_balance"]["output"] = std::string("matched_power_balance.dat");
    mMap["matched_power_balance"]["input"] = std::string("gemma_matched_power_balance_input_deck.yaml");

    mMap["unmatched_power_balance"]["output"] = std::string("unmatched_power_balance.dat");
    mMap["unmatched_power_balance"]["input"] = std::string("gemma_unmatched_power_balance_input_deck.yaml");
}

/*****************************************************************************************************/
/********************************************** Options **********************************************/
/*****************************************************************************************************/

Options::Options()
{
    this->build();
}

const std::string& Options::get(const std::string& aOuterKey, const std::string& aInnerKey) const
{
    auto tOuterItr = mMap.find(aOuterKey);
    if(tOuterItr == mMap.end())
    {
        THROWERR(std::string("Did not find outer key '") + aOuterKey + "' in associative container.")
    }
    auto tInnerItr = tOuterItr->second.find(aInnerKey);
    if(tInnerItr == tOuterItr->second.end())
    {
        THROWERR(std::string("Did not find inner key '") + aInnerKey + "' in associative container.")
    }
    return tInnerItr->second;
}

const std::unordered_map<std::string, std::string>& Options::get(const std::string& aOuterKey) const
{
    auto tOuterItr = mMap.find(aOuterKey);
    if(tOuterItr == mMap.end())
    {
        THROWERR(std::string("Did not find outer key '") + aOuterKey + "' in associative container.")
    }
    return tOuterItr->second;
}

void Options::set(const std::string& aOuterKey, const std::string& aInnerKey, const std::string& aValues)
{
    mMap[aOuterKey][aInnerKey] = aValues;
}

std::vector<std::string> Options::otags() const
{
    std::vector<std::string> tOutput;
    for(auto tPair : mMap)
    {
        tOutput.push_back(tPair.first);
    }
    return tOutput;
}

void Options::build()
{
    // operation executable name defaults
    mMap["commands"]["web_app"] = "curl";
    mMap["commands"]["plato_app"] = ""; // to be determined once the plato-gemma app is implemented
    mMap["commands"]["system_call"] = "gemma";

    // function name defaults
    mMap["functions"]["web_app"] = "SystemCall";
    mMap["functions"]["plato_app"] = ""; // to be determined once the plato-gemma app is implemented
    mMap["functions"]["system_call"] = "SystemCallMPI";
    
    // app executable name defaults
    mMap["executables"]["web_app"] = "gemma";
    mMap["executables"]["plato_app"] = ""; // to be determined once the plato-gemma app is implemented
    mMap["executables"]["system_call"] = "gemma";
}

/*****************************************************************************************************/
/******************************************* Free Functions ******************************************/
/*****************************************************************************************************/

void append_general_web_app_options
(const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData)
{
    aOperationMetaData.append("code", "gemma");
    aOperationMetaData.append("base_web_port_number", "7000");
    aOperationMetaData.append("base_subdirectory_name", "evaluations");
    auto tNumConcurrentEvals = aInputMetaData.optimization_parameters().concurrent_evaluations();
    aOperationMetaData.append("concurrent_evaluations", tNumConcurrentEvals);
}
// function append_general_web_app_options

void write_web_app_input_deck(const XMLGen::InputData& aInputMetaData)
{
    XMLGen::gemma::Options tAppOptions;
    XMLGen::OperationMetaData tOperationMetaData;
    XMLGen::gemma::append_general_web_app_options(aInputMetaData, tOperationMetaData);
    XMLGen::set_operation_option_from_service_metadata("type", aInputMetaData, tOperationMetaData);
    XMLGen::append_operation_options_based_on_app_type("executables", tAppOptions.get("executables"), tOperationMetaData);
    XMLGen::append_evaluation_subdirectories(tOperationMetaData);
    XMLGen::matched_power_balance::write_web_app_input_deck(tOperationMetaData);
}
// function write_web_app_input_deck

void write_input_deck
(const XMLGen::InputData& aInputMetaData)
{
    XMLGen::OperationMetaData tOperationMetaData;
    tOperationMetaData.append("code", "gemma");
    tOperationMetaData.append("base_subdirectory_name", "evaluations");
    auto tNumConcurrentEvals = aInputMetaData.optimization_parameters().concurrent_evaluations();
    tOperationMetaData.append("concurrent_evaluations", tNumConcurrentEvals);
    XMLGen::append_evaluation_subdirectories(tOperationMetaData);
    XMLGen::matched_power_balance::write_input_deck(aInputMetaData, tOperationMetaData);
}
// function write_input_deck

void write_system_call_app_operation_files
(const XMLGen::InputData& aInputMetaData)
{ 
    pugi::xml_document tDocument;
    XMLGen::matched_power_balance::write_aprepro_system_call_operation("evaluations", aInputMetaData, tDocument);
    XMLGen::matched_power_balance::write_run_app_via_system_call_operation("evaluations", aInputMetaData, tDocument);
    XMLGen::matched_power_balance::write_harvest_data_from_file_operation("evaluations", aInputMetaData, tDocument);
    tDocument.save_file("plato_gemma_app_operation_file.xml", "  ");
}
// function write_system_call_app_operation_files

void write_operation_file
(const XMLGen::InputData& aInputMetaData)
{
    XMLGen::gemma::write_system_call_app_operation_files(aInputMetaData);
}
// function write_operation_file

}
// namespace gemma

}
// namespace XMLGen