/*
 * XMLGeneratorDakotaInterfaceFileUtilities.cpp
 *
 *  Created on: Jan 18, 2022
 */

#include <tuple>

#include "XMLGeneratorUtilities.hpp"
#include "XMLGeneratorParserUtilities.hpp"
#include "XMLGeneratorInterfaceFileUtilities.hpp"
#include "XMLGeneratorDakotaInterfaceFileUtilities.hpp"
#include "XMLGeneratorPerformersUtilities.hpp"

namespace XMLGen
{

/******************************************************************************/
void write_dakota_interface_xml_file
(const XMLGen::InputData& aMetaData)
{
    pugi::xml_document tDocument;

    XMLGen::append_include_defines_xml_data(aMetaData, tDocument);
    XMLGen::append_console_data(aMetaData, tDocument);
    XMLGen::append_dakota_performer_data(aMetaData, tDocument);
    XMLGen::append_dakota_shared_data(aMetaData, tDocument);
    XMLGen::append_dakota_stages(aMetaData, tDocument);
    XMLGen::append_dakota_driver_options(aMetaData, tDocument);

    tDocument.save_file("interface.xml", "  ");
}
/******************************************************************************/

/******************************************************************************/
void append_dakota_performer_data
(const XMLGen::InputData& aMetaData,
 pugi::xml_document& aDocument)
{
    XMLGen::append_plato_main_performer(aMetaData, aDocument);

    int tPerformerId = 1;
    XMLGen::append_physics_performers_dakota_usecase(aMetaData, aDocument, tPerformerId);
    XMLGen::append_platoservices_dakota_usecase(aMetaData, aDocument, tPerformerId);
}
/******************************************************************************/

/******************************************************************************/
void append_physics_performers_dakota_usecase
(const XMLGen::InputData& aMetaData,
 pugi::xml_document& aDocument,
 int& aPerformerId)
{
    std::vector<std::string> tKeywords = { "Name", "Code", "PerformerID" };
    auto tEvaluations = std::stoi(aMetaData.optimization_parameters().concurrent_evaluations());
    for(auto& tService : aMetaData.services())
    {
        if(tService.code() == "plato_analyze" || tService.code() == "sierra_sd")
        {
            for (size_t iEvaluation = 0; iEvaluation < tEvaluations; iEvaluation++)
            {
                auto tPerformerNode = aDocument.append_child("Performer");
                std::string tPerformerName = tService.performer() + std::string("_") + std::to_string(iEvaluation);
                std::vector<std::string> tValues = { tPerformerName, tService.code(), std::to_string(aPerformerId) };
                XMLGen::append_children( tKeywords, tValues, tPerformerNode);
                aPerformerId++;
            }
        }
    }
}

/******************************************************************************/
void append_platoservices_dakota_usecase
(const XMLGen::InputData& aMetaData,
 pugi::xml_document& aDocument,
 int& aPerformerId)
{
    auto tPerformerNode = aDocument.append_child("Performer");
    addChild(tPerformerNode, "PerformerID", std::to_string(aPerformerId));
    auto tForNode = tPerformerNode.append_child("For");
    tForNode.append_attribute("var") = "I";
    tForNode.append_attribute("in") = "Parameters";
    addChild(tForNode, "Name", "plato_services_{I}");
    addChild(tForNode, "Code", "plato_services");
}

/******************************************************************************/
void append_dakota_shared_data
(const XMLGen::InputData& aMetaData,
 pugi::xml_document& aDocument)
{
    XMLGen::append_concurrent_design_variables_shared_data(aMetaData, aDocument);
    XMLGen::append_dakota_criterion_shared_data(aMetaData, aDocument);
}
/******************************************************************************/

/******************************************************************************/
void append_concurrent_design_variables_shared_data
(const XMLGen::InputData& aMetaData,
 pugi::xml_document& aDocument)
{
    std::string tFirstPlatoMainPerformer = aMetaData.getFirstPlatoMainPerformer();
    auto tForNode = aDocument.append_child("For");
    tForNode.append_attribute("var") = "I";
    tForNode.append_attribute("in") = "Parameters";
    auto tTmpNode = tForNode.append_child("SharedData");
    addChild(tTmpNode, "Name", "design_parameters_{I}");
    addChild(tTmpNode, "Type", "Scalar");
    addChild(tTmpNode, "Layout", "Global");
    addChild(tTmpNode, "Size", aMetaData.optimization_parameters().num_shape_design_variables());
    addChild(tTmpNode, "OwnerName", tFirstPlatoMainPerformer);
    addChild(tTmpNode, "UserName", tFirstPlatoMainPerformer);
    for(auto& tService : aMetaData.services())
    {
        if(tService.code() == "plato_analyze" || tService.code() == "sierra_sd")
        {
            addChild(tTmpNode, "UserName", tService.performer() + std::string("_{I}"));
        }
    }
}

/******************************************************************************/
void append_dakota_criterion_shared_data
(const XMLGen::InputData& aMetaData,
 pugi::xml_document& aDocument)
{
    std::string tFirstPlatoMainPerformer = aMetaData.getFirstPlatoMainPerformer();

    auto tConcretizedCriteria = aMetaData.getConcretizedCriteria();
    for(auto& tCriterion : tConcretizedCriteria)
    {      
        std::string tCriterionID = std::get<0>(tCriterion);
        std::string tServiceID = std::get<1>(tCriterion);
        std::string tScenarioID = std::get<2>(tCriterion);
        std::string tIdentifierString = XMLGen::get_concretized_criterion_identifier_string(tCriterion);

        XMLGen::Service tService = aMetaData.service(tServiceID);
        std::string tOwnerName = tService.performer();

        auto tForNode = aDocument.append_child("For");
        tForNode.append_attribute("var") = "I";
        tForNode.append_attribute("in") = "Parameters";
        auto tTmpNode = tForNode.append_child("SharedData");

        std::string tTag = tIdentifierString + std::string("_{I}");
        std::vector<std::string> tKeys = { "Name", "Type", "Layout", "Size", "OwnerName", "UserName" };
        std::vector<std::string> tValues = { tTag, "Scalar", "Global", "1", tOwnerName + std::string("_{I}"), tFirstPlatoMainPerformer };
        XMLGen::append_children(tKeys, tValues, tTmpNode);
    }
}

/******************************************************************************/
void append_dakota_stages
(const XMLGen::InputData& aMetaData,
 pugi::xml_document& aDocument)
{
    XMLGen::append_initialize_stage(aMetaData, aDocument);
    XMLGen::append_criterion_value_stages(aMetaData, aDocument);
}

/******************************************************************************/
void append_initialize_stage
(const XMLGen::InputData& aMetaData,
 pugi::xml_document& aDocument)
{
    auto tStageNode = aDocument.append_child("Stage");
    XMLGen::append_children( { "Name" }, { "Initialize Meshes" }, tStageNode);
    XMLGen::append_design_parameters_input(tStageNode);

    auto tOperationNode = tStageNode.append_child("Operation");
    XMLGen::append_parallel_update_geometry_on_change_operation(tOperationNode);

    tOperationNode = tStageNode.append_child("Operation");
    XMLGen::append_parallel_reinitialize_on_change_operation(aMetaData,tOperationNode);
}

/******************************************************************************/
void append_design_parameters_input
(pugi::xml_node& aParentNode)
{
    auto tForNode = aParentNode.append_child("For");
    XMLGen::append_attributes({"var", "in"}, {"I", "Parameters"}, tForNode);
    auto tInputNode = tForNode.append_child("Input");
    XMLGen::append_children({ "SharedDataName" }, { "design_parameters_{I}" }, tInputNode);
}

/******************************************************************************/
void append_parallel_update_geometry_on_change_operation
(pugi::xml_node& aParentNode)
{
    auto tForNode = aParentNode.append_child("For");
    XMLGen::append_attributes({"var", "in"}, {"I", "Parameters"}, tForNode);
    auto tOperationNode = tForNode.append_child("Operation");
    XMLGen::append_children({"Name", "PerformerName"}, {"update_geometry_on_change_{I}", "plato_services_{I}"}, tOperationNode);
    auto tInputNode = tOperationNode.append_child("Input");
    XMLGen::append_children({"SharedDataName", "ArgumentName"}, {"design_parameters_{I}", "Parameters_{I}"}, tInputNode);
}

/******************************************************************************/
void append_parallel_reinitialize_on_change_operation
(const XMLGen::InputData& aMetaData,
 pugi::xml_node& aParentNode)
{
    auto tForNode = aParentNode.append_child("For");
    XMLGen::append_attributes({"var", "in"}, {"I", "Parameters"}, tForNode);
    for(auto& tService : aMetaData.services())
    {
        if(tService.code() != "plato_esp" && tService.code() != "platomain")
        {
            auto tOperationNode = tForNode.append_child("Operation");
            auto tOperationName = std::string("reinitialize_on_change_") + tService.performer();
            auto tPerformerName = tService.performer() + std::string("_{I}");
            XMLGen::append_children({"Name", "PerformerName"}, {tOperationName, tPerformerName}, tOperationNode);
            auto tInputNode = tOperationNode.append_child("Input");
            XMLGen::append_children({"SharedDataName", "ArgumentName"}, {"design_parameters_{I}", "Parameters"}, tInputNode);
        }
    }
}

/******************************************************************************/
void append_criterion_value_stages
(const XMLGen::InputData& aMetaData,
 pugi::xml_document& aDocument)
{
    int tCriteriaCounter = 0;
    XMLGen::append_objective_criterion_value_stages(aMetaData,aDocument,tCriteriaCounter);
    XMLGen::append_constraint_criterion_value_stages(aMetaData,aDocument,tCriteriaCounter);
}

/******************************************************************************/
void append_objective_criterion_value_stages
(const XMLGen::InputData& aMetaData,
 pugi::xml_document& aDocument,
 int& aCriterionNumber)
{
    XMLGen::Objective tObjective = aMetaData.objective;
    for (size_t i=0; i<tObjective.criteriaIDs.size(); ++i)
    {
        auto tStageNode = aDocument.append_child("Stage");
        auto tStageName = std::string("Compute Criterion ") + std::to_string(aCriterionNumber) + std::string(" Value");
        XMLGen::append_children( { "Name" }, { tStageName }, tStageNode);

        std::string tCriterionID = tObjective.criteriaIDs[i];
        std::string tServiceID = tObjective.serviceIDs[i];
        std::string tScenarioID = tObjective.scenarioIDs[i];
        ConcretizedCriterion tConcretizedCriterion(tCriterionID,tServiceID,tScenarioID);
        auto tIdentifierString = XMLGen::get_concretized_criterion_identifier_string(tConcretizedCriterion);
        XMLGen::Service tService = aMetaData.service(tServiceID); 

        XMLGen::append_parallel_criterion_value_operation(tStageNode,tService,tIdentifierString,aCriterionNumber);
        aCriterionNumber++;
    }
}

/******************************************************************************/
void append_parallel_criterion_value_operation
(pugi::xml_node& aParentNode,
 XMLGen::Service& aService,
 const std::string& aIdentifierString,
 int aCriterionNumber)
 {
    auto tOperationNode = aParentNode.append_child("Operation");
    auto tForNode = tOperationNode.append_child("For");
    XMLGen::append_attributes({"var", "in"}, {"I", "Parameters"}, tForNode);
    tOperationNode = tForNode.append_child("Operation");
    auto tInnerOperationName = std::string("Compute Criterion Value - ") + aIdentifierString;
    XMLGen::append_children({"Name", "PerformerName"}, {tInnerOperationName, aService.performer() + std::string("_{I}")}, tOperationNode);
    auto tOutputNode = tOperationNode.append_child("Output");
    auto tArgumentName = std::string("Criterion ") + std::to_string(aCriterionNumber) + std::string(" Value");
    XMLGen::append_children({"SharedDataName", "ArgumentName"}, {aIdentifierString + std::string("_{I}"), tArgumentName}, tOutputNode);

    tForNode = aParentNode.append_child("For");
    XMLGen::append_attributes({"var", "in"}, {"I", "Parameters"}, tForNode);
    tOutputNode = tForNode.append_child("Output");
    XMLGen::append_children({ "SharedDataName" }, { aIdentifierString + std::string("_{I}") }, tOutputNode);

 }

/******************************************************************************/
void append_constraint_criterion_value_stages
(const XMLGen::InputData& aMetaData,
 pugi::xml_document& aDocument,
 int& aCriterionNumber)
{
    for(auto& tConstraint : aMetaData.constraints)
    {
        auto tStageNode = aDocument.append_child("Stage");
        auto tStageName = std::string("Compute Criterion ") + std::to_string(aCriterionNumber) + std::string(" Value");
        XMLGen::append_children( { "Name" }, { tStageName }, tStageNode);

        std::string tCriterionID = tConstraint.criterion();
        std::string tServiceID = tConstraint.service();
        std::string tScenarioID = tConstraint.scenario();
        ConcretizedCriterion tConcretizedCriterion(tCriterionID,tServiceID,tScenarioID);
        auto tIdentifierString = XMLGen::get_concretized_criterion_identifier_string(tConcretizedCriterion);
        XMLGen::Service tService = aMetaData.service(tServiceID); 

        XMLGen::append_parallel_criterion_value_operation(tStageNode,tService,tIdentifierString,aCriterionNumber);
        aCriterionNumber++;
    }
}

/******************************************************************************/
void append_dakota_driver_options
(const XMLGen::InputData& aMetaData,
 pugi::xml_document& aDocument)
 {
    auto tDriverNode = aDocument.append_child("DakotaDriver");
    XMLGen::append_initialize_stage_options(tDriverNode);

    int tCriteriaCounter = 0;
    XMLGen::append_objective_criteria_stage_options(aMetaData,tDriverNode,tCriteriaCounter);
    XMLGen::append_constraint_criteria_stage_options(aMetaData,tDriverNode,tCriteriaCounter);
 }

/******************************************************************************/
void append_initialize_stage_options
(pugi::xml_node& aParentNode)
{
    auto tStageNode = aParentNode.append_child("Stage");
    XMLGen::append_children( { "StageTag", "StageName" }, { "initialize", "Initialize Meshes" }, tStageNode);
    auto tForNode = tStageNode.append_child("For");
    XMLGen::append_attributes({"var", "in"}, {"I", "Parameters"}, tForNode);
    auto tInputNode = tForNode.append_child("Input");
    XMLGen::append_children({ "Tag", "SharedDataName" }, { "continuous", "design_parameters_{I}" }, tInputNode);
}

/******************************************************************************/
void append_objective_criteria_stage_options
(const XMLGen::InputData& aMetaData,
 pugi::xml_node& aParentNode,
 int& aCriterionNumber)
 {
    XMLGen::Objective tObjective = aMetaData.objective;
    for (size_t i=0; i<tObjective.criteriaIDs.size(); ++i)
    {
        std::string tCriterionID = tObjective.criteriaIDs[i];
        std::string tServiceID = tObjective.serviceIDs[i];
        std::string tScenarioID = tObjective.scenarioIDs[i];
        ConcretizedCriterion tConcretizedCriterion(tCriterionID,tServiceID,tScenarioID);
        auto tIdentifierString = XMLGen::get_concretized_criterion_identifier_string(tConcretizedCriterion);

        XMLGen::append_criterion_stage_options(aParentNode,tIdentifierString,aCriterionNumber);
        aCriterionNumber++;
    }
 }

/******************************************************************************/
void append_criterion_stage_options
(pugi::xml_node& aParentNode,
 const std::string& aIdentifierString,
 int aCriterionNumber)
 {
    auto tStageName = std::string("Compute Criterion ") + std::to_string(aCriterionNumber) + std::string(" Value");
    auto tStageTag  = std::string("criterion_value_") + std::to_string(aCriterionNumber);

    auto tStageNode = aParentNode.append_child("Stage");
    XMLGen::append_children( { "StageTag", "StageName" }, { tStageTag, tStageName }, tStageNode);

    auto tForNode = tStageNode.append_child("For");
    XMLGen::append_attributes({"var", "in"}, {"I", "Parameters"}, tForNode);
    auto tOutputNode = tForNode.append_child("Output");
    XMLGen::append_children({ "SharedDataName" }, { aIdentifierString + std::string("_{I}") }, tOutputNode);
 }

/******************************************************************************/
void append_constraint_criteria_stage_options
(const XMLGen::InputData& aMetaData,
 pugi::xml_node& aParentNode,
 int& aCriterionNumber)
 {
    for(auto& tConstraint : aMetaData.constraints)
    {
        std::string tCriterionID = tConstraint.criterion();
        std::string tServiceID = tConstraint.service();
        std::string tScenarioID = tConstraint.scenario();
        ConcretizedCriterion tConcretizedCriterion(tCriterionID,tServiceID,tScenarioID);
        auto tIdentifierString = XMLGen::get_concretized_criterion_identifier_string(tConcretizedCriterion);

        XMLGen::append_criterion_stage_options(aParentNode,tIdentifierString,aCriterionNumber);
        aCriterionNumber++;
    }
 }

}
// namespace XMLGen
