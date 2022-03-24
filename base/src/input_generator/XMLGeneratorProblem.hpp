/*
 * XMLGeneratorProblem.hpp
 *
 *  Created on: Nov 20, 2020
 */

#pragma once

namespace XMLGen
{

struct InputData;

namespace Problem
{

/******************************************************************************//**
 * \fn create_subdirectory_for_evaluation
 * \brief create subdirectory and copy required files for performer
 * \param [in] aCsmFileName csm file name
 * \param [in] aEvaluation evaluation ID
**********************************************************************************/
void create_subdirectory_for_evaluation
(const std::string& aCsmFileName,
 int aEvaluation);

/******************************************************************************//**
 * \fn create_concurrent_evaluation_subdirectories
 * \brief create directories for concurrent evaluation performers
 * \param [in] aMetaData input metadata
**********************************************************************************/
void create_concurrent_evaluation_subdirectories
(const XMLGen::InputData& aMetaData);

/******************************************************************************//**
 * \fn move_input_deck_to_subdirectory
 * \brief move performer input deck to subdirectory
 * \param [in] aInputFileName performer input file name
**********************************************************************************/
void move_input_deck_to_subdirectory
(const std::string& aInputFileName,
 int aEvaluation);

/******************************************************************************//**
 * \fn write_plato_services_performer_input_deck_files
 * \brief Write the input deck file for the performer in the metada
 * \param [in] aMetaData input metadata
**********************************************************************************/
void write_plato_services_performer_input_deck_files
(XMLGen::InputData& aMetaData);

/******************************************************************************//**
 * \fn write_performer_operation_xml_file_gradient_based_problem
 * \brief Write the operations file for the performer in the metada
 * \param [in] aInputData input metadata
**********************************************************************************/
void write_performer_operation_xml_file_gradient_based_problem
(const XMLGen::InputData& aMetaData);

/******************************************************************************//**
 * \fn write_performer_operation_xml_file_dakota_problem
 * \brief Write the operations file for the performer in the metada
 * \param [in] aInputData input metadata
**********************************************************************************/
void write_performer_operation_xml_file_dakota_problem
(const XMLGen::InputData& aMetaData);

/******************************************************************************//**
 * \fn write_performer_input_deck_file_gradient_based_problem
 * \brief Write the input deck file for the performer in the metada
 * \param [in] aInputData input metadata
**********************************************************************************/
void write_performer_input_deck_file_gradient_based_problem
(const XMLGen::InputData& aMetaData);

/******************************************************************************//**
 * \fn write_performer_input_deck_file_dakota_problem
 * \brief Write the input deck file for the performer in the metada
 * \param [in] aMetaData input metadata
**********************************************************************************/
void write_performer_input_deck_file_dakota_problem
(XMLGen::InputData& aMetaData);

/******************************************************************************//**
 * \fn write_optimization_problem
 * \brief Write input files needed to solve optimization problems with Plato Analyze.
 * \param [in] aInputData input metadata
**********************************************************************************/
void write_optimization_problem
(const XMLGen::InputData& aMetaData,
 const std::vector<XMLGen::InputData>& aPreProcessedMetaData);

/******************************************************************************//**
 * \fn write_dakota_esp_problem
 * \brief Write input files needed to solve problems with dakota and engineering sketch pad (esp).
 * \param [in] aInputData input metadata
 * \param [in] aPreProcessedMetaData pre process metadata for problems with \n 
 *    concurrent app evaluations
**********************************************************************************/
void write_dakota_esp_problem
(XMLGen::InputData& aMetaData,
 const std::vector<XMLGen::InputData>& aPreProcessedMetaData);

/******************************************************************************//**
 * \fn write_dakota_problem
 * \brief Write input files needed to solve problems with dakota.
 * \param [in] aInputData input metadata
 * \param [in] aPreProcessedMetaData pre process metadata for problems with \n 
 *    concurrent app evaluations
**********************************************************************************/
void write_dakota_problem
(XMLGen::InputData& aMetaData,
 const std::vector<XMLGen::InputData>& aPreProcessedMetaData);

}
// namespace Problem

}
// namespace XMLGen
