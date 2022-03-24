/*
 * XMLGeneratorGemmaMatchedPowerBalanceUtilities.cpp
 *
 *  Created on: March 24, 2022
 */

#include "pugixml.hpp"

#include "XMLGeneratorUtilities.hpp"
#include "XMLGeneratorDataStruct.hpp"
#include "XMLGeneratorGemmaUtilities.hpp"
#include "XMLGeneratorScenarioUtilities.hpp"
#include "XMLGeneratorOperationsFileUtilities.hpp"
#include "XMLGeneratorOperationsDataStructures.hpp"

namespace XMLGen
{

namespace matched_power_balance
{

void append_input_file_options
(XMLGen::OperationMetaData& aOperationMetaData)
{
    XMLGen::gemma::Files tFileFormats; 
    auto tInputFileName = tFileFormats.filename("matched_power_balance", "input");
    aOperationMetaData.append("input_file", tInputFileName);
    aOperationMetaData.append("template_file", tInputFileName + ".template");
}
// function append_input_file_options

void write_aprepro_system_call_operation
(const std::string& aBaseSubDirName,
 const XMLGen::InputData& aInputMetaData,
 pugi::xml_document& aDocument)
{
    XMLGen::OperationMetaData tOperationMetaData;
    tOperationMetaData.append("base_subdirectory_name", aBaseSubDirName);
    XMLGen::matched_power_balance::append_input_file_options(tOperationMetaData);
    XMLGen::write_aprepro_operation(aInputMetaData, tOperationMetaData, aDocument);
}
// function write_aprepro_system_call_operation

void are_inputs_defined
(const std::unordered_map<std::string,std::string>& aKeyValuePairs)
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
// function are_inputs_defined

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
// function write_input_deck_to_file

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
// function get_input_key_value_pairs

void write_input_deck
(const XMLGen::InputData& aInputMetaData,
 const XMLGen::OperationMetaData& aOperationMetaData)
{
    XMLGen::gemma::Files tFileFormats; 
    auto tInputFileName = tFileFormats.filename("matched_power_balance", "input");
    auto tDescriptors = aInputMetaData.optimization_parameters().descriptors();
    auto tUniqueScenarioIDs = XMLGen::get_unique_scenario_ids(aOperationMetaData.front("code"), aInputMetaData);
    for(auto& tScenarioID : tUniqueScenarioIDs)
    {
        auto tScenario = aInputMetaData.scenario(tScenarioID);
        auto tMaterialID = tScenario.material();
        auto tMaterial = aInputMetaData.material(tMaterialID);
        auto tInputKeyValuePairs = XMLGen::matched_power_balance::get_input_key_value_pairs(tScenario, tMaterial, tDescriptors);
        auto tFileName = tDescriptors.empty() ? tInputFileName : tInputFileName + ".template";
        XMLGen::matched_power_balance::write_input_deck_to_file(tFileName, tInputKeyValuePairs);
        XMLGen::move_file_to_subdirectories(tFileName, aOperationMetaData.get("subdirectories"));
        auto tCommand = std::string("rm -f ") + tFileName;
        Plato::system(tCommand.c_str());
    }
}
// function write_input_deck

void write_run_app_via_system_call_operation
(const std::string& aBaseSubDirName,
 const XMLGen::InputData& aInputMetaData,
 pugi::xml_document& aDocument)
{
    XMLGen::gemma::Options tAppOptions;
    XMLGen::OperationMetaData tOperationMetaData;
    tOperationMetaData.append("code", "gemma");
    tOperationMetaData.append("base_web_port_number", "7000");
    tOperationMetaData.append("algorithm", "matched_power_balance");
    tOperationMetaData.append("base_subdirectory_name", aBaseSubDirName);
    XMLGen::write_run_app_via_system_call_operation(aInputMetaData, tAppOptions, tOperationMetaData, aDocument);
}
// function write_run_app_via_system_call_operation

void write_harvest_data_from_file_operation
(const std::string& aBaseSubDirName,
 const XMLGen::InputData& aInputMetaData,
 pugi::xml_document& aDocument)
{
    XMLGen::gemma::Files tFileFormats;
    XMLGen::OperationMetaData tOperationMetaData;
    tOperationMetaData.append("base_subdirectory_name", aBaseSubDirName);
    tOperationMetaData.append("base_output_file", tFileFormats.filename("matched_power_balance", "output"));
    XMLGen::write_harvest_data_from_file_operation(aInputMetaData, tOperationMetaData, aDocument);
}
// function write_harvest_data_from_file_operation

void write_web_app_input_deck
(XMLGen::OperationMetaData& aOperationMetaData)
{
    XMLGen::gemma::Files tFileFormats;
    aOperationMetaData.set("arguments", {"gemma", tFileFormats.filename("matched_power_balance", "input")});
    XMLGen::write_web_app_input_deck(aOperationMetaData);
}
// function write_web_app_input_deck

}
// namespace matched_power_balance

}
// namespace XMLGen