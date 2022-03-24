/*
 * XMLGeneratorGemmaUtilities.hpp
 *
 *  Created on: March 23, 2022
 */

#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "XMLGeneratorOperationsDataStructures.hpp"

namespace XMLGen
{

struct InputData;

namespace gemma
{

/*****************************************************************************************************/
/*********************************************** Files ***********************************************/
/*****************************************************************************************************/

/******************************************************************************//**
 * \struct This C++ structure implements a multi-dimensional associative container \n
 *    holding the file names used for the Gemma solver based on the solver/algorithm \n
 *    and requested file format. 
**********************************************************************************/
struct Files : public XMLGen::FileFormats
{
private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> mMap; /*!< associative container */

public:
    /******************************************************************************//**
     * \fn constructor
     * \brief Simple constructor
    **********************************************************************************/
    Files();

    /******************************************************************************//**
     * \fn filename
     * \brief Return filename associated with the corresponding solver and file format types. \n
     *    Supported solver types include: 'matched_power_balance' and 'unmatched_power_balance'. \n
     *    Supported file formats include: 'input' and 'output'.
     * \param [in] aSolverType Gemma solver/algorithm type
     * \param [in] aFileFormat requested file format
    **********************************************************************************/
    std::string filename(const std::string& aSolverType, const std::string& aFileFormat) const;

private:
    /******************************************************************************//**
     * \fn build
     * \brief Build multi-dimensional associative container. 
    **********************************************************************************/
    void build();
};
// struct Files

/*****************************************************************************************************/
/********************************************** Options **********************************************/
/*****************************************************************************************************/

/******************************************************************************//**
 * \struct Options
 * \brief This C++ structure implements a multi-dimensional associative container \n
 *    holding Gemma options based on the requested command and service (app) type. \n
 *    Supported service types include: 'plato_app', 'web_app', and 'system_call'.
**********************************************************************************/
struct Options : public XMLGen::AppOptions
{
private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> mMap; /*!< associative container */

public:
    /******************************************************************************//**
     * \fn constructor
     * \brief Simple constructor
    **********************************************************************************/
    Options();

    /******************************************************************************//**
     * \fn get
     * \brief Return constant reference to parameter value.
     * \param [in] aOuterKey multi-dimensional associative container outer key
     * \param [in] aInnerKey multi-dimensional associative container inner key
     * \return parameter value
    **********************************************************************************/
    const std::string& get(const std::string& aOuterKey, const std::string& aInnerKey) const;

    /******************************************************************************//**
     * \fn get
     * \brief Return constant reference to inner associative container.
     * \param [in] aOuterKey multi-dimensional associative container outer key
     * \return associative container
    **********************************************************************************/
    const std::unordered_map<std::string, std::string>& get(const std::string& aOuterKey) const;

    /******************************************************************************//**
     * \fn set
     * \brief Set parameter value.
     * \param [in] aOuterKey multi-dimensional associative container outer key
     * \param [in] aInnerKey multi-dimensional associative container inner key
     * \param [in] aValue    parameter value to store in multi-dimensional associative container
    **********************************************************************************/
    void set(const std::string& aOuterKey, const std::string& aInnerKey, const std::string& aValue);

    /******************************************************************************//**
     * \fn otags
     * \brief Return outer tags from multi-dimensional associative container.
     * \return sequence container
    **********************************************************************************/
    std::vector<std::string> otags() const;

private:
    /******************************************************************************//**
     * \fn build
     * \brief Build multi-dimensional associative container. 
    **********************************************************************************/
    void build();
};
// struct Options

/*****************************************************************************************************/
/******************************************* Free Functions ******************************************/
/*****************************************************************************************************/

/******************************************************************************//**
 * \fn append_general_web_app_options
 * \brief Append general web application options. 
 * \param [in] aInputMetaData     input metadata read from Plato input deck
 * \param [in] aOperationMetaData operation metadata structure
**********************************************************************************/
void append_general_web_app_options
(const XMLGen::InputData& aInputMetaData,
 XMLGen::OperationMetaData& aOperationMetaData);

/******************************************************************************//**
 * \fn write_web_app_input_deck
 * \brief Write Gemma web application input deck. 
 * \param [in] aInputMetaData input metadata read from Plato input deck
**********************************************************************************/
void write_web_app_input_deck
(const XMLGen::InputData& aInputMetaData);

/******************************************************************************//**
 * \fn write_input_deck
 * \brief Write Gemma application input deck. 
 * \param [in] aInputMetaData input metadata read from Plato input deck
**********************************************************************************/
void write_input_deck
(const XMLGen::InputData& aInputMetaData);

/******************************************************************************//**
 * \fn write_system_call_app_operation_files
 * \brief Write system call operations to operation xml file. 
 * \param [in] aInputMetaData input metadata read from Plato input deck
**********************************************************************************/
void write_system_call_app_operation_files
(const XMLGen::InputData& aInputMetaData);

/******************************************************************************//**
 * \fn write_operation_file
 * \brief Write operation xml file. 
 * \param [in] aInputMetaData input metadata read from Plato input deck
**********************************************************************************/
void write_operation_file
(const XMLGen::InputData& aInputMetaData);

}
// namespace gemma

}
// namespace XMLGen