#include "XMLGeneratorGemmaProblem.hpp"
#include "XMLGeneratorParserUtilities.hpp"
#include "XMLGeneratorUtilities.hpp"
#include <sstream>

namespace XMLGen
{
XMLGeneratorProblem::XMLGeneratorProblem()
{
    mPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain",0);
}

XMLGeneratorGemmaProblem::XMLGeneratorGemmaProblem(const InputData& aMetaData) : XMLGeneratorProblem()
{
    
    auto tServices = aMetaData.services();
    std::string tNumRanks = "1";
    std::string tGemmaInputFile; ///Where is this stored?

    for (unsigned int iService = 0 ; iService < tServices.size(); iService++)
    if(tServices[iService].code()=="gemma")
    {
        tNumRanks = tServices[iService].numberProcessors();
        tGemmaInputFile = "matched_power_balance.yaml" ; //tServices[iService].value("input_file");
    }

    std::string tDataColumn;
    std::string tMathOperation;
    std::string tDataFile;
    auto tCriteria = aMetaData.criteria();
    for (int iCriteria = 0; iCriteria < tCriteria.size(); iCriteria++)
    if(tCriteria[iCriteria].type()=="system_call")
    {
        tDataColumn = tCriteria[iCriteria].value("data_group");
        tMathOperation = tCriteria[iCriteria].value("data_extraction_operation");
        tDataFile = tCriteria[iCriteria].value("data_file");
    }

    std::vector<std::string> tDescriptors = aMetaData.optimization_parameters().descriptors();
    
    auto tEvaluations = std::stoi(aMetaData.optimization_parameters().concurrent_evaluations());
    int tNumParams = aMetaData.optimization_parameters().descriptors().size();
    
    mPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",tEvaluations);
    
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {mPerformerMain,mPerformer};
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformersJustMain = {mPerformerMain};

    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tInputSharedData = std::make_shared<XMLGen::XMLGeneratorSharedData>("design_parameters","3",mPerformerMain,tUserPerformers,tEvaluations);
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tOutputSharedData = std::make_shared<XMLGen::XMLGeneratorSharedData>("criterion value","1",mPerformer,tUserPerformersJustMain,tEvaluations);
    
    mSharedData.push_back(tInputSharedData);
    mSharedData.push_back(tOutputSharedData);

    std::shared_ptr<XMLGen::XMLGeneratorOperationAprepro> tAprepro = std::make_shared<XMLGen::XMLGeneratorOperationAprepro>(tGemmaInputFile , tDescriptors, tInputSharedData, mPerformer, tEvaluations );
    std::shared_ptr<XMLGen::XMLGeneratorOperationGemmaMPISystemCall> tGemma = std::make_shared<XMLGen::XMLGeneratorOperationGemmaMPISystemCall>(tGemmaInputFile , tNumRanks, mPerformer, tEvaluations);
    std::shared_ptr<XMLGen::XMLGeneratorOperationWait> tWait = std::make_shared<XMLGen::XMLGeneratorOperationWait>("Wait", tDataFile, mPerformer, tEvaluations);
    std::shared_ptr<XMLGen::XMLGeneratorOperationHarvestDataFunction> tHarvestData = std::make_shared<XMLGen::XMLGeneratorOperationHarvestDataFunction>(tDataFile, tMathOperation, tDataColumn, tOutputSharedData, mPerformer, tEvaluations);

    mOperations.push_back(tAprepro);
    mOperations.push_back(tGemma);
    mOperations.push_back(tWait);
    mOperations.push_back(tHarvestData);

    XMLGen::XMLGeneratorStage tInitializeStage("Initialize",tInputSharedData,tOutputSharedData);
    tInitializeStage.addStageOperation(tAprepro);

    XMLGen::XMLGeneratorStage tCriterionStage("Criterion Value",tInputSharedData,tOutputSharedData);
    tCriterionStage.addStageOperation(tGemma);
    tCriterionStage.addStageOperation(tWait);
    tCriterionStage.addStageOperation(tHarvestData);
    
}

void XMLGeneratorGemmaProblem::write_plato_main(pugi::xml_document& aDocument)
{
    for( unsigned int tLoopInd = 0; tLoopInd < mOperations.size(); ++tLoopInd )
    {
        for(unsigned int iEvaluations = 0; iEvaluations < mOperations[tLoopInd]->evaluations(); ++iEvaluations)
        mOperations[tLoopInd]->write_definition(aDocument,std::to_string(iEvaluations));
        std::cout<<mOperations[tLoopInd]->name()<<std::endl;
    }

}

void XMLGeneratorGemmaProblem::write_interface(pugi::xml_document& aDocument)
{
    auto tConsoleNode = aDocument.append_child("Console");
    addChild(tConsoleNode, "Enabled", "false");  /// These should belong to a XMLGeneratorProblem 
    addChild(tConsoleNode, "Verbose", "true");

    mPerformerMain->write(aDocument);
    mPerformer->write(aDocument);
    
    for( unsigned int tLoopInd = 0; tLoopInd < mSharedData.size(); ++tLoopInd )
    {
        mSharedData[tLoopInd]->write(aDocument);
        std::cout<<mSharedData[tLoopInd]->name()<<std::endl;
    }
    for(unsigned int iStage = 0 ; iStage < mStages.size(); ++iStage)
        mStages[iStage].write(aDocument); ///NEED FOR LOOP WRAPPER 

    //Write dakota stage
}

void XMLGeneratorGemmaProblem::create_evaluation_subdirectories_and_gemma_input(const InputData& aMetaData)
{
    create_matched_power_balance_input_deck(aMetaData);

    std::vector<std::string> tEvaluationFolders;
    auto tEvaluations = std::stoi(aMetaData.optimization_parameters().concurrent_evaluations());
    for (int iEvaluation = 0; iEvaluation < tEvaluations; iEvaluation++)
    {
        std::string tEvaluation = "evaluations_" + std::to_string(iEvaluation);
        tEvaluationFolders.push_back(tEvaluation);
    }
    XMLGen::move_file_to_subdirectories("gemma_matched_power_balance_input_deck.yaml.template", tEvaluationFolders);
}

void XMLGeneratorGemmaProblem::create_matched_power_balance_input_deck(const InputData& aMetaData)
{
    auto tScenario = aMetaData.scenarios();

    std::string tCavityRadius, tCavityHeight, tConductivity, tFrequencyMin, tFrequencyMax, tFrequencyStep;

    for(unsigned int iScenario = 0 ; iScenario < tScenario.size(); iScenario++)
    if(tScenario[iScenario].physics() == "electromagnetics")
    {
        tCavityRadius = tScenario[iScenario].cavity_radius();
        tCavityHeight = tScenario[iScenario].cavity_height();
        tFrequencyMin = tScenario[iScenario].frequency_min();
        tFrequencyMax = tScenario[iScenario].frequency_max();
        tFrequencyStep = tScenario[iScenario].frequency_step();
        tConductivity = aMetaData.material(tScenario[iScenario].value("material")).property("conductivity");
    }

    std::ofstream tOutFile;
    tOutFile.open("gemma_matched_power_balance_input_deck.yaml.template", std::ofstream::out | std::ofstream::trunc);
    tOutFile << "%YAML 1.1\n";
    tOutFile << "---\n\n";

    tOutFile << "Gemma-dynamic:\n\n";

    tOutFile << "  Global:\n";
    tOutFile << "    Description: Higgins cylinder\n";
    tOutFile << "    Solution type: power balance\n\n";

    tOutFile << "  Power balance: \n";
    tOutFile << "    Algorithm: matched bound\n";
    tOutFile << "    Radius: " << tCavityRadius << "\n";
    tOutFile << "    Height: " << tCavityHeight << "\n";
    tOutFile << "    Conductivity: " << tConductivity << "\n";
    tOutFile << "    Slot length: {" << "slot_length" << "}\n";
    tOutFile << "    Slot width: {" << "slot_width" << "}\n";
    tOutFile << "    Slot depth: {" << "slot_depth" << "}\n";
    tOutFile << "    Start frequency range: " << tFrequencyMin << "\n";
    tOutFile << "    End frequency range: " << tFrequencyMax << "\n";
    tOutFile << "    Frequency interval size: " << tFrequencyStep << "\n\n";;

    tOutFile << "...\n";
    tOutFile.close();
}

}