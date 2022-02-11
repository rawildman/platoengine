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
 * Plato_SystemCall.cpp
 *
 *  Created on: Jun 29, 2019
 */

#include <cstdlib>
#include <sstream>

#include "Plato_Parser.hpp"
#include "Plato_Console.hpp"
#include "Plato_InputData.hpp"
#include "Plato_Exceptions.hpp"
#include "Plato_SystemCall.hpp"
#include "Plato_OperationsUtilities.hpp"
#include <Plato_FreeFunctions.hpp>
#include "Plato_Macros.hpp"

namespace Plato
{
/******************************************************************************/
void SystemCall::getArguments(std::vector<Plato::LocalArg> & aLocalArgs)
/******************************************************************************/
{
    for(auto& tInputName : mInputNames) {
        aLocalArgs.push_back(Plato::LocalArg(Plato::data::layout_t::SCALAR, tInputName));
    }
}

/******************************************************************************/
SystemCall::SystemCall(const Plato::InputData & aNode, const Plato::SystemCallMetadata& aMetaData)
/******************************************************************************/
{
    // set basic info
    mName = Plato::Get::String(aNode, "Name");
    mOnChange = Plato::Get::Bool(aNode, "OnChange");
    mAppendInput = Plato::Get::Bool(aNode, "AppendInput");
    mStringCommand = Plato::Get::String(aNode, "Command");
    
    this->setInputSharedDataNames(aNode);
    this->setArguments(aNode);
    this->setOptions(aNode,aMetaData);
}

void SystemCall::setOptions(const Plato::InputData& aNode, const Plato::SystemCallMetadata& aMetaData)
{
    auto tNumOptions = aNode.getByName<std::string>("Option").size();

    auto tTotalNumParameters = this->countTotalNumParameters(aMetaData);
    if(tNumOptions > tTotalNumParameters)
    {
        THROWERR(std::string("Number of options cannot exceed the number of total input parameters. ") 
            + "The number of options is set to '" + std::to_string(tNumOptions) 
            + "' while the number of input parameters is set to '" + std::to_string(tTotalNumParameters) + ".")
    }
    mOptions = std::vector<std::string>(tTotalNumParameters, "");
    
    size_t tIndex = 0;
    for(auto tStrValue : aNode.getByName<std::string>("Option"))
    {
        mOptions[tIndex++] = tStrValue;
    }
}

void SystemCall::setInputSharedDataNames(const Plato::InputData& aNode)
{
    for(Plato::InputData& tInputNode : aNode.getByName<Plato::InputData>("Input"))
    {
        mInputNames.push_back(Plato::Get::String(tInputNode, "ArgumentName"));
    }
}

size_t SystemCall::countTotalNumParameters(const Plato::SystemCallMetadata& aMetaData) const
{
    size_t tTotalNumParameters = 0;
    for(auto& tInputArgumentPair : aMetaData.mInputArgumentMap)
    {
        tTotalNumParameters += tInputArgumentPair.second->size();
    }
    return tTotalNumParameters;
}

void SystemCall::setArguments(const Plato::InputData& aNode)
{
    // set arguments
    for(auto& tStrValue : aNode.getByName<std::string>("Argument"))
    {
        mArguments.push_back(tStrValue);
    }
}

bool SystemCall::checkForLocalParameterChanges(const Plato::SystemCallMetadata& aMetaData)
{
    bool tDidParametersChanged = false;
    for(size_t tIndexJ=0; tIndexJ<mInputNames.size(); ++tIndexJ)
    {
        bool tLocalChanged = false;
        Plato::Console::Status("PlatoMain: On Change SystemCall -- \"" + mInputNames[tIndexJ] + "\" Values:");
        auto tInputArgument = aMetaData.mInputArgumentMap.at(mInputNames[tIndexJ]);
        for(size_t tIndexI=0; tIndexI<tInputArgument->size(); ++tIndexI)
        {
            double tSavedValue = mSavedParameters[tIndexJ][tIndexI];
            double tCurrentValue = tInputArgument->data()[tIndexI];
            if(tSavedValue != tCurrentValue)
            {
                double tPercentChange = (fabs(tSavedValue-tCurrentValue)/fabs(tSavedValue))*100.0;
                char tPercentChangeString[20];
                sprintf(tPercentChangeString, "%.1lf", tPercentChange);
                Plato::Console::Status("Saved: " + std::to_string(mSavedParameters[tIndexJ][tIndexI]) 
                                       + "; \tCurrent: " + std::to_string(tInputArgument->data()[tIndexI]) 
                                       + " \t-- CHANGED " + tPercentChangeString + "%");
                tLocalChanged = true;
                tDidParametersChanged = true;
            }
            else
            {
                Plato::Console::Status("Saved: " + std::to_string(mSavedParameters[tIndexJ][tIndexI]) + "; \tCurrent: " 
                                       + std::to_string(tInputArgument->data()[tIndexI]) + " \t-- NO CHANGE");
            }
        }
        if(tLocalChanged == true)
        {
            mSavedParameters[tIndexJ] = *(tInputArgument);
        }
    }
    return tDidParametersChanged;
}

void SystemCall::performSystemCall(const Plato::SystemCallMetadata& aMetaData)
{
    std::vector<std::string> tArguments;
    for(auto& tArgumentName : mArguments)
    {
        tArguments.push_back(tArgumentName);
    }
    if(mAppendInput)
    {
        this->appendOptionsAndValues(aMetaData,tArguments);
    }
    this->executeCommand(tArguments);
}

void SystemCall::appendOptionsAndValues(const Plato::SystemCallMetadata& aMetaData, std::vector<std::string>& aArguments)
{
    for(auto& tInputName : mInputNames)
    {
        auto tInputArgument = aMetaData.mInputArgumentMap.at(tInputName);
        for(size_t tIndex=0; tIndex<tInputArgument->size(); ++tIndex)
        {
            std::stringstream tDataString;
            tDataString << std::setprecision(16) << mOptions[tIndex] << tInputArgument->data()[tIndex];
            aArguments.push_back(tDataString.str());
        }
    }
}

void SystemCall::saveParameters(const Plato::SystemCallMetadata& aMetaData)
{
    for(size_t tIndexJ=0; tIndexJ<mInputNames.size(); ++tIndexJ)
    {
        Plato::Console::Status("PlatoMain: On Change SystemCall -- \"" + mInputNames[tIndexJ] + "\" Values:");
        auto tInputArgument = aMetaData.mInputArgumentMap.at(mInputNames[tIndexJ]);


        std::vector<double> tCurVector(tInputArgument->size());
        for(size_t tIndexI=0; tIndexI<tInputArgument->size(); ++tIndexI)
        {
            tCurVector[tIndexI] = tInputArgument->data()[tIndexI];
            Plato::Console::Status("Saved: Not set yet; Current: " + std::to_string(tCurVector[tIndexI]));
        }
        mSavedParameters.push_back(tCurVector);
    }
}

/******************************************************************************/
void SystemCall::operator()(const Plato::SystemCallMetadata& aMetaData)
/******************************************************************************/
{
    bool tChanged = true;
    if(mOnChange)
    {
        // If we haven't saved any parameters yet save them now and set "changed" to true.
        if(mSavedParameters.size() == 0)
        {
            this->saveParameters(aMetaData);
        }
        else
        {
            tChanged = this->checkForLocalParameterChanges(aMetaData);
        }
    }

    if(tChanged)
    {
        Plato::Console::Status("PlatoMain: SystemCall -- Condition met. Calling.");
        this->performSystemCall(aMetaData);
    }
    else
    {
        Plato::Console::Status("PlatoMain: SystemCall -- Condition not met. Not calling.");
    }
}

void SystemCall::executeCommand(const std::vector<std::string> &aArguments)
{
    mCommandPlusArguments = mStringCommand;
    for(const auto &tArgument : aArguments) {
        mCommandPlusArguments += " " + tArgument;
    }

    // make system call
    auto tExitStatus = Plato::system_with_return(mCommandPlusArguments.c_str());
    if (tExitStatus)
    {
        std::string tErrorMessage = std::string("System call ' ") + mCommandPlusArguments + 
                                    std::string(" 'exited with exit status: ") + std::to_string(tExitStatus);
        THROWERR(tErrorMessage)
    }
}

SystemCallMPI::SystemCallMPI(const Plato::InputData & aNode, const Plato::SystemCallMetadata& aMetaData, const MPI_Comm& aComm) : 
    SystemCall(aNode,aMetaData),
    mComm(aComm)
{
    mNumRanks = Plato::Get::Int(aNode, "NumRanks");
}

void SystemCallMPI::executeCommand(const std::vector<std::string> &aArguments)
{
    std::vector<char*> tArgumentPointers;
    mCommandPlusArguments = mStringCommand;
    for(const auto &tArgument : aArguments) {
        mCommandPlusArguments += " " + tArgument;
        const int tLengthWithNullTerminator = tArgument.length() + 1;
        char *tPointer = new char [tLengthWithNullTerminator];
        std::copy_n(tArgument.c_str(), tLengthWithNullTerminator, tPointer);
        tArgumentPointers.push_back(tPointer);
    }
    tArgumentPointers.push_back(nullptr);

    MPI_Comm tIntercom;
    auto tExitStatus = MPI_Comm_spawn(mStringCommand.c_str(),
                                      tArgumentPointers.data(),
                                      mNumRanks, MPI_INFO_NULL,
                                      0,
                                      mComm,
                                      &tIntercom, MPI_ERRCODES_IGNORE);

    if (tExitStatus != MPI_SUCCESS)
    {
        std::string tErrorMessage = std::string("System call ' ") + mStringCommand + std::string(" 'exited with exit status: ") + std::to_string(tExitStatus);
        THROWERR(tErrorMessage)
    }

    for(auto tPointer : tArgumentPointers) {
        delete [] tPointer;
    }
}

}

