#include "XMLGeneratorGemmaProblem.hpp"
#include "XMLGeneratorUtilities.hpp"
#include <sstream>

namespace XMLGen
{
XMLGeneratorProblem::XMLGeneratorProblem()
{

}


XMLGeneratorGemmaProblem::XMLGeneratorGemmaProblem(const InputData& aMetaData)
{

  // define optimization parameters 
    auto tServices = aMetaData.services();
    std::string tNumRanks = "1";
    std::string tGemmaInputFile; ///Where is this stored?

    for (unsigned int iService = 0 ; iService < tServices.size(); iService++)
        if(tServices[iService].code()=="gemma")
        {
            tNumRanks = tServices[iService].numberProcessors();
            tGemmaInputFile = "matched_power_balance.yaml" ; //tServices[iService].value("input_file");
        }

    std::vector<std::string> tDescriptors = aMetaData.optimization_parameters().descriptors(); 

    auto tCriteria = aMetaData.criteria();
    auto tEvaluations = std::stoi(aMetaData.optimization_parameters().concurrent_evaluations());
    for (int iEvaluation = 0; iEvaluation < tEvaluations; iEvaluation++)
    {
        std::stringstream tStringStreamEvaluation;
        tStringStreamEvaluation << iEvaluation;
        //Aprepro
        mOperations.push_back(new XMLGeneratorOperationAprepro(tGemmaInputFile,tDescriptors,tStringStreamEvaluation.str()));
        //Gemma call
        mOperations.push_back(new XMLGeneratorOperationGemmaMPISystemCall(tGemmaInputFile,tNumRanks,tStringStreamEvaluation.str()));
        bool tOneWaitPerEvaluation = false;
        for (int iCriteria = 0; iCriteria < tCriteria.size(); iCriteria++)
        {
            
            if(tCriteria[iCriteria].type()=="system_call")
            {
            std::string tCriteriaID = tCriteria[iCriteria].id();
            std::string tDataColumn = tCriteria[iCriteria].value("data_group");
            std::string tMathOperation = tCriteria[iCriteria].value("data_extraction_operation");
            std::string tDataFile = tCriteria[iCriteria].value("data_file");

            //Wait (only 1 per evaluation folder, but depends on the data file name)
            if(!tOneWaitPerEvaluation)
            {
                mOperations.push_back(new XMLGeneratorOperationWait("wait",tDataFile,tStringStreamEvaluation.str()));
                tOneWaitPerEvaluation=true;
            }
            //Harvest
            mOperations.push_back(new XMLGeneratorOperationHarvestDataFunction(tDataFile,tMathOperation,tDataColumn,tStringStreamEvaluation.str(),tCriteriaID));
            }
        }
    }
}
void XMLGeneratorGemmaProblem::write_plato_main(pugi::xml_document& aDocument)
{
    for( unsigned int tLoopInd = 0; tLoopInd < mOperations.size(); ++tLoopInd )
    {
        mOperations[tLoopInd]->write(aDocument);
        std::cout<<mOperations[tLoopInd]->name()<<std::endl;
    }

}
void XMLGeneratorGemmaProblem::create_evaluation_subdirectories_and_gemma_input(const InputData& aMetaData)
{
    //Create templated input deck for Gemma
    create_matched_power_balance_input_deck(aMetaData);
    //Move all into evaluation folders
    std::vector<std::string> tEvaluationFolders;
    auto tEvaluations = std::stoi(aMetaData.optimization_parameters().concurrent_evaluations());
    for (int iEvaluation = 0; iEvaluation < tEvaluations; iEvaluation++)
    {
        std::stringstream tStringStreamEvaluation;
        tStringStreamEvaluation <<"evaluations_"<< iEvaluation;
        tEvaluationFolders.push_back(tStringStreamEvaluation.str());
    }
    move_file_to_subdirectories("matched_power_balance.yaml",tEvaluationFolders);
}

void XMLGeneratorGemmaProblem::create_matched_power_balance_input_deck(const InputData& aMetaData)
{
    auto tScenario = aMetaData.scenarios();

    std::string tCavityRadius;
    std::string tCavityHeight;
    std::string tConductivity;
    std::string tFrequencyMin;
    std::string tFrequencyMax;
    std::string tFrequencyStep;

    for(unsigned int iScenario = 0 ; iScenario < tScenario.size(); iScenario++)
    if(tScenario[iScenario].physics() == "electromagnetics")
    {
        tCavityRadius = tScenario[iScenario].cavity_radius();
        tCavityHeight = tScenario[iScenario].cavity_height();
        tFrequencyMin = tScenario[iScenario].frequency_min();
        tFrequencyMax = tScenario[iScenario].frequency_max();
        tFrequencyStep = tScenario[iScenario].frequency_step();
        tConductivity = aMetaData.material(tScenario[iScenario].value("material")).attribute("conductivity");
    }

    std::ofstream tOutFile;
    tOutFile.open("matched_power_balance.yaml.template", std::ofstream::out | std::ofstream::trunc);
    tOutFile << "%YAML 1.1\n";
    tOutFile << "---\n\n";

    tOutFile << "Gemma-dynamic:\n\n";

    tOutFile << "  Global:\n";
    tOutFile << "    Description: Higgins cylinder\n";
    tOutFile << "    Solution type: power balance\n\n";

    tOutFile << "  Power balance: \n";
    tOutFile << "    Algorithm: matched bound\n";
    tOutFile << "    Radius: {" << tCavityRadius << "}\n";
    tOutFile << "    Height: {" << tCavityHeight << "}\n";
    tOutFile << "    Conductivity: {" << tConductivity << "}\n";
    tOutFile << "    Slot length: {" << "slot_length" << "}\n";
    tOutFile << "    Slot width: {" << "slot_width" << "}\n";
    tOutFile << "    Slot depth: {" << "slot_depth" << "}\n";
    tOutFile << "    Start frequency range: {" << tFrequencyMin << "}\n";
    tOutFile << "    End frequency range: {" << tFrequencyMax << "}\n";
    tOutFile << "    Frequency interval size: {" << tFrequencyStep << "}\n\n";;

    tOutFile << "...\n";
    tOutFile.close();
}

}