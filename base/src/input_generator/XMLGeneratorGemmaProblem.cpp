#include "XMLGeneratorGemmaProblem.hpp"
#include "XMLGeneratorParserUtilities.hpp"
#include "XMLGeneratorUtilities.hpp"
#include <sstream>

namespace XMLGen
{
XMLGeneratorProblem::XMLGeneratorProblem()
{
   // mPerformers.push_back(std::make_shared<XMLGeneratorPerformer>("0","PlatoMain","PlatoMain"));
}

void XMLGeneratorProblem::addPlatoServicesPerformers(int aStartID, int aEndID)
{
    /*
    for(int iID = aStartID; iID < aEndID ; iID++)
    {
        std::stringstream tStringStreamID;
        tStringStreamID << iID;
        std::stringstream tStringStreamIDMinusOne;
        tStringStreamIDMinusOne << (iID-1);
        
        mPerformers.push_back(std::make_shared<XMLGeneratorPerformer>(tStringStreamID.str(),"plato_services_"+tStringStreamIDMinusOne.str(),"plato_services"));
    }*/
}

XMLGeneratorGemmaProblem::XMLGeneratorGemmaProblem(const InputData& aMetaData) : XMLGeneratorProblem()
{
    /*
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

    addPlatoServicesPerformers(1,tEvaluations+1);
    int tNumParams = aMetaData.optimization_parameters().descriptors().size();
    addSharedDataPairs(tNumParams,tEvaluations);

    for (int iEvaluation = 0; iEvaluation < tEvaluations; iEvaluation++)
    {
        std::stringstream tStringStreamEvaluation;
        tStringStreamEvaluation << iEvaluation;
        //Aprepro
        mOperations.push_back(std::make_shared<XMLGeneratorOperationAprepro>(tGemmaInputFile,tDescriptors,tStringStreamEvaluation.str()));
        //Gemma call
        mOperations.push_back(std::make_shared<XMLGeneratorOperationGemmaMPISystemCall>(tGemmaInputFile,tNumRanks,tStringStreamEvaluation.str()));
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
                    mOperations.push_back(std::make_shared<XMLGeneratorOperationWait>("wait",tDataFile,tStringStreamEvaluation.str()));
                    tOneWaitPerEvaluation=true;
                }
                //Harvest
                mOperations.push_back(std::make_shared<XMLGeneratorOperationHarvestDataFunction>(tDataFile,tMathOperation,tDataColumn,tStringStreamEvaluation.str(),tCriteriaID));
            }
        }
    }*/
}

void XMLGeneratorGemmaProblem::write_plato_main(pugi::xml_document& aDocument)
{
    for( unsigned int tLoopInd = 0; tLoopInd < mOperations.size(); ++tLoopInd )
    {
        mOperations[tLoopInd]->write_definition(aDocument);
        std::cout<<mOperations[tLoopInd]->name()<<std::endl;
    }

}

void XMLGeneratorGemmaProblem::write_interface(pugi::xml_document& aDocument)
{
    auto tConsoleNode = aDocument.append_child("Console");
    addChild(tConsoleNode, "Enabled", "false");  /// These should belong to a XMLGeneratorProblem 
    addChild(tConsoleNode, "Verbose", "true");

    for( unsigned int tLoopInd = 0; tLoopInd < mPerformers.size(); ++tLoopInd )
    {
        mPerformers[tLoopInd]->write(aDocument);
        std::cout<<mPerformers[tLoopInd]->name()<<std::endl;
    }
    for( unsigned int tLoopInd = 0; tLoopInd < mSharedData.size(); ++tLoopInd )
    {
        mSharedData[tLoopInd]->write(aDocument);
        std::cout<<mSharedData[tLoopInd]->name()<<std::endl;
    }
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

void XMLGeneratorGemmaProblem::addSharedDataPairs(int aNumParameters, int aEvaluations)
{
    /*
    std::stringstream tNumParams;
    tNumParams<<aNumParameters;
    for(unsigned int iEvaluation = 0 ; iEvaluation < aEvaluations; iEvaluation++)
    {
        std::stringstream tEvaluation;
        tEvaluation<<iEvaluation;
        std::vector<std::string> tUserName = {"PlatoMain", std::string("plato_services_") + tEvaluation.str()} ;

        mSharedData.push_back(std::make_shared<XMLGeneratorSharedData>
        (std::string("parameters_")+tEvaluation.str(),tNumParams.str(),"PlatoMain",tUserName));
    }
    
    for(unsigned int iEvaluation = 0 ; iEvaluation < aEvaluations; iEvaluation++)
    {
        std::stringstream tEvaluation;
        tEvaluation<<iEvaluation;
        std::string tOwnerName = std::string("plato_services_")+tEvaluation.str();
        std::vector<std::string> tUserName= {"PlatoMain"};
        mSharedData.push_back(
            std::make_shared<XMLGeneratorSharedData>(std::string("objective_value_")+tEvaluation.str(),"1",tOwnerName,tUserName)
            );
    }
    */
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