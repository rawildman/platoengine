/*
 * XMLGeneratorProblem.cpp
 *
 *  Created on: March 24, 2022
 */

#include <string>
#include <vector>

#include "XMLGeneratorProblem.hpp"
#include "XMLGeneratorUtilities.hpp"
#include "XMLGeneratorDataStruct.hpp"
#include "XMLGeneratorServiceUtilities.hpp"
#include "XMLGeneratorSierraSDUtilities.hpp"
#include "XMLGeneratorPlatoXTKInputFile.hpp"
#include "XMLGeneratorDefinesFileUtilities.hpp"
#include "XMLGeneratorPlatoXTKOperationFile.hpp"
#include "XMLGeneratorLaunchScriptUtilities.hpp"
#include "XMLGeneratorInterfaceFileUtilities.hpp"
#include "XMLGeneratorPlatoESPInputFileUtilities.hpp"
#include "XMLGeneratorSierraSDInputDeckUtilities.hpp"
#include "XMLGeneratorPlatoMainInputFileUtilities.hpp"
#include "XMLGeneratorDakotaInterfaceFileUtilities.hpp"
#include "XMLGeneratorPlatoAnalyzeInputFileUtilities.hpp"
#include "XMLGeneratorDakotaDriverInputFileUtilities.hpp"
#include "XMLGeneratorPlatoMainOperationFileUtilities.hpp"
#include "XMLGeneratorPlatoESPOperationsFileUtilities.hpp"
#include "XMLGeneratorSierraSDOperationsFileUtilities.hpp"
#include "XMLGeneratorPostOptimizationRunFileUtilities.hpp"
#include "XMLGeneratorPlatoAnalyzeOperationsFileUtilities.hpp"

namespace XMLGen
{

namespace Problem
{

void create_subdirectory_for_evaluation
(const std::string& aCsmFileName,
 int aEvaluation)
{
    std::string tDirectoryName = std::string("evaluations_") + std::to_string(aEvaluation) + std::string("/");

    std::string tTag = std::string("_") + std::to_string(aEvaluation);
    auto tAppendedCsmFileName = XMLGen::append_concurrent_tag_to_file_string(aCsmFileName,tTag);

    std::string tCommand = std::string("mkdir ") + tDirectoryName;
    Plato::system_with_throw(tCommand.c_str());
    tCommand = std::string("cp ") + aCsmFileName + std::string(" ") + tDirectoryName + tAppendedCsmFileName;
    Plato::system_with_throw(tCommand.c_str());
}
// function create_subdirectory_for_evaluation

void create_concurrent_evaluation_subdirectories
(const XMLGen::InputData& aMetaData)
{
    std::string tCsmFileName = aMetaData.optimization_parameters().csm_file();
    auto tEvaluations = std::stoi(aMetaData.optimization_parameters().concurrent_evaluations());
    for (int iEvaluation = 0; iEvaluation < tEvaluations; iEvaluation++)
    {
        if (!XMLGen::subdirectory_exists(std::string("evaluations_") + std::to_string(iEvaluation)))
            XMLGen::Problem::create_subdirectory_for_evaluation(tCsmFileName,iEvaluation);
    }
}
// function create_concurrent_evaluation_subdirectories

void move_input_deck_to_subdirectory
(const std::string& aInputFileName,
 int aEvaluation)
{
    std::string tDirectoryName = std::string("evaluations_") + std::to_string(aEvaluation) + std::string("/");

    std::string tTag = std::string("_") + std::to_string(aEvaluation);
    auto tAppendedInputFileName = XMLGen::append_concurrent_tag_to_file_string(aInputFileName,tTag);

    auto tCommand = std::string("mv ") + aInputFileName + std::string(" ") + tDirectoryName + tAppendedInputFileName;
    Plato::system_with_throw(tCommand.c_str());
}
// function move_input_deck_to_subdirectory

void write_plato_services_performer_input_deck_files
(XMLGen::InputData& aMetaData)
{
    std::string tMeshName = aMetaData.mesh.run_name;
    std::string tInputFileName = "plato_main_input_deck.xml";
    auto tEvaluations = std::stoi(aMetaData.optimization_parameters().concurrent_evaluations());
    for (int iEvaluation = 0; iEvaluation < tEvaluations; iEvaluation++)
    {
        std::string tTag = std::string("_") + std::to_string(iEvaluation);
        auto tAppendedMeshName = XMLGen::append_concurrent_tag_to_file_string(tMeshName,tTag);
        aMetaData.mesh.run_name = std::string("evaluations_") + std::to_string(iEvaluation) + std::string("/") + tAppendedMeshName;
        XMLGen::write_plato_main_input_deck_file(aMetaData);
        XMLGen::Problem::move_input_deck_to_subdirectory(tInputFileName,iEvaluation);
    }
    aMetaData.mesh.run_name = tMeshName;
}
// function write_plato_services_performer_input_deck_files

void write_performer_operation_xml_file_gradient_based_problem
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
// function write_performer_operation_xml_file_gradient_based_problem

void write_performer_operation_xml_file_dakota_problem
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
//function write_performer_operation_xml_file_dakota_problem

void write_performer_input_deck_file_gradient_based_problem
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
// function write_performer_input_deck_file_gradient_based_problem

void write_performer_input_deck_file_dakota_problem
(XMLGen::InputData& aMetaData)
{
    if(XMLGen::is_physics_performer(aMetaData))
    {
        std::string tMeshName = aMetaData.mesh.run_name;
        auto tEvaluations = std::stoi(aMetaData.optimization_parameters().concurrent_evaluations());
        for (int iEvaluation = 0; iEvaluation < tEvaluations; iEvaluation++)
        {
            std::string tTag = std::string("_") + std::to_string(iEvaluation);
            auto tAppendedMeshName = XMLGen::append_concurrent_tag_to_file_string(tMeshName,tTag);
            aMetaData.mesh.run_name = std::string("evaluations_") + std::to_string(iEvaluation) + std::string("/") + tAppendedMeshName;

            std::string tInputFileName = "";
            if(aMetaData.services()[0].code() == "plato_analyze")
            {
                auto tServiceID = XMLGen::get_plato_analyze_service_id(aMetaData);
                tInputFileName = std::string("plato_analyze_") + tServiceID + "_input_deck.xml";
                XMLGen::write_plato_analyze_input_deck_file(aMetaData);
            }
            else if(aMetaData.services()[0].code() == "sierra_sd")
            {
                auto tServiceID = XMLGen::get_salinas_service_id(aMetaData);
                tInputFileName = std::string("sierra_sd_") + tServiceID + "_input_deck.i";
                XMLGen::write_sierra_sd_input_deck(aMetaData);
            }
            XMLGen::Problem::move_input_deck_to_subdirectory(tInputFileName,iEvaluation);
        }
        aMetaData.mesh.run_name = tMeshName;
    }
}
// function write_performer_input_deck_file_dakota_problem

void write_optimization_problem
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
        XMLGen::Problem::write_performer_operation_xml_file_gradient_based_problem(tCurMetaData);
        XMLGen::Problem::write_performer_input_deck_file_gradient_based_problem(tCurMetaData);
    }

    XMLGen::write_post_optimization_run_files(aMetaData);
}
// write_optimization_problem

void write_dakota_esp_problem
(XMLGen::InputData& aMetaData,
 const std::vector<XMLGen::InputData>& aPreProcessedMetaData)
{
    XMLGen::Problem::create_concurrent_evaluation_subdirectories(aMetaData);
    XMLGen::write_define_xml_file(aMetaData);
    XMLGen::write_dakota_interface_xml_file(aMetaData);
    XMLGen::generate_launch_script(aMetaData);
    XMLGen::write_dakota_plato_main_operations_xml_file(aMetaData);
    XMLGen::write_amgx_input_file(aMetaData);
    XMLGen::write_plato_esp_input_deck_file(aMetaData);
    XMLGen::write_plato_esp_operations_file(aMetaData);
    XMLGen::write_xtk_input_deck_file(aMetaData);
    XMLGen::write_xtk_operations_file(aMetaData);

    XMLGen::Problem::write_plato_services_performer_input_deck_files(aMetaData);
    for(auto tCurMetaData : aPreProcessedMetaData)
    {
        XMLGen::Problem::write_performer_operation_xml_file_dakota_problem(tCurMetaData);
        XMLGen::Problem::write_performer_input_deck_file_dakota_problem(tCurMetaData);
    }

    XMLGen::write_plato_main_input_deck_file(aMetaData);
    XMLGen::write_dakota_driver_input_deck(aMetaData);
}

void write_dakota_problem
(XMLGen::InputData& aMetaData,
 const std::vector<XMLGen::InputData>& aPreProcessedMetaData)
{
    XMLGen::Problem::write_dakota_esp_problem(aMetaData, aPreProcessedMetaData);
}
// function write_dakota_problem

}
// namespace Problem

}
// namespace XMLGen