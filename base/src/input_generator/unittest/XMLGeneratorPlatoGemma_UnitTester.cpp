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

namespace XMLGen
{

struct OperationArguments
{
private:
    std::unordered_map<std::string, std::string> mData;
public:
    void set(const std::string& aKey, const std::string& aValue)
    {
        auto tLowerKey = XMLGen::to_lower(aKey);
        mData[tLowerKey] = aValue;
    }
    const std::string& get(const std::string& aKey) const
    {
        auto tLowerKey = XMLGen::to_lower(aKey);
        return mData.find(tLowerKey)->second;
    }
};

struct OperationMetaData
{
private:
    std::unordered_map<std::string, std::vector<std::string>> mData;
public:
    void append(const std::string& aKey, const std::string& aValue)
    {
        auto tLowerKey = XMLGen::to_lower(aKey);
        mData[tLowerKey].push_back(aValue);
    }
    void set(const std::string& aKey, const std::vector<std::string>& aValues)
    {
        auto tLowerKey = XMLGen::to_lower(aKey);
        mData[tLowerKey] = aValues;
    }
    const std::vector<std::string>& get(const std::string& aKey) const
    {
        auto tLowerKey = XMLGen::to_lower(aKey);
        return mData.find(tLowerKey)->second;
    }
    bool find(const std::string& aKey) const
    {
        auto tItr = mData.find(aKey);
        if(tItr == mData.end())
        {
            return false;
        }
        return true;
    }
    std::vector<std::string> keys() const
    {
        std::vector<std::string> tKeys;
        for(auto tPair : mData)
        {
            tKeys.push_back(tPair.first);
        }
        return tKeys;
    }
};

void append_integer_to_aprepro_option
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

void append_integer_to_aprepro_options
(XMLGen::OperationMetaData& aOperationMetaDataOptions)
{
    auto tNumConcurrentEvals = std::stoi(aOperationMetaDataOptions.get("concurrent_evaluations").front());
    aOperationMetaDataOptions.set("functions", std::vector<std::string>(tNumConcurrentEvals, "OperationMetaData"));
    aOperationMetaDataOptions.set("commands", std::vector<std::string>(tNumConcurrentEvals, "aprepro"));

    XMLGen::append_integer_to_aprepro_option("names", "aprepro_", aOperationMetaDataOptions);
    XMLGen::append_integer_to_aprepro_option("directories", "evaluation_", aOperationMetaDataOptions);
    
    auto tDescriptors = aOperationMetaDataOptions.get("descriptors");
    aOperationMetaDataOptions.set("options", tDescriptors);
    auto tInputFileName = aOperationMetaDataOptions.get("input_file").front();
    auto tTemplateFileName = aOperationMetaDataOptions.get("template_file").front();
    std::vector<std::string> tArguments = {"-q", tTemplateFileName, tInputFileName};
    aOperationMetaDataOptions.set("arguments", tArguments);
}

void append_aprepro_system_call_operation_inputs
(const std::vector<XMLGen::OperationArguments>& aInputs,
 const std::string& aSuffix,
 pugi::xml_node& aOperation)
{
    for(auto tInput : aInputs)
    {
        auto tInputNode = aOperation.append_child("Input");
        auto tArgName = tInput.get("name") + aSuffix;
        XMLGen::append_children({"ArgumentName", "Layout", "Size"}, {tArgName, tInput.get("layout"), tInput.get("size")}, tInputNode);
    }
}

void write_aprepro_system_call_operation
(const std::vector<XMLGen::OperationArguments>& aInputs,
 const XMLGen::OperationMetaData& aOperationMetaData,
 pugi::xml_document& aDocument)
{
    auto tOptions = aOperationMetaData.get("options");
    auto tArguments = aOperationMetaData.get("arguments");
    auto tAppendInput = aInputs.empty() ? "false" : "true";

    auto tNumConcurrentEvals = std::stoi(aOperationMetaData.get("concurrent_evaluations").front());
    for(decltype(tNumConcurrentEvals) tIndex = 0; tIndex < tNumConcurrentEvals; tIndex++)
    {
        auto tSuffix = std::string("_") + std::to_string(tIndex);
        auto tOperation = aDocument.append_child("Operation");

        auto tFuncName = std::string("aprepro") + tSuffix;
        std::vector<std::string> tKeys = {"Function", "Name", "Command", "OnChange", "AppendInput"};
        std::vector<std::string> tVals = {aOperationMetaData.get("functions")[tIndex], 
                                          aOperationMetaData.get("names")[tIndex], 
                                          aOperationMetaData.get("commands")[tIndex], 
                                          "true", tAppendInput};
        XMLGen::append_children(tKeys, tVals, tOperation);
        XMLGen::append_children(std::vector<std::string>(tArguments.size(), "Argument"), tArguments, tOperation);
        XMLGen::append_children(std::vector<std::string>(tOptions.size(), "Option"), tOptions, tOperation);

        XMLGen::append_aprepro_system_call_operation_inputs(aInputs, tSuffix, tOperation);
    }
}

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

std::string set_descriptor_value
(const std::string& aTargetDescriptor,
 const std::string& aTargetDescriptorValue,
 const std::vector<std::string>& aDescriptors)
{
    auto tOutput = std::find(aDescriptors.begin(), aDescriptors.end(), aTargetDescriptor) == aDescriptors.end() ? aTargetDescriptorValue : std::string("{") + aTargetDescriptor + "}";
    return tOutput;
}

namespace matched_power_balance
{

void are_aprepro_input_options_defined
(const XMLGen::OperationMetaData& aOperationMetaData)
{
    std::vector<std::string> tValidKeys = {"concurrent_evaluations", "names", "options", "arguments", "commands", "functions"};
    for(auto& tValidKey : tValidKeys)
    {
        if(!aOperationMetaData.find(tValidKey))
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

void preprocess_aprepro_system_call_options
(const XMLGen::InputData& aMetaData,
 XMLGen::OperationMetaData& aOperationMetaData)
{
    aOperationMetaData.append("input_file", "gemma_matched_power_balance_input_deck.yaml");
    aOperationMetaData.append("template_file", "gemma_matched_power_balance_input_deck.yaml.template");
    auto tNumConcurrentEvals = aMetaData.optimization_parameters().concurrent_evaluations();
    aOperationMetaData.append("concurrent_evaluations", tNumConcurrentEvals);
    auto tDescriptors = aMetaData.optimization_parameters().descriptors();
    aOperationMetaData.set("descriptors", tDescriptors);
}

void preprocess_aprepro_system_call_inputs
(const XMLGen::InputData& aMetaData,
 std::vector<XMLGen::OperationArguments>& aData)
{
    XMLGen::OperationArguments tParamData;
    tParamData.set("name", "parameters");
    tParamData.set("layout", "scalar");
    auto tSize = aMetaData.optimization_parameters().values("lower_bounds").size();
    if(tSize <= 0)
    {
        THROWERR("Number of parameters can not be less or equal than zero.")
    }
    auto tSrSize = std::to_string(tSize);
    tParamData.set("size", tSrSize);
    aData.push_back(tParamData);
}

void aprepro_system_call
(const XMLGen::InputData& aMetaData,
 pugi::xml_document& tDocument)
{
    XMLGen::OperationMetaData tOperationMetaData;
    XMLGen::matched_power_balance::preprocess_aprepro_system_call_options(aMetaData, tOperationMetaData);
    XMLGen::append_integer_to_aprepro_options(tOperationMetaData);
    std::vector<XMLGen::OperationArguments> tData;
    XMLGen::matched_power_balance::preprocess_aprepro_system_call_inputs(aMetaData, tData);
    XMLGen::matched_power_balance::are_aprepro_input_options_defined(tOperationMetaData);
    XMLGen::write_aprepro_system_call_operation(tData, tOperationMetaData, tDocument);
}

void are_inputs_defned(const std::unordered_map<std::string,std::string>& aKeyValuePairs)
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

void input_deck(const std::string& aFileName, const std::unordered_map<std::string,std::string>& aKeyValuePairs)
{
    XMLGen::matched_power_balance::are_inputs_defned(aKeyValuePairs);

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

}
// namespace matched_power_balance

void write_matched_power_balance_input_deck(const XMLGen::InputData& aMetaData)
{
    std::vector<std::string> tDescriptors = aMetaData.optimization_parameters().descriptors();
    auto tUniqueGemmaScenarioIDs = XMLGen::get_unique_scenario_ids("gemma", aMetaData);
    for(auto& tScenarioID : tUniqueGemmaScenarioIDs)
    {
        auto tScenario = aMetaData.scenario(tScenarioID);
        auto tMaterialID = tScenario.material();
        auto tMaterial = aMetaData.material(tMaterialID);
        auto tInputKeyValuePairs = XMLGen::matched_power_balance::get_input_key_value_pairs(tScenario, tMaterial, tDescriptors);
        auto tFileName = tDescriptors.empty() ? "gemma_matched_power_balance_input_deck.yaml" : "gemma_matched_power_balance_input_deck.yaml.template";
        XMLGen::matched_power_balance::input_deck(tFileName, tInputKeyValuePairs);
    }
}

void write_gemma_input_deck(const XMLGen::InputData& aMetaData)
{
    XMLGen::write_matched_power_balance_input_deck(aMetaData);
}

}

namespace PlatoTestXMLGenerator
{

TEST(PlatoTestXMLGenerator, aprepro_system_call_matched_power_balance)
{
    XMLGen::InputData tMetaData;
    // define optmization parameters
    XMLGen::OptimizationParameters tOptParams;
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    tOptParams.descriptors(tDescriptors);
    tOptParams.append("concurrent_evaluations", "3");
    tOptParams.set("lower_bounds", std::vector<std::string>(3, "1.0"));
    tMetaData.set(tOptParams);

    pugi::xml_document tDocument;
    XMLGen::matched_power_balance::aprepro_system_call(tMetaData, tDocument);
    tDocument.save_file("dummy.txt");

    auto tReadData = XMLGen::read_data_from_file("dummy.txt");
    auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>OperationMetaData</Function><Name>aprepro_0</Name><Command>aprepro</Command><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>gemma_matched_power_balance_input_deck.yaml.template</Argument><Argument>gemma_matched_power_balance_input_deck.yaml</Argument><Option>slot_length</Option><Option>slot_width</Option><Option>slot_depth</Option><Input><ArgumentName>parameters_0</ArgumentName><Layout>scalar</Layout><Size>3</Size></Input></Operation><Operation><Function>OperationMetaData</Function><Name>aprepro_1</Name><Command>aprepro</Command><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>gemma_matched_power_balance_input_deck.yaml.template</Argument><Argument>gemma_matched_power_balance_input_deck.yaml</Argument><Option>slot_length</Option><Option>slot_width</Option><Option>slot_depth</Option><Input><ArgumentName>parameters_1</ArgumentName><Layout>scalar</Layout><Size>3</Size></Input></Operation><Operation><Function>OperationMetaData</Function><Name>aprepro_2</Name><Command>aprepro</Command><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>gemma_matched_power_balance_input_deck.yaml.template</Argument><Argument>gemma_matched_power_balance_input_deck.yaml</Argument><Option>slot_length</Option><Option>slot_width</Option><Option>slot_depth</Option><Input><ArgumentName>parameters_2</ArgumentName><Layout>scalar</Layout><Size>3</Size></Input></Operation>");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
    Plato::system("rm -f dummy.txt");
}

TEST(PlatoTestXMLGenerator, are_aprepro_input_options_defined_matched_power_balance)
{
    XMLGen::OperationMetaData tOperationMetaData1;
    tOperationMetaData1.append("concurrent_evaluations", "2");
    tOperationMetaData1.set("names", {"aprepro_0", "aprepro_1"});
    tOperationMetaData1.set("options", {"length=", "depth=", "width="});
    tOperationMetaData1.set("commands", std::vector<std::string>(2u, "aprepro"));
    tOperationMetaData1.set("functions", std::vector<std::string>(2u, "SystemCall"));
    tOperationMetaData1.set("arguments", {"-q", "matched.yaml.template", "matched.yaml"});
    EXPECT_NO_THROW(XMLGen::matched_power_balance::are_aprepro_input_options_defined(tOperationMetaData1));

    XMLGen::OperationMetaData tOperationMetaData2;
    tOperationMetaData2.set("names", {"aprepro_0", "aprepro_1"});
    tOperationMetaData2.set("options", {"length=", "depth=", "width="});
    tOperationMetaData2.set("commands", std::vector<std::string>(2u, "aprepro"));
    tOperationMetaData2.set("functions", std::vector<std::string>(2u, "SystemCall"));
    tOperationMetaData2.set("arguments", {"-q", "matched.yaml.template", "matched.yaml"});
    EXPECT_THROW(XMLGen::matched_power_balance::are_aprepro_input_options_defined(tOperationMetaData2), std::runtime_error);
}

TEST(PlatoTestXMLGenerator, write_aprepro_system_call_operation)
{
    XMLGen::OperationMetaData tOperationMetaData;
    tOperationMetaData.append("concurrent_evaluations", "2");
    tOperationMetaData.set("names", {"aprepro_0", "aprepro_1"});
    tOperationMetaData.set("options", {"length=", "depth=", "width="});
    tOperationMetaData.set("commands", std::vector<std::string>(2u, "aprepro"));
    tOperationMetaData.set("functions", std::vector<std::string>(2u, "SystemCall"));
    tOperationMetaData.set("arguments", {"-q", "matched.yaml.template", "matched.yaml"});

    std::vector<XMLGen::OperationArguments> tInputs;
    XMLGen::OperationArguments tInput;
    tInput.set("size", "3");
    tInput.set("layout", "scalars");
    tInput.set("name", "parameters");
    tInputs.push_back(tInput);

    pugi::xml_document tDocument;
    XMLGen::write_aprepro_system_call_operation(tInputs, tOperationMetaData, tDocument);
    tDocument.save_file("dummy.txt");

    auto tReadData = XMLGen::read_data_from_file("dummy.txt");
    auto tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Function>SystemCall</Function><Name>aprepro_0</Name><Command>aprepro</Command><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>matched.yaml.template</Argument><Argument>matched.yaml</Argument><Option>length=</Option><Option>depth=</Option><Option>width=</Option><Input><ArgumentName>parameters_0</ArgumentName><Layout>scalars</Layout><Size>3</Size></Input></Operation><Operation><Function>SystemCall</Function><Name>aprepro_1</Name><Command>aprepro</Command><OnChange>true</OnChange><AppendInput>true</AppendInput><Argument>-q</Argument><Argument>matched.yaml.template</Argument><Argument>matched.yaml</Argument><Option>length=</Option><Option>depth=</Option><Option>width=</Option><Input><ArgumentName>parameters_1</ArgumentName><Layout>scalars</Layout><Size>3</Size></Input></Operation>");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
    Plato::system("rm -f dummy.txt");
}

TEST(PlatoTestXMLGenerator, preprocess_aprepro_system_call_inputs_matched_power_balance)
{
    XMLGen::InputData tMetaData;
    // define optmization parameters
    XMLGen::OptimizationParameters tOptParams;
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    tOptParams.descriptors(tDescriptors);
    tOptParams.append("concurrent_evaluations", "3");
    tOptParams.set("lower_bounds", std::vector<std::string>(3, "1.0"));
    tMetaData.set(tOptParams);

    std::vector<XMLGen::OperationArguments> tData;
    XMLGen::matched_power_balance::preprocess_aprepro_system_call_inputs(tMetaData, tData);
    EXPECT_EQ(1u, tData.size());
    EXPECT_STREQ("3", tData.front().get("size").c_str());
    EXPECT_STREQ("scalar", tData.front().get("layout").c_str());
    EXPECT_STREQ("parameters", tData.front().get("name").c_str());
}

TEST(PlatoTestXMLGenerator, append_integer_to_aprepro_option)
{
    XMLGen::OperationMetaData tOperationMetaData;
    tOperationMetaData.append("concurrent_evaluations", "3");
    XMLGen::append_integer_to_aprepro_option("names", "aprepro_", tOperationMetaData);

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

TEST(PlatoTestXMLGenerator, write_gemma_input_deck)
{
    XMLGen::InputData tMetaData;
    // define optmization parameters
    XMLGen::OptimizationParameters tOptParams;
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    tOptParams.descriptors(tDescriptors);
    tMetaData.set(tOptParams);

    // define objective 
    XMLGen::Objective tObjective;
    tObjective.type = "weighted_sum";
    tObjective.criteriaIDs.push_back("1");
    tObjective.criteriaIDs.push_back("2");
    tObjective.serviceIDs.push_back("1");
    tObjective.serviceIDs.push_back("2");
    tObjective.scenarioIDs.push_back("1");
    tObjective.scenarioIDs.push_back("2");
    tMetaData.objective = tObjective;

    // define service
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tMetaData.append(tServiceOne);
    XMLGen::Service tServiceTwo;
    tServiceTwo.append("code", "platomain");
    tServiceTwo.append("id", "2");
    tMetaData.append(tServiceTwo);

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
    tMetaData.append(tScenario);

    // define material
    XMLGen::Material tMaterial;
    tMaterial.id("1");
    tMaterial.property("conductivity", "1e6");
    tMetaData.append(tMaterial);

    // TEST 1: WRITE TEMPLATE FILE NEEDED FOR APREPRO OPERATION
    XMLGen::write_gemma_input_deck(tMetaData);
    auto tReadData = XMLGen::read_data_from_file("gemma_matched_power_balance_input_deck.yaml.template");
    std::string tGoldString = std::string("%YAML1.1---Gemma-dynamic:Global:Description:HigginscylinderSolutiontype:powerbalancePowerbalance:Algorithm:matchedboundRadius:0.1016Height:0.1018Conductivity:1e6Slotlength:{slot_length}Slotwidth:{slot_width}Slotdepth:{slot_depth}Startfrequencyrange:10Endfrequencyrange:100Frequencyintervalsize:5...");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
    Plato::system("rm -f gemma_matched_power_balance_input_deck.yaml.template");

    // TEST 2: ERROR - PARAMETER VALUE WAS NOT DEFINED
    tDescriptors.clear();
    tOptParams.descriptors(tDescriptors);
    tMetaData.set(tOptParams);
    ASSERT_THROW(XMLGen::write_gemma_input_deck(tMetaData), std::runtime_error);

    // TEST 3: WRITE REGULAR INPUT DECK
    tScenario.append("slot_width", "0.0118");
    tScenario.append("slot_depth", "0.00218");
    tScenario.append("slot_length", "0.0116");
    std::vector<XMLGen::Scenario> tScenarios;
    tScenarios.push_back(tScenario);
    tMetaData.set(tScenarios);
    XMLGen::write_gemma_input_deck(tMetaData);
    tReadData = XMLGen::read_data_from_file("gemma_matched_power_balance_input_deck.yaml");
    tGoldString = std::string("%YAML1.1---Gemma-dynamic:Global:Description:HigginscylinderSolutiontype:powerbalancePowerbalance:Algorithm:matchedboundRadius:0.1016Height:0.1018Conductivity:1e6Slotlength:0.0116Slotwidth:0.0118Slotdepth:0.00218Startfrequencyrange:10Endfrequencyrange:100Frequencyintervalsize:5...");
    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
    Plato::system("rm -f gemma_matched_power_balance_input_deck.yaml");
}

TEST(PlatoTestXMLGenerator, matched_power_balance_input_deck)
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
    XMLGen::matched_power_balance::input_deck("input_deck.yaml",tMap);
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