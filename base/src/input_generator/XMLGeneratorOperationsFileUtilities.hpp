/*
 * XMLGeneratorOperationsFileUtilities.hpp
 *
 *  Created on: March 21, 2022
 */

#pragma once

namespace pugi
{
    class xml_document;
}

namespace XMLGen
{

struct InputData;
struct OperationMetaData;
struct OperationArgument;

/******************************************************************************//**
 * \fn append_evaluation_subdirectories
 * \brief Set subdirectories associated with each concurrent performer evaluation.
 * \param [in]  aSubDirBaseName     base name for the subdirectories
 * \param [out] aOperationMetaData  operation metadata
**********************************************************************************/
void append_evaluation_subdirectories
(const std::string& aSubDirBaseName, 
 XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn append_concurrent_evaluation_index_to_option
 * \brief Append index associated with a given concurrent evaluation to value \n
 *        associated with input key and save in associated container.
 * \param [in]  aKey   key to access value in associative container
 * \param [in]  aValue value accessed with key
 * \param [out] aOperationMetaData operation metadata
**********************************************************************************/
void append_concurrent_evaluation_index_to_option
(const std::string& aKey,
 const std::string& aValue,
 XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn set_aprepro_system_call_options
 * \brief Set options associated with an aprepro system call.
 * \param [in] aOperationMetaData operation metadata
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
 * \param [in]  aOperationMetaData operation metadata
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
 * \param [out] aOperationMetaData operation metadata
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
 * \fn append_concurrent_operation_matadata_based_on_app_type
 * \brief Append operation option values based on app type. The number of values \n
 *    appended equals the number of concurrent evaluations. 
 * \param [in]  aKey associative key
 * \param [out] aOperationMetaData operation metadata
**********************************************************************************/

}
// namespace XMLGen