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
  /*
    XMLGen::OptimizationParameters tOptParams;
    std::vector<std::string> tDescriptors = {"slot_length", "slot_width", "slot_depth"};
    tOptParams.descriptors(tDescriptors);
    tOptParams.append("concurrent_evaluations", "2");
    tInputMetaData.set(tOptParams);
    // service parameters
    XMLGen::Service tServiceOne;
    tServiceOne.append("code", "gemma");
    tServiceOne.append("id", "1");
    tServiceOne.append("type", "system_call");
    tServiceOne.append("number_processors", "1");
    tServiceOne.append("input_file", "match.yaml");*/

    auto tServices = aMetaData.services();
    std::string tNumRanks = "1";
    std::string tGemmaInputFile; ///Where is this stored?

    for (unsigned int iService = 0 ; iService < tServices.size(); iService++)
        if(tServices[iService].code()=="gemma")
        {
            tNumRanks = tServices[iService].numberProcessors();
            tGemmaInputFile = tServices[iService].value("input_file");
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
}