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
 * \fn append_evaluation_subdirectory_names
 * \brief Set subdirectories associated with each concurrent performer evaluation.
 * \param [in] aSubDirBaseName     base name for the subdirectories
 * \param [in] aOperationMetaData  operation metadata
**********************************************************************************/
void append_evaluation_subdirectory_names
(const std::string& aSubDirBaseName, 
 XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn append_concurrent_evaluation_index_to_option
 * \brief Append index associated with a given concurrent evaluation to value \n
 *        associated with input key and save in associated container.
 * \param [in] aKey   key to access value in associative container
 * \param [in] aValue value accessed with key
 * \param [in] aOperationMetaData operation metadata
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
 * \brief Append shared data arguments to operation.
 * \param [in]  aArgument  data structure providing access to key-value pairs in class' associative container.
 * \param [out] aOperation pugi xml node 
**********************************************************************************/
void append_shared_data_argument_to_operation
(const XMLGen::OperationArgument& aArgument,
 pugi::xml_node& aOperation);

}
// namespace XMLGen