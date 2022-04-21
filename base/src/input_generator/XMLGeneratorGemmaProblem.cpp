#include "XMLGeneratorGemmaProblem.hpp"
#include "XMLGeneratorParserUtilities.hpp"
#include "XMLGeneratorUtilities.hpp"
#include <fstream>
#include <sstream>

namespace XMLGen
{
XMLGeneratorProblem::XMLGeneratorProblem()
{
    mPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain","platomain");
    
    mInterfaceFileName = "interface.xml";
    mOperationsFileName = "plato_main_operations.xml";
    mInputDeckName = "plato_main_input_deck.xml";
    mMPISourceName = "mpirun.source";
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

    std::string tDataColumn, tMathOperation, tDataFile;
    auto tCriteria = aMetaData.criteria();
    for (int iCriteria = 0; iCriteria < tCriteria.size(); iCriteria++)
    if(tCriteria[iCriteria].type() == "system_call")
    {
        tDataColumn = tCriteria[iCriteria].value("data_group");
        tMathOperation = tCriteria[iCriteria].value("data_extraction_operation");
        tDataFile = tCriteria[iCriteria].value("data_file");
    }

    std::vector<std::string> tDescriptors = aMetaData.optimization_parameters().descriptors();
    
    auto tEvaluations = std::stoi(aMetaData.optimization_parameters().concurrent_evaluations());
    int tNumParams = aMetaData.optimization_parameters().descriptors().size();
    
    mPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",std::stoi(tNumRanks),tEvaluations);
    
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {mPerformerMain,mPerformer};
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformersJustMain = {mPerformerMain};

    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tInputSharedData = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("design_parameters","3",mPerformerMain,tUserPerformers,tEvaluations);
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tOutputSharedData = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("criterion value","1",mPerformer,tUserPerformersJustMain,tEvaluations);
    
    mSharedData.push_back(tInputSharedData);
    mSharedData.push_back(tOutputSharedData);

    std::shared_ptr<XMLGen::XMLGeneratorOperationAprepro> tAprepro = std::make_shared<XMLGen::XMLGeneratorOperationAprepro>(tGemmaInputFile , tDescriptors, tInputSharedData, mPerformer, tEvaluations );
    std::shared_ptr<XMLGen::XMLGeneratorOperationGemmaMPISystemCall> tGemma = std::make_shared<XMLGen::XMLGeneratorOperationGemmaMPISystemCall>(tGemmaInputFile , tNumRanks, mPerformer, tEvaluations);
    std::shared_ptr<XMLGen::XMLGeneratorOperationWait> tWait = std::make_shared<XMLGen::XMLGeneratorOperationWait>("wait", tDataFile, mPerformer, tEvaluations);
    std::shared_ptr<XMLGen::XMLGeneratorOperationHarvestDataFunction> tHarvestData = std::make_shared<XMLGen::XMLGeneratorOperationHarvestDataFunction>(tDataFile, tMathOperation, tDataColumn, tOutputSharedData, mPerformer, tEvaluations);

    mOperations.push_back(tAprepro);
    mOperations.push_back(tGemma);
    mOperations.push_back(tWait);
    mOperations.push_back(tHarvestData);

    XMLGen::XMLGeneratorStage tInitializeStage("Initialize",{tAprepro},tInputSharedData,tOutputSharedData);
    
    XMLGen::XMLGeneratorStage tCriterionStage("Criterion Value",{tGemma,tWait,tHarvestData},tInputSharedData,tOutputSharedData);

    mStages.push_back(tInitializeStage);
    mStages.push_back(tCriterionStage);


  //  mDakotaStage = std::make_shared<XMLGeneratorStage>("Dakota",tInputSharedData,tOutputSharedData);
    
}

void XMLGeneratorGemmaProblem::write_plato_main(pugi::xml_document& aDocument)
{
    for( unsigned int tLoopInd = 0; tLoopInd < mOperations.size(); ++tLoopInd )
    {
        for(unsigned int iEvaluations = 0; iEvaluations < mOperations[tLoopInd]->evaluations(); ++iEvaluations)
        mOperations[tLoopInd]->write_definition(aDocument,std::to_string(iEvaluations));
    }

}

void XMLGeneratorGemmaProblem::write_interface(pugi::xml_document& aDocument)
{
    auto tConsoleNode = aDocument.append_child("Console");
    addChild(tConsoleNode, "Enabled", "false");  /// These should belong to a XMLGeneratorProblem 
    addChild(tConsoleNode, "Verbose", "true");

    mPerformerMain->write_interface(aDocument);
    for(unsigned int iEvaluation = 0; iEvaluation < mPerformer->evaluations(); ++iEvaluation)
    mPerformer->write_interface(aDocument,1,std::to_string(iEvaluation));
    
    for( unsigned int tLoopInd = 0; tLoopInd < mSharedData.size(); ++tLoopInd )
    {
        if(mSharedData[tLoopInd]->evaluations()==0)
            mSharedData[tLoopInd]->write_interface(aDocument);
        else
        {
            auto tForNode = aDocument.append_child("For");
            tForNode.append_attribute("var") = "E";
            tForNode.append_attribute("in") = "Parameters";
            mSharedData[tLoopInd]->write_interface(tForNode);  
        }
    }
    
    int tDakotaIndexInfo = -1;
    for(unsigned int iStage = 0 ; iStage < mStages.size(); ++iStage)
    {
        mStages[iStage].write(aDocument); 
        if(mStages[iStage].name() == "Criterion Value")
            tDakotaIndexInfo = iStage;
    }
    //Write dakota stage
    //This does not appear to fit the pattern, so I will semi-hard code this here:
    //auto tDakotaNode aDocument.append_child("DakotaDriver");
    //auto tDakotaStage tDakotaNode.append_child("Stage");
    //addChild(tDakotaStage, "StageTag", mStages[tDakotaIndexInfo].name());
    
/*
    <DakotaDriver>

    <Stage>

        <StageTag>criterion_value_0</StageTag>
        <StageName>Criterion 0 Value</StageName>

        <For var="PerformerIndex" in="Performers">
        <Input>
            <Tag>continuous</Tag>
            <SharedDataName>parameters_{PerformerIndex}</SharedDataName>
        </Input>
        </For>

        <For var="PerformerIndex" in="Performers">
        <Output>
            <SharedDataName>objective_value_{PerformerIndex}</SharedDataName>
        </Output>
        </For>

    </Stage>

    </DakotaDriver>
    */

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

void XMLGeneratorGemmaProblem::write_mpisource(std::string aFileName)
{
    std::ofstream tOutFile(aFileName.c_str());
    tOutFile<<"mpirun --oversubscribe -np 1 -x PLATO_PERFORMER_ID=0 \\\n";
    tOutFile<<"-x PLATO_INTERFACE_FILE="<<mInterfaceFileName<<" \\\n";
    tOutFile<<"-x PLATO_APP_FILE="<<mOperationsFileName<<" \\\n";
    tOutFile<<"PlatoMain "<<mInputDeckName<<" \\\n";

    //TODO Number of processors for a service engine....
    for(int iPerformer = 0; iPerformer < mPerformer->evaluations(); ++iPerformer )
    {
        tOutFile<<": -np 1 -x PLATO_PERFORMER_ID="<<mPerformer->ID(1,std::to_string(iPerformer))<<" \\\n";
        tOutFile<<"-x PLATO_INTERFACE_FILE="<<mInterfaceFileName<<" \\\n";
        tOutFile<<"-x PLATO_APP_FILE="<<mOperationsFileName<<" \\\n";
        tOutFile<<"PlatoEngineServices "<<mInputDeckName<<" \\\n";
    }
    tOutFile.close();
}

}