/*
 * XMLGeneratorPlatoGemma_UnitTester.cpp
 *
 *  Created on: Feb 28, 2022
 */

#include <gtest/gtest.h>

#include "pugixml.hpp"

#include "XMLGeneratorDataStruct.hpp"
#include "XMLGeneratorParserUtilities.hpp"

namespace XMLGen
{

struct MapFromCriterionToServiceScenarioPairs
{
public:
    void insert(const std::string& aCriterionID, const std::string& aServiceID, const std::string& aScenarioID)
    {
        mMap[aCriterionID] = std::make_pair(aServiceID, aScenarioID);
    }
    std::vector<std::string> criterion_ids() const
    {
        std::vector<std::string> tOutput;
        for(auto& tPair : mMap)
        {
            tOutput.push_back(tPair.first);
        }
        return tOutput;
    }
    std::vector<std::string> scenario_ids() const
    {
        std::vector<std::string> tOutput;
        for(auto& tPair : mMap)
        {
            tOutput.push_back(tPair.second.first);
        }
        return tOutput;
    }
    std::vector<std::string> service_ids() const
    {
        std::vector<std::string> tOutput;
        for(auto& tPair : mMap)
        {
            tOutput.push_back(tPair.second.second);
        }
        return tOutput;
    }
    std::vector<std::pair<std::string,std::string>> criterion_scenario_pairs() const
    {
        std::vector<std::pair<std::string,std::string>> tOutput;
        for(auto& tPair : mMap)
        {
            tOutput.push_back(std::make_pair(tPair.first,tPair.second.second));
        }
        return tOutput;
    }

private:
    std::unordered_map<std::string, std::pair<std::string, std::string>> mMap;
};

std::vector<std::string> 
get_unique_scenario_ids
(const std::string& aTargetCodeName,
 const XMLGen::InputData& aMetaData)
{
    std::vector<std::string> tUniqueScenarioIDs;
    auto tLowerTargetCodeName = XMLGen::to_lower(aTargetCodeName);

    for(auto& tID : aMetaData.objective.criteriaIDs)
    {
        auto tIndex = &tID - &aMetaData.objective.criteriaIDs[0];
        auto tServiceID = aMetaData.objective.serviceIDs[tIndex];
        auto tLowerCode = XMLGen::to_lower(aMetaData.service(tServiceID).code());
        if(tLowerCode == tLowerTargetCodeName)
        {
            auto tScenarioID = aMetaData.objective.scenarioIDs[tIndex];
            if(std::find(tUniqueScenarioIDs.begin(), tUniqueScenarioIDs.end(), tScenarioID) == tUniqueScenarioIDs.end())
            {
                tUniqueScenarioIDs.push_back(tScenarioID);   
            }
        }
    }

    return tUniqueScenarioIDs;
}

XMLGen::MapFromCriterionToServiceScenarioPairs
get_map_from_criterion_to_service_scenario_pairs
(const XMLGen::InputData& aMetaData)
{
    XMLGen::MapFromCriterionToServiceScenarioPairs tOutput;
    for(auto& tID : aMetaData.objective.criteriaIDs)
    {
        auto tIndex = &tID - &aMetaData.objective.criteriaIDs[0];
        tOutput.insert(tID, aMetaData.objective.serviceIDs[tIndex], aMetaData.objective.scenarioIDs[tIndex]);
    }
}

std::vector<std::pair<std::string,std::string>> 
get_unique_gemma_scenario_ids
(const XMLGen::MapFromCriterionToServiceScenarioPairs& aMap)
{
    auto tAllPairs = aMap.criterion_scenario_pairs();
    std::vector<std::pair<std::string,std::string>> tOutput;
    for(auto& tPair : tAllPairs)
    {
        auto tLower = XMLGen::to_lower(tPair.second);
        if(tLower == "gemma")
        {
            tOutput.push_back(tPair);
        }
    }
    return tOutput;
}

std::string set_descriptor_value
(const std::string& aTargetDescriptor,
 const std::string& aTargetDescriptorValue,
 const std::vector<std::string>& aDescriptors)
{
    std::find(aDescriptors.begin(), aDescriptors.end(), aTargetDescriptor) == aDescriptors.end() ? aTargetDescriptorValue : std::string("{") + aTargetDescriptor + "}";
}

namespace matched_power_balance
{

void write_input_deck
(const std::unordered_map<std::string,std::string>& aKeyValuePairs)
{
    std::ofstream tOutFile;
    tOutFile.open("matched.yaml", std::ofstream::out | std::ofstream::trunc);
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

    tMap["slot_width"] = XMLGen::set_descriptor_value("slot_width", aScenario.value("slot_width"), aDescriptors);
    tMap["slot_depth"] = XMLGen::set_descriptor_value("slot_depth", aScenario.value("slot_depth"), aDescriptors);
    tMap["slot_depth"] = XMLGen::set_descriptor_value("slot_depth", aScenario.value("slot_depth"), aDescriptors);
    tMap["slot_length"] = XMLGen::set_descriptor_value("slot_length", aScenario.value("slot_length"), aDescriptors);
    tMap["cavity_radius"] = XMLGen::set_descriptor_value("cavity_radius", aScenario.value("cavity_radius"), aDescriptors);
    tMap["cavity_height"] = XMLGen::set_descriptor_value("cavity_height", aScenario.value("cavity_height"), aDescriptors);
    tMap["frequency_min"] = XMLGen::set_descriptor_value("frequency_min", aScenario.value("frequency_min"), aDescriptors);
    tMap["frequency_max"] = XMLGen::set_descriptor_value("frequency_max", aScenario.value("frequency_max"), aDescriptors);
    tMap["frequency_step"] = XMLGen::set_descriptor_value("frequency_step", aScenario.value("frequency_step"), aDescriptors);
    tMap["conductivity"] = XMLGen::set_descriptor_value("conductivity", aMaterial.property("conductivity"), aDescriptors);

    return tMap;
}

}
// namespace matched_power_balance

void write_matched_power_balance_input_deck(const XMLGen::InputData& aMetaData)
{
    std::vector<std::string> tDescriptors; // = aMetaData.optimization_parameters().descriptors();
    auto tUniqueGemmaScenarioIDs = XMLGen::get_unique_scenario_ids("gemma", aMetaData);
    for(auto& tScenarioID : tUniqueGemmaScenarioIDs)
    {
        auto tScenario = aMetaData.scenario(tScenarioID);
        auto tMaterialID = tScenario.material();
        auto tMaterial = aMetaData.material(tMaterialID);
        auto tInputKeyValuePairs = XMLGen::matched_power_balance::get_input_key_value_pairs(tScenario, tMaterial, tDescriptors);
        XMLGen::matched_power_balance::write_input_deck(tInputKeyValuePairs);
    }
}

void write_gemma_input_deck(const XMLGen::InputData& aMetaData)
{
    XMLGen::write_matched_power_balance_input_deck(aMetaData);
}

}

namespace PlatoTestXMLGenerator
{

TEST(PlatoTestXMLGenerator, ParseSystemCallCriterion)
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
        "begin criterion MySystemCall\n"
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