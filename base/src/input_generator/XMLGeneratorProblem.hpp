/*
 * XMLGeneratorProblem.hpp
 *
 *  Created on: Nov 20, 2020
 */

#pragma once

#include <iostream>

#include "XMLGeneratorUtilities.hpp"
#include "XMLGeneratorDefinesFileUtilities.hpp"
#include "XMLGeneratorInterfaceFileUtilities.hpp"
#include "XMLGeneratorPlatoMainInputFileUtilities.hpp"
#include "XMLGeneratorPlatoMainOperationFileUtilities.hpp"
#include "XMLGeneratorPlatoESPInputFileUtilities.hpp"
#include "XMLGeneratorPlatoESPOperationsFileUtilities.hpp"
#include "XMLGeneratorPlatoXTKInputFile.hpp"
#include "XMLGeneratorPlatoXTKOperationFile.hpp"
#include "XMLGeneratorPlatoAnalyzeInputFileUtilities.hpp"
#include "XMLGeneratorPlatoAnalyzeOperationsFileUtilities.hpp"
#include "XMLGeneratorSierraSDInputDeckUtilities.hpp"
#include "XMLGeneratorSierraSDOperationsFileUtilities.hpp"
#include "XMLGeneratorLaunchScriptUtilities.hpp"
#include "XMLGeneratorPostOptimizationRunFileUtilities.hpp"
#include "XMLGeneratorDakotaInterfaceFileUtilities.hpp"
#include "XMLGeneratorDakotaDriverInputFileUtilities.hpp"

namespace XMLGen
{

namespace Problem
{

/******************************************************************************//**
 * \fn create_subdirectory_for_performer
 * \brief create subdirectory and required files for performer
 * \param [in] aCsmFileName csm file name
 * \param [in] aInputFileName performer input file name
 * \param [in] aEvaluation evaluation ID
**********************************************************************************/
inline void create_subdirectory_for_performer
(const std::string& aMeshFileName,
 const std::string& aCsmFileName,
 const std::string& aInputFileName,
 int aEvaluation)
{
    std::string tDirectoryName = std::string("evaluations_") + std::to_string(aEvaluation) + std::string("/");

    std::string tTag = std::string("_") + std::to_string(aEvaluation);
    auto tAppendedCsmFileName = XMLGen::append_concurrent_tag_to_file_string(aCsmFileName,tTag);
    auto tAppendedMeshFileName = XMLGen::append_concurrent_tag_to_file_string(aMeshFileName,tTag);
    auto tAppendedInputFileName = XMLGen::append_concurrent_tag_to_file_string(aInputFileName,tTag);

    std::string tCommand = std::string("mkdir ") + tDirectoryName;
    Plato::system(tCommand.c_str());
    tCommand = std::string("cp ") + aCsmFileName + std::string(" ") + tDirectoryName + tAppendedCsmFileName;
    Plato::system(tCommand.c_str());
    tCommand = std::string("cp ") + aMeshFileName + std::string(" ") + tDirectoryName + tAppendedMeshFileName;
    Plato::system(tCommand.c_str());
    tCommand = std::string("mv ") + aInputFileName + std::string(" ") + tDirectoryName + tAppendedInputFileName;
    Plato::system(tCommand.c_str());
}

/******************************************************************************//**
 * \fn write_performer_operation_xml_file_gradient_based_problem
 * \brief Write the operations file for the performer in the metada
 * \param [in] aInputData input metadata
**********************************************************************************/
inline void write_performer_operation_xml_file_gradient_based_problem
(const XMLGen::InputData& aMetaData)
{
    if(aMetaData.services().size() > 0)
    {
        if(aMetaData.services()[0].code() == "plato_analyze")
        {
            if(aMetaData.services()[0].id() == "helmholtz")
            {
                XMLGen::write_plato_analyze_helmholtz_operation_xml_file(aMetaData);
            }
            else
            {
                XMLGen::write_plato_analyze_operation_xml_file(aMetaData);
            }
        }
        else if(aMetaData.services()[0].code() == "sierra_sd")
        {
            XMLGen::write_sierra_sd_operation_xml_file(aMetaData);
        }
    }
}

/******************************************************************************//**
 * \fn write_performer_operation_xml_file_dakota_problem
 * \brief Write the operations file for the performer in the metada
 * \param [in] aInputData input metadata
**********************************************************************************/
inline void write_performer_operation_xml_file_dakota_problem
(const XMLGen::InputData& aMetaData)
{
    if(aMetaData.services().size() > 0)
    {
        if(aMetaData.services()[0].code() == "plato_analyze")
        {
            XMLGen::write_plato_analyze_operation_xml_file_dakota_problem(aMetaData);
        }
        else if(aMetaData.services()[0].code() == "sierra_sd")
        {
            XMLGen::write_sierra_sd_operation_xml_file(aMetaData);
        }
    }
}

/******************************************************************************//**
 * \fn write_performer_input_deck_file_gradient_based_problem
 * \brief Write the input deck file for the performer in the metada
 * \param [in] aInputData input metadata
**********************************************************************************/
inline void write_performer_input_deck_file_gradient_based_problem
(const XMLGen::InputData& aMetaData)
{
    if(aMetaData.services().size() > 0)
    {
        if(aMetaData.services()[0].code() == "plato_analyze")
        {
            if(aMetaData.services()[0].id() == "helmholtz")
            {
                XMLGen::write_plato_analyze_helmholtz_input_deck_file(aMetaData);
            }
            else
            {
                XMLGen::write_plato_analyze_input_deck_file(aMetaData);
            }
        }
        else if(aMetaData.services()[0].code() == "sierra_sd")
        {
            XMLGen::write_sierra_sd_input_deck(aMetaData);
        }
    }
}

/******************************************************************************//**
 * \fn write_performer_input_deck_file_dakota_problem
 * \brief Write the input deck file for the performer in the metada
 * \param [in] aInputData input metadata
**********************************************************************************/
inline void write_performer_input_deck_file_dakota_problem
(XMLGen::InputData& aMetaData)
{
    if(aMetaData.services().size() > 0)
    {
        auto tEvaluations = std::stoi(aMetaData.optimization_parameters().concurrent_evaluations());
        std::string tMeshName = aMetaData.mesh.run_name;
        std::string tCsmFileName = aMetaData.optimization_parameters().csm_file();

        if(aMetaData.services()[0].code() == "plato_analyze")
        {
            auto tServiceID = XMLGen::get_plato_analyze_service_id(aMetaData);
            std::string tInputFileName = std::string("plato_analyze_") + tServiceID + "_input_deck.xml";

            for (int iEvaluation = 0; iEvaluation < tEvaluations; iEvaluation++)
            {
                std::string tTag = std::string("_") + std::to_string(iEvaluation);
                auto tAppendedMeshName = XMLGen::append_concurrent_tag_to_file_string(tMeshName,tTag);
                aMetaData.mesh.run_name = std::string("evaluations_") + std::to_string(iEvaluation) + std::string("/") + tAppendedMeshName;

                XMLGen::write_plato_analyze_input_deck_file(aMetaData);
                XMLGen::Problem::create_subdirectory_for_performer(tMeshName,tCsmFileName,tInputFileName,iEvaluation);
            }
        }
        else if(aMetaData.services()[0].code() == "sierra_sd")
        {
            XMLGen::write_sierra_sd_input_deck(aMetaData);
        }
    }
}

/******************************************************************************//**
 * \fn write_optimization_problem
 * \brief Write input files needed to solve optimization problems with Plato Analyze.
 * \param [in] aInputData input metadata
**********************************************************************************/
inline void write_optimization_problem
(const XMLGen::InputData& aMetaData,
 const std::vector<XMLGen::InputData>& aPreProcessedMetaData)
{
    XMLGen::write_define_xml_file(aMetaData);
    XMLGen::write_interface_xml_file(aMetaData);
    XMLGen::generate_launch_script(aMetaData);
    XMLGen::write_plato_main_operations_xml_file(aMetaData);
    XMLGen::write_plato_main_input_deck_file(aMetaData);
    XMLGen::write_amgx_input_file(aMetaData);
    XMLGen::write_plato_esp_input_deck_file(aMetaData);
    XMLGen::write_plato_esp_operations_file(aMetaData);
    XMLGen::write_xtk_input_deck_file(aMetaData);
    XMLGen::write_xtk_operations_file(aMetaData);

    for(auto tCurMetaData : aPreProcessedMetaData)
    {
        write_performer_operation_xml_file_gradient_based_problem(tCurMetaData);
        write_performer_input_deck_file_gradient_based_problem(tCurMetaData);
    }

    XMLGen::write_post_optimization_run_files(aMetaData);
}

/******************************************************************************//**
 * \fn write_dakota_problem
 * \brief Write input files needed to solve problems with dakota.
 * \param [in] aInputData input metadata
**********************************************************************************/
inline void write_dakota_problem
(const XMLGen::InputData& aMetaData,
 const std::vector<XMLGen::InputData>& aPreProcessedMetaData)
{
    XMLGen::write_define_xml_file(aMetaData);
    XMLGen::write_dakota_interface_xml_file(aMetaData);
    XMLGen::generate_launch_script(aMetaData);
    XMLGen::write_dakota_plato_main_operations_xml_file(aMetaData);
    XMLGen::write_plato_main_input_deck_file(aMetaData);
    XMLGen::write_amgx_input_file(aMetaData);
    XMLGen::write_plato_esp_input_deck_file(aMetaData);
    XMLGen::write_plato_esp_operations_file(aMetaData);
    XMLGen::write_xtk_input_deck_file(aMetaData);
    XMLGen::write_xtk_operations_file(aMetaData);

    for(auto tCurMetaData : aPreProcessedMetaData)
    {
        write_performer_operation_xml_file_dakota_problem(tCurMetaData);
        write_performer_input_deck_file_dakota_problem(tCurMetaData);
    }

    XMLGen::write_dakota_driver_input_deck(aMetaData);
}

}
// namespace Problem

}
// namespace XMLGen
