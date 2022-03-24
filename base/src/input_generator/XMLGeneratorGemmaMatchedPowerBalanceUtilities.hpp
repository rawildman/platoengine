/*
 * XMLGeneratorGemmaMatchedPowerBalanceUtilities.cpp
 *
 *  Created on: March 24, 2022
 */

#pragma once

namespace pugi
{
    class xml_document;
}

namespace XMLGen
{

struct OperationMetaData;

namespace matched_power_balance
{

/******************************************************************************//**
 * \fn append_input_file_options
 * \brief Append input file options
 * \param [out] aOperationMetaData operation metadata structure
**********************************************************************************/
void append_input_file_options
(XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn append_input_file_options
 * \brief Append input file options
 * \param [in]  aBaseSubDirName subdirectory base name (a subdirectory is created for each concurrent evaluation)
 * \param [in]  aInputMetaData  input metadata read from Plato input deck
 * \param [out] aDocument       pugi xml node 
**********************************************************************************/
void write_aprepro_system_call_operation
(const std::string& aBaseSubDirName,
 const XMLGen::InputData& aInputMetaData,
 pugi::xml_document& aDocument);

/******************************************************************************//**
 * \fn are_inputs_defined
 * \brief Check if input file options are defined before writing the input deck. \n
 *    An error is thrown if a required input option is not defined.
 * \param [in] aKeyValuePairs associative container
**********************************************************************************/
void are_inputs_defined
(const std::unordered_map<std::string,std::string>& aKeyValuePairs);

/******************************************************************************//**
 * \fn write_input_deck_to_file
 * \brief Write input deck for mathced power balance solver/algorithm.
 * \param [in] aFileName name for input deck file
 * \param [in] aKeyValuePairs associative container with input options
**********************************************************************************/
void write_input_deck_to_file
(const std::string& aFileName, 
 const std::unordered_map<std::string,std::string>& aKeyValuePairs);

/******************************************************************************//**
 * \fn get_input_key_value_pairs
 * \brief Return associative container with input option key-value pairs.
 * \param [in] aScenario scenario metadata
 * \param [in] aMaterial material metadata
 * \param [in] aDescriptors optimizable parameter descriptors
 * \return associative container
**********************************************************************************/
std::unordered_map<std::string,std::string> 
get_input_key_value_pairs
(const XMLGen::Scenario& aScenario,
 const XMLGen::Material& aMaterial,
 const std::vector<std::string>& aDescriptors);

/******************************************************************************//**
 * \fn write_input_deck
 * \brief Write input deck for matched power balance algorithm
 * \param [in] aInputMetaData  input metadata read from Plato input deck.
 * \param [in] aOperationMetaData operation metadata structure
**********************************************************************************/
void write_input_deck
(const XMLGen::InputData& aInputMetaData,
 const XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn write_run_app_via_system_call_operation
 * \brief Write run app via system call operation to operation xml document.
 * \param [in]  aBaseSubDirName base name for evaluation subdirectory
 * \param [in]  aInputMetaData  input metadata read from Plato input deck
 * \param [out] aDocument       pugi xml node 
**********************************************************************************/
void write_run_app_via_system_call_operation
(const std::string& aBaseSubDirName,
 const XMLGen::InputData& aInputMetaData,
 pugi::xml_document& aDocument);

/******************************************************************************//**
 * \fn write_harvest_data_from_file_operation
 * \brief Write harvest data from file operation to operation xml document.
 * \param [in]  aBaseSubDirName base name for evaluation subdirectory
 * \param [in]  aInputMetaData  input metadata read from Plato input deck
 * \param [out] aDocument       pugi xml node 
**********************************************************************************/
void write_harvest_data_from_file_operation
(const std::string& aBaseSubDirName,
 const XMLGen::InputData& aInputMetaData,
 pugi::xml_document& aDocument);

/******************************************************************************//**
 * \fn write_web_app_input_deck
 * \brief Write web app input deck for matched power balance algorithm.
 * \param [in] aOperationMetaData operation metadata structure
**********************************************************************************/
void write_web_app_input_deck
(XMLGen::OperationMetaData& aOperationMetaData);

}
// namespace matched_power_balance

}
// namespace XMLGen