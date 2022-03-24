/*
 * XMLGeneratorOperationsFileUtilities.hpp
 *
 *  Created on: March 21, 2022
 */

#pragma once

#include "pugixml.hpp"

namespace XMLGen
{

struct InputData;
struct AppOptions;
struct OperationMetaData;
struct OperationArgument;

/******************************************************************************//**
 * \fn append_evaluation_subdirectories
 * \brief Set subdirectories associated with each concurrent performer evaluation.
 * \param [in] aOperationMetaData  operation metadata structure
**********************************************************************************/
void append_evaluation_subdirectories
(XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn append_concurrent_evaluation_index_to_option
 * \brief Append index associated with a given concurrent evaluation to value \n
 *        associated with input key and save in associated container.
 * \param [in]  aKey   key to access value in associative container
 * \param [in]  aValue value accessed with key
 * \param [out] aOperationMetaData operation metadata structure
**********************************************************************************/
void append_concurrent_evaluation_index_to_option
(const std::string& aKey,
 const std::string& aValue,
 XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn set_aprepro_system_call_options
 * \brief Set options associated with an aprepro system call.
 * \param [in] aOperationMetaData operation metadata structure
**********************************************************************************/
void set_aprepro_system_call_options
(XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn append_shared_data_argument_to_operation
 * \brief Append/Write shared data arguments to operation.
 * \param [in]  aArgument  data structure providing access to key-value pairs in class' associative container.
 * \param [out] aOperation pugi xml node 
**********************************************************************************/
void append_shared_data_argument_to_operation
(const XMLGen::OperationArgument& aArgument,
 pugi::xml_node& aOperation);

/******************************************************************************//**
 * \fn write_aprepro_system_call_operation
 * \brief Append/Write aprepro system call operation in operation file.
 * \param [in]  aInputs            input arguments to aprepro operation
 * \param [in]  aOperationMetaData operation metadata structure
 * \param [out] aDocument          pugi xml node 
**********************************************************************************/
void write_aprepro_system_call_operation
(const std::vector<XMLGen::OperationArgument>& aInputs,
 const XMLGen::OperationMetaData& aOperationMetaData,
 pugi::xml_document& aDocument);

/******************************************************************************//**
 * \fn set_descriptor_value
 * \brief Set descriptor value for aprepro operation. If the target descriptor is \n
 *     an active optimization parameter, set value to \{descriptor_name\}. If the \n
 *     target descriptor is a non-optimization parameter, set its value to its \n 
 *     input numeric value. \n
 * \param [in] aTargetDescriptor      target descriptor name
 * \param [in] aTargetDescriptorValue target descriptor value
 * \param [in] aDescriptors           list of optimizable descriptors
**********************************************************************************/
std::string set_descriptor_value
(const std::string& aTargetDescriptor,
 const std::string& aTargetDescriptorValue,
 const std::vector<std::string>& aDescriptors);

/******************************************************************************//**
 * \fn set_operation_option_from_service_metadata
 * \brief Set values associated with target key in associative container with \n
 *    operation's metadata. The number of values should match the number of \n
 *    concurrent evaluations.
 * \param [in]  aTargetKey         target key
 * \param [in]  aInputMetaData     input metadata read from Plato input deck
 * \param [out] aOperationMetaData operation metadata structure
**********************************************************************************/
void set_operation_option_from_service_metadata
(const std::string& aTargetKey,
 const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn check_app_service_type
 * \brief Check if service type equals 'plato_app', throw error if service type \n
     matches 'plato_app''.
 * \param [in] aServices List of services to check
**********************************************************************************/
void check_app_service_type(const std::string& aType);

/******************************************************************************//**
 * \fn append_operation_options_based_on_app_type
 * \brief Append operation option values based on app type. The number of values \n
 *    appended equals the number of concurrent evaluations. 
 * \param [in]  aKey associative key to set in operation metadata structure
 * \param [in]  aMap associative map with app-sepcific options
 * \param [out] aOperationMetaData operation metadata structure
**********************************************************************************/
void append_operation_options_based_on_app_type
(const std::string& aKey,
 const std::unordered_map<std::string, std::string>& aMap,
 XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn set_general_run_app_via_system_call_options
 * \brief Set general options for system call operation. 
 * \param [in]  aInputMetaData     input metadata read from Plato input deck
 * \param [out] aOperationMetaData operation metadata structure
**********************************************************************************/
void set_general_run_app_via_system_call_options
(const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn get_web_port_number
 * \brief Return web port number used for a given web application.
 * \param [in] aEvalIndex             concurrent evaluation index
 * \param [in] aOperationMetaData     operation metadata structure
 * \param [in] tDefaultBaseWebPortNum default base web port number (default = 7000)
**********************************************************************************/
size_t get_web_port_number
(const size_t aEvalIndex,
 const XMLGen::OperationMetaData& aOperationMetaData,
 const size_t& tDefaultBaseWebPortNum = 7000);

/******************************************************************************//**
 * \fn append_run_system_call_shared_data_arguments
 * \brief Append arguments needed to run an app via the system call operation. \n
 *    The arguments will be selected based on the service type. Is the app of \n
 *    type 'system_call' or 'web_app'. Arguments will be appended to the \n
 *    operation metadata structure.
 * \param [out] aOperationMetaData operation metadata structure
**********************************************************************************/
void append_run_system_call_shared_data_arguments
(XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn write_run_app_via_system_call_operation_to_file
 * \brief Write/Append the run app system call operation to the operation xml document.
 * \param [in]  aOperationMetaData operation metadata structure
 * \param [out] aDocument          pugi xml node 
**********************************************************************************/
void write_run_app_via_system_call_operation_to_file
(const XMLGen::OperationMetaData& aOperationMetaData,
 pugi::xml_document& aDocument);

/******************************************************************************//**
 * \fn get_data_group_type
 * \brief Return data type (column or row) for input data group.
 * \param [in]  aDataGroup data group - defined the data type and corresponding location index in file
 * \return data group type
**********************************************************************************/
std::string get_data_group_type(const std::string& aDataGroup);

/******************************************************************************//**
 * \fn get_data_group_index
 * \brief Return location index associated with this data group.
 * \param [in]  aDataGroup data group - defined the data type and corresponding location index in file
 * \return data group index
**********************************************************************************/
std::string get_data_group_index(const std::string& aDataGroup);

/******************************************************************************//**
 * \fn set_file_names
 * \brief Define and save file names in associative container. 
 * \param [in]  aKey key to access storage space in associative container.
 * \param [out] aOperationMetaData operation metadata structure
**********************************************************************************/
void set_file_names
(const std::string& aKey, 
 XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn write_harvest_objective_data_from_file_operation
 * \brief Write harvest the objective function value from file operation to operation xml document. 
 * \param [in]  aOutputs           output shared data arguments metadata
 * \param [in]  aOperationMetaData operation metadata structure
 * \param [out] aDocument          pugi xml node 
**********************************************************************************/
void write_harvest_objective_data_from_file_operation
(const std::vector<XMLGen::OperationArgument>& aOutputs,
 const XMLGen::OperationMetaData& aOperationMetaData,
 pugi::xml_document& aDocument);

/******************************************************************************//**
 * \fn write_harvest_constraint_data_from_file_operation
 * \brief Write harvest the constraint value from file operation to operation xml document. 
 * \param [in]  aOutputs           output shared data arguments metadata
 * \param [in]  aOperationMetaData operation metadata structure
 * \param [out] aDocument          pugi xml node 
**********************************************************************************/
void write_harvest_constraint_data_from_file_operation
(const std::vector<XMLGen::OperationArgument>& aOutputs,
 const XMLGen::OperationMetaData& aOperationMetaData,
 pugi::xml_document& aDocument);

/******************************************************************************//**
 * \fn append_system_call_constraint_criteria_options
 * \brief Append constraint criterion options used for a system call operation to operation metadata. 
 * \param [in]  aCriteriaIDs       sequence container of criteria ids
 * \param [in]  aInputMetaData     input metadata read from Plato input deck
 * \param [out] aOperationMetaData operation metadata structure
**********************************************************************************/
void append_system_call_constraint_criteria_options
(const std::vector<std::string>& aCriteriaIDs,
 const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn append_system_call_objective_criteria_options
 * \brief Append objective criterion options used for a system call operation to operation metadata. 
 * \param [in]  aCriteriaIDs       sequence container of criteria ids
 * \param [in]  aInputMetaData     input metadata read from Plato input deck
 * \param [out] aOperationMetaData operation metadata structure
**********************************************************************************/
void append_system_call_objective_criteria_options
(const std::vector<std::string>& aCriteriaIDs,
 const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn append_general_harvest_data_from_file_operation_options
 * \brief Append general options for harvest data operation to operation metadata. 
 * \param [in]  aInputMetaData     input metadata read from Plato input deck
 * \param [out] aOperationMetaData operation metadata structure
**********************************************************************************/
void append_general_harvest_data_from_file_operation_options
(const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn set_input_args_metadata_for_aprepro_operation
 * \brief Return sequence container with input arguments' metadata for aprepro operation. 
 * \param [in]  aOperationMetaData operation metadata structure
 * \return sequence container with input arguments' metadata
**********************************************************************************/
std::vector<XMLGen::OperationArgument>
set_input_args_metadata_for_aprepro_operation
(const XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn are_aprepro_input_options_defined
 * \brief Check if required aprepro operation options are defined. 
 * \param [in]  aOperationMetaData operation metadata structure
**********************************************************************************/
void are_aprepro_input_options_defined
(const XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn get_criterion_value_argument_metadata
 * \brief Return sequence container with criterion value arguments metadata. 
 * \param [in] aCriterionType     criterion type (objective or constraint)
 * \param [in] aOperationMetaData operation metadata structure
**********************************************************************************/
std::vector<XMLGen::OperationArgument>
get_criterion_value_argument_metadata
(const std::string& aCriterionType,
 const XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn append_web_port_numbers
 * \brief Append web port numbers to operation metadata. 
 * \param [in] aOperationMetaData operation metadata structure
**********************************************************************************/
void append_web_port_numbers
(XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn append_app_options
 * \brief Append app options to operation metadata. 
 * \param [in] aAppOptions app-specific options
 * \param [in] aOperationMetaData operation metadata structure
**********************************************************************************/
void append_app_options
(const XMLGen::AppOptions& aAppOptions,
 XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn append_general_aprepro_operation_options
 * \brief Append general options for aprepro operation to operation metadata. 
 * \param [in] aInputMetaData     input metadata read from Plato input deck
 * \param [in] aOperationMetaData operation metadata structure
**********************************************************************************/
void append_general_aprepro_operation_options
(const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn write_aprepro_operation
 * \brief Write aprepro operation to operation xml document. 
 * \param [in]  aInputMetaData     input metadata read from Plato input deck
 * \param [in]  aOperationMetaData operation metadata structure
 * \param [out] aDocument          pugi xml node 
**********************************************************************************/
void write_aprepro_operation
(const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData aOperationMetaData,
 pugi::xml_document& tDocument);

/******************************************************************************//**
 * \fn write_run_app_via_system_call_operation
 * \brief Write run app system call operation to operation xml document. 
 * \param [in]  aInputMetaData     input metadata read from Plato input deck
 * \param [in]  aAppOptions        application specific options
 * \param [in]  aOperationMetaData operation metadata structure
 * \param [out] aDocument          pugi xml node 
**********************************************************************************/
void write_run_app_via_system_call_operation
(const XMLGen::InputData& aInputMetaData,
 const XMLGen::AppOptions& aAppOptions,
 XMLGen::OperationMetaData& aOperationMetaData,
 pugi::xml_document& aDocument);

/******************************************************************************//**
 * \fn write_harvest_data_from_file_operation
 * \brief Write harvest data from file operation to operation xml document. 
 * \param [in]  aInputMetaData     input metadata read from Plato input deck
 * \param [in]  aOperationMetaData operation metadata structure
 * \param [out] aDocument          pugi xml node 
**********************************************************************************/
void write_harvest_data_from_file_operation
(const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData,
 pugi::xml_document& aDocument);

/******************************************************************************//**
 * \fn write_web_app_input_deck
 * \brief Write web app input deck to file on disk. 
 * \param [in] aOperationMetaData operation metadata structure
**********************************************************************************/
void write_web_app_input_deck
(XMLGen::OperationMetaData& aOperationMetaData);

}
// namespace XMLGen