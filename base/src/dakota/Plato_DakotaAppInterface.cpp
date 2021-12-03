/*
//@HEADER
// *************************************************************************
//   Plato Engine v.1.0: Copyright 2018, National Technology & Engineering
//                    Solutions of Sandia, LLC (NTESS).
//
// Under the terms of Contract DE-NA0003525 with NTESS,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Sandia Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact the Plato team (plato3D-help@sandia.gov)
//
// *************************************************************************
//@HEADER
*/

/*
 * Plato_DakotaAppInterface.cpp
 *
 *  Created on: Aug 14, 2021
 */

// Dakota includes
#include "PRPMultiIndex.hpp"
#include "DakotaResponse.hpp"
#include "DakotaVariables.hpp"
#include "ParamResponsePair.hpp"

#include "Plato_Macros.hpp"
#include "Plato_Parser.hpp"
#include "Plato_DakotaAppInterface.hpp"

namespace Plato
{

DakotaAppInterface::DakotaAppInterface
(const Dakota::ProblemDescDB &aProblemDataBase, 
 Plato::Interface *aInterface) : 
     Dakota::DirectApplicInterface(aProblemDataBase),
     mInterface(aInterface),
     mDataMap(aInterface->getInputData())
{
}

int DakotaAppInterface::derived_map_if(const Dakota::String &if_name)
{
    THROWERR(std::string("derived_map_if is not supported with Plato Dakota Driver, Plato should always use wait_local_evaluations"))
    return 0; /* can be used the same way the updateProblem in Plato is used. */
}

int DakotaAppInterface::derived_map_ac(const Dakota::String &aAnalysisDriverName)
{
    THROWERR(std::string("derived_map_ac is not supported with Plato Dakota Driver, Plato should always use wait_local_evaluations"))
    return 0;
}

int DakotaAppInterface::derived_map_of(const Dakota::String &of_name)
{
    THROWERR(std::string("derived_map_of is not supported with Plato Dakota Driver, Plato should always use wait_local_evaluations"))
    return 0; /* can be used to call output functions in other apps. the function is called one per iteration */
}

void DakotaAppInterface::wait_local_evaluations(Dakota::PRPQueue &aParamResponsePairQueue)
{
    this->setStageData(aParamResponsePairQueue);
    this->evaluateStages();
    this->setDakotaOutputData(aParamResponsePairQueue);
}

void DakotaAppInterface::derived_map_asynch(const Dakota::ParamResponsePair& pair)
{
  // virtual function overwritten to enable call to wait_local_evaluations
}

void DakotaAppInterface::set_communicators_checks(int max_eval_concurrency)
{
  // virtual function overwritten to enable call to wait_local_evaluations
}

void DakotaAppInterface::setStageData(const Dakota::PRPQueue &aParamResponsePairQueue)
{
    this->setInitializeStageData(aParamResponsePairQueue);

    this->setEvaluationFlags(aParamResponsePairQueue);
    this->setCriterionStageData(aParamResponsePairQueue);

    this->setFinalizeStageData(aParamResponsePairQueue);
}

void DakotaAppInterface::setEvaluationFlags(const Dakota::PRPQueue &aParamResponsePairQueue)
{
    size_t tNumCriteria = this->determineNumberOfCriteria(aParamResponsePairQueue);
    mEvaluationFlags.resize(tNumCriteria);

    for(size_t tFlagIndex = 0; tFlagIndex < tNumCriteria; tFlagIndex++)
    {
        mEvaluationFlags[tFlagIndex] = NO_DATA;
    }

    for(size_t tCriterion = 0; tCriterion < tNumCriteria; tCriterion++)
    {
        for (auto tPRP : aParamResponsePairQueue)
        {
            const Dakota::Variables &tVariables = tPRP.variables();
            const Dakota::ActiveSet &tActiveSet = tPRP.active_set();
            Dakota::DirectApplicInterface::set_local_data(tVariables, tActiveSet);

            auto tDirectFnASV = Dakota::DirectApplicInterface::directFnASV[tCriterion];

            if(mEvaluationFlags[tCriterion] == NO_DATA)
                mEvaluationFlags[tCriterion] = static_cast<Plato::DakotaAppInterface::evaluationTypes>(tDirectFnASV);

            if(mEvaluationFlags[tCriterion] == GET_VALUE && (tDirectFnASV == GET_GRADIENT || tDirectFnASV == GET_GRADIENT_AND_VALUE))
                mEvaluationFlags[tCriterion] = GET_GRADIENT_AND_VALUE;

            if(mEvaluationFlags[tCriterion] == GET_GRADIENT && (tDirectFnASV == GET_VALUE || tDirectFnASV == GET_GRADIENT_AND_VALUE))
                mEvaluationFlags[tCriterion] = GET_GRADIENT_AND_VALUE;
      
            if(tDirectFnASV != GET_VALUE && tDirectFnASV != GET_GRADIENT && tDirectFnASV != GET_GRADIENT_AND_VALUE)
                 THROWERR(std::string("Invalid evaluation type requested. Valid evaluation types are: value, gradient, or gradient and value "))
        }
    }
}

size_t DakotaAppInterface::determineNumberOfCriteria(const Dakota::PRPQueue &aParamResponsePairQueue)
{
    Dakota::PRPQueueIter tFirstPRPIter = aParamResponsePairQueue.begin();
    const Dakota::Variables &tVariables = tFirstPRPIter->variables();
    const Dakota::ActiveSet &tActiveSet = tFirstPRPIter->active_set();
    Dakota::DirectApplicInterface::set_local_data(tVariables, tActiveSet);
    return Dakota::DirectApplicInterface::directFnASV.size();
}

void DakotaAppInterface::setInitializeStageData(const Dakota::PRPQueue &aParamResponsePairQueue)
{
    std::string tStageTag = "initialize";
    auto tStageName = mDataMap.getStageName(tStageTag);
    if (!tStageName.empty())
    {
        size_t tPrpIndex = 0;
        for (Dakota::PRPQueueIter tParamRespPairIter = aParamResponsePairQueue.begin();
                tParamRespPairIter != aParamResponsePairQueue.end(); tParamRespPairIter++, tPrpIndex++)
        {
            const Dakota::Variables &tVariables = tParamRespPairIter->variables();
            const Dakota::ActiveSet &tActiveSet = tParamRespPairIter->active_set();
            Dakota::DirectApplicInterface::set_local_data(tVariables, tActiveSet);

            this->setStageSharedData(tStageTag,tPrpIndex);
        }
    }
}

void DakotaAppInterface::setCriterionStageData(const Dakota::PRPQueue &aParamResponsePairQueue)
{
    auto tNumCriteria = mEvaluationFlags.size();

    size_t tPrpIndex = 0;
    for (Dakota::PRPQueueIter tParamRespPairIter = aParamResponsePairQueue.begin();
             tParamRespPairIter != aParamResponsePairQueue.end(); tParamRespPairIter++, tPrpIndex++)
    {
        const Dakota::Variables &tVariables = tParamRespPairIter->variables();
        const Dakota::ActiveSet &tActiveSet = tParamRespPairIter->active_set();
        Dakota::DirectApplicInterface::set_local_data(tVariables, tActiveSet);

        auto tNumActiveCriteria = Dakota::DirectApplicInterface::directFnASV.size();
        if (tNumActiveCriteria != tNumCriteria)
        {
            THROWERR(std::string("In setCriterionStageData: Number of active criteria for PRP ") + std::to_string(tPrpIndex) + std::string(" is different from PRP 0"))
        }

        for (size_t tCriterion=0; tCriterion < tNumActiveCriteria; tCriterion++)
        {
            if (mEvaluationFlags[tCriterion] == GET_VALUE) // value
            {
                std::string tStageTag = "criterion_value_" + std::to_string(tCriterion);
                this->setStageSharedData(tStageTag,tPrpIndex);
            }

            if (mEvaluationFlags[tCriterion] == GET_GRADIENT) // gradient
            {
                std::string tStageTag = "criterion_gradient_" + std::to_string(tCriterion);
                this->setStageSharedData(tStageTag,tPrpIndex);
            }

            if (mEvaluationFlags[tCriterion] == GET_GRADIENT_AND_VALUE) // value and gradient
            {
                std::string tStageTag = "criterion_value_" + std::to_string(tCriterion);
                this->setStageSharedData(tStageTag,tPrpIndex);
                tStageTag = "criterion_gradient_" + std::to_string(tCriterion);
                this->setStageSharedData(tStageTag,tPrpIndex);
            }
        
        }
    }
}

void DakotaAppInterface::setFinalizeStageData(const Dakota::PRPQueue &aParamResponsePairQueue)
{
    std::string tStageTag = "finalize";
    auto tStageName = mDataMap.getStageName(tStageTag);
    if (!tStageName.empty())
    {
        size_t tPrpIndex = 0;
        for (Dakota::PRPQueueIter tParamRespPairIter = aParamResponsePairQueue.begin();
                tParamRespPairIter != aParamResponsePairQueue.end(); tParamRespPairIter++, tPrpIndex++)
        {
            const Dakota::Variables &tVariables = tParamRespPairIter->variables();
            const Dakota::ActiveSet &tActiveSet = tParamRespPairIter->active_set();
            Dakota::DirectApplicInterface::set_local_data(tVariables, tActiveSet);

            this->setStageSharedData(tStageTag,tPrpIndex);
        }
    }
}

void DakotaAppInterface::setStageSharedData(const std::string aStageTag, const size_t aPrpIndex)
{
    if ( mDataMap.stageHasInputSharedData(aStageTag) )
    {
        this->setStageContinuousVarsInputSharedData(aStageTag, aPrpIndex);
        this->setStageDiscreteRealVarsInputSharedData(aStageTag, aPrpIndex);
        this->setStageDiscreteIntegerVarsInputSharedData(aStageTag, aPrpIndex);
    }
    if ( mDataMap.stageHasOutputSharedData(aStageTag) )
    {
        this->setStageOutputSharedData(aStageTag, aPrpIndex);
    }
}

void DakotaAppInterface::setStageContinuousVarsInputSharedData(const std::string &aStageTag, const size_t aPrpIndex)
{
    auto tDataNames = mDataMap.getInputSharedDataNames(aStageTag,"continuous");
    
    if (tDataNames.size() > 0)
    {
        std::string tName = tDataNames[aPrpIndex];
        mDataMap.setContinuousVarsSharedData(tName,Dakota::DirectApplicInterface::numACV,Dakota::DirectApplicInterface::xC);
        mParameterList.set(tName, mDataMap.getContinuousVarsSharedData(tName).data());
    }
}

void DakotaAppInterface::setStageDiscreteRealVarsInputSharedData(const std::string &aStageTag, const size_t aPrpIndex)
{
    auto tDataNames = mDataMap.getInputSharedDataNames(aStageTag,"discrete real");
    
    if (tDataNames.size() > 0)
    {
        std::string tName = tDataNames[aPrpIndex];
        mDataMap.setDiscreteRealVarsSharedData(tName,Dakota::DirectApplicInterface::numADRV,Dakota::DirectApplicInterface::xDR);
        mParameterList.set(tName, mDataMap.getDiscreteRealVarsSharedData(tName).data());
    }
}

void DakotaAppInterface::setStageDiscreteIntegerVarsInputSharedData(const std::string &aStageTag, const size_t aPrpIndex)
{
    auto tDataNames = mDataMap.getInputSharedDataNames(aStageTag,"discrete integer");
    
    if (tDataNames.size() > 0)
    {
        std::string tName = tDataNames[aPrpIndex];
        mDataMap.setDiscreteIntegerVarsSharedData(tName,Dakota::DirectApplicInterface::numADIV,Dakota::DirectApplicInterface::xDI);
        mParameterList.set(tName, mDataMap.getDiscreteIntegerVarsSharedData(tName).data());
    }
}

void DakotaAppInterface::setStageOutputSharedData(const std::string &aStageTag, const size_t aPrpIndex)
{
    auto tSharedDataNames = mDataMap.getOutputSharedDataNames(aStageTag);
    if (tSharedDataNames.size() > 0)
    {
        auto tSharedDataName = tSharedDataNames[aPrpIndex];
        Plato::dakota::SharedDataMetaData tMetaData;
        tMetaData.mName = tSharedDataName;
        tMetaData.mValues.resize(1u,0.0);
        mDataMap.setOutputVarsSharedData(aStageTag,tMetaData,aPrpIndex);
        mParameterList.set(tSharedDataName, mDataMap.getOutputVarsSharedData(aStageTag,aPrpIndex).mValues.data());
    }
}

void DakotaAppInterface::evaluateStages()
{
    std::vector<std::string> tStageNames;
    this->setStageNamesForEvaluation(tStageNames);
    mInterface->compute(tStageNames, mParameterList);
}

void DakotaAppInterface::setStageNamesForEvaluation(std::vector<std::string> & aStageNames)
{
    this->setInitializeStageNameForEvaluation(aStageNames);
    this->setCriterionStageNamesForEvaluation(aStageNames);
    this->setFinalizeStageNameForEvaluation(aStageNames);
}

void DakotaAppInterface::setInitializeStageNameForEvaluation(std::vector<std::string> & aStageNames)
{
    auto tStageName = mDataMap.getStageName("initialize");
    if (!tStageName.empty())
    {
        aStageNames.push_back(tStageName);
    }
}

void DakotaAppInterface::setCriterionStageNamesForEvaluation(std::vector<std::string> & aStageNames)
{
    std::vector<std::string> tStageTagList = mDataMap.getStageTags();
    for (std::string tStageTag : tStageTagList)
    {
        auto tTagTokens = Plato::tokenize(tStageTag);
        if (tTagTokens[0] == "criterion")
        {
            std::string tStageName = mDataMap.getStageName(tStageTag);
            aStageNames.push_back(tStageName);
        }
    }
}

void DakotaAppInterface::setFinalizeStageNameForEvaluation(std::vector<std::string> & aStageNames)
{
    auto tStageName = mDataMap.getStageName("finalize");
    if (!tStageName.empty())
    {
        aStageNames.push_back(tStageName);
    }
}

void DakotaAppInterface::setDakotaOutputData(const Dakota::PRPQueue &aParamResponsePairQueue)
{
    auto tNumCriteria = mEvaluationFlags.size();

    size_t tPrpIndex = 0;
    for (Dakota::PRPQueueIter tParamRespPairIter = aParamResponsePairQueue.begin();
             tParamRespPairIter != aParamResponsePairQueue.end(); tParamRespPairIter++, tPrpIndex++)
    {
        const Dakota::Variables &tVariables = tParamRespPairIter->variables();
        const Dakota::ActiveSet &tActiveSet = tParamRespPairIter->active_set();
        Dakota::DirectApplicInterface::set_local_data(tVariables, tActiveSet);

        auto tNumActiveCriteria = Dakota::DirectApplicInterface::directFnASV.size();
        if (tNumActiveCriteria != tNumCriteria)
        {
            THROWERR(std::string("In setDakotaOutputData: Number of active criteria for PRP ") + std::to_string(tPrpIndex) + std::string(" is different from PRP 0"))
        }

        auto tEvaluationID = tParamRespPairIter->eval_id();
        Dakota::Response tResponse = tParamRespPairIter->response();
        Dakota::RealVector tDakotaFunVals = tResponse.function_values_view();
        Dakota::RealMatrix tDakotaFunGrads = tResponse.function_gradients_view();

        for (size_t tCriterion=0; tCriterion < tNumActiveCriteria; tCriterion++)
        {
            if (mEvaluationFlags[tCriterion] == GET_VALUE) // value
            {
                this->setValueOutputs(tCriterion,tPrpIndex,tDakotaFunVals);
            }

            if (mEvaluationFlags[tCriterion] == GET_GRADIENT) // gradient
            {
                this->setGradientOutputs(tCriterion,tPrpIndex,tDakotaFunGrads);
            }

            if (mEvaluationFlags[tCriterion] == GET_GRADIENT_AND_VALUE) // value and gradient
            {
                this->setValueOutputs(tCriterion,tPrpIndex,tDakotaFunVals);
                this->setGradientOutputs(tCriterion,tPrpIndex,tDakotaFunGrads);
            }
        
        }
        Dakota::DirectApplicInterface::completionSet.insert(tEvaluationID);
    }
}

void DakotaAppInterface::setValueOutputs(const size_t aCriterion, const size_t aPrpIndex, Dakota::RealVector & aDakotaFunVals)
{
    std::string tStageTag = "criterion_value_" + std::to_string(aCriterion);

    aDakotaFunVals[aCriterion] = mDataMap.getOutputVarsSharedData(tStageTag,aPrpIndex).mValues.front();
}

void DakotaAppInterface::setGradientOutputs(const size_t aCriterion, const size_t aPrpIndex, Dakota::RealMatrix & aDakotaFunGrads)
{
    std::string tStageTag = "criterion_gradient_" + std::to_string(aCriterion);
    for (size_t tVarIndex = 0; tVarIndex < Dakota::DirectApplicInterface::numDerivVars; tVarIndex++)
    {
        aDakotaFunGrads[aCriterion][tVarIndex] = mDataMap.getOutputVarsSharedData(tStageTag,aPrpIndex).mValues[tVarIndex];
    }
}

}
// namespace Plato
