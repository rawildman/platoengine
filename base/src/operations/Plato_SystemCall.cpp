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
#include <mpi.h>

#include "PlatoApp.hpp"
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
SystemCall::SystemCall(PlatoApp* aPlatoApp, Plato::InputData & aNode,
                       bool aUnitTestBool) :
    Plato::LocalOp(aPlatoApp)
    mUnitTest(aUnitTestBool)
/******************************************************************************/
{
    // set basic info
    mName = Plato::Get::String(aNode, "Name");
    mOnChange = Plato::Get::Bool(aNode, "OnChange");
    mAppendInput = Plato::Get::Bool(aNode, "AppendInput");
    mStringCommand = Plato::Get::String(aNode, "Command");
    
    this->setInputSharedDataNames(aNode);
    this->setArguments(aNode);


    // set options
    // count total parameters
    size_t tTotalNumParameters = 0;
    for(auto tInputName : mInputNames)
    {
        auto tInputArgument = mPlatoApp->getValue(tInputName);
        tTotalNumParameters += tInputArgument.size();
    }

    // set mOptions size to total num parameters
    mOptions = std::vector<std::string>(tTotalNumParameters, "");

    for(auto tStrValue : aNode.getByName<std::string>("Option"))
    {
        auto tIndex = &tStrValue - &aNode.getByName<std::string>("Option")[0];
        mOptions[tIndex] = tStrValue;
    }
}

SystemCall::setInputSharedDataNames(Plato::InputData& aNode)
{
    for(Plato::InputData tInputNode : aNode.getByName<Plato::InputData>("Input"))
    {
        mInputNames.push_back(Plato::Get::String(tInputNode, "ArgumentName"));
    }
}

SystemCall::setArguments(Plato::InputData& aNode)
{
    // set arguments
    for(auto tStrValue : aNode.getByName<std::string>("Argument"))
    {
        mArguments.push_back(tStrValue);
    }
}

SystemCallMPI::SystemCallMPI(PlatoApp* aPlatoApp, Plato::InputData & aNode,
                       bool aUnitTestBool) : SystemCall(aPlatoApp, aNode, aUnitTestBool)
{
    mNumRanks = Plato::Get::Int(aNode, "NumRanks");
}

bool SystemCall::saveParameters()
{
    bool tChanged = false;
    if(mSavedParameters.size() == 0)
    {
        for(size_t tIndexJ=0; tIndexJ<mInputNames.size(); ++tIndexJ)
        {
            Plato::Console::Status("PlatoMain: On Change SystemCall -- \"" + mInputNames[tIndexJ] + "\" Values:");
            auto tInputArgument = mPlatoApp->getValue(mInputNames[tIndexJ]);
            std::vector<double> tCurVector(tInputArgument->size());
            for(size_t tIndexI=0; tIndexI<tInputArgument->size(); ++tIndexI)
            {
                tCurVector[tIndexI] = tInputArgument->data()[tIndexI];
                Plato::Console::Status("Saved: Not set yet; Current: " + std::to_string(tCurVector[tIndexI]));
            }
            mSavedParameters.push_back(tCurVector);
        }
        tChanged = true;
    }
    return tChanged;
}

bool SystemCall::checkForLocalParameterChanges()
{
    bool tDidParametersChanged = false;
    for(size_t tIndexJ=0; tIndexJ<mInputNames.size(); ++tIndexJ)
    {
        bool tLocalChanged = false;
        Plato::Console::Status("PlatoMain: On Change SystemCall -- \"" + mInputNames[tIndexJ] + "\" Values:");
        auto tInputArgument = mPlatoApp->getValue(mInputNames[tIndexJ]);
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

bool SystemCall::shouldEnginePerformSystemCall(bool aDidParametersChanged)
{
    bool tPerformSystemCall = true;
    if(!aDidParametersChanged)
    {
        tPerformSystemCall = false;
        Plato::Console::Status("PlatoMain: SystemCall -- Condition not met. Not calling.");
    } 
    else
    {
        Plato::Console::Status("PlatoMain: SystemCall -- Condition met. Calling.");
    }
    return tPerformSystemCall;
}

void SystemCall::performSystemCall()
{
    std::vector<std::string> tArguments;
    for(auto& tArgumentName : mArguments)
    {
        tArguments.push_back(tArgumentName);
    }
    if(mAppendInput)
    {
        this->appendOptionsAndValues(tArguments);
    }
    this->executeCommand(tArguments);
}

void SystemCall::appendOptionsAndValues(std::vector<std::string>& aArguments)
{
    for(auto& tInputName : mInputNames)
    {
        auto tIndex = &tInputName - &mInputNames[0];
        auto tInputArgument = mPlatoApp->getValue(tInputName);
        for(size_t tIndexI=0; tIndexI<tInputArgument->size(); ++tIndexI)
        {
            std::stringstream tDataString;
            tDataString << std::setprecision(16) << mOptions[tIndex] << tInputArgument->data()[tIndexI];
            aArguments.push_back(tDataString.str());
        }
    }
}

/******************************************************************************/
void SystemCall::operator()()
/******************************************************************************/
{
    bool tPerformSystemCall = true;
    if(mOnChange)
    {
        // If we haven't saved any parameters yet save them now and set "changed" to true.
        auto tChanged = this->saveParameters();
        if(!tChanged)
        {
            tChanged = this->checkForLocalParameterChanges();
        }
        tPerformSystemCall = this->shouldEnginePerformSystemCall(tChanged);
    }

    if(tPerformSystemCall)
    {
        this->performSystemCall();
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
    auto tExitStatus = MPI_Comm_spawn(mStringCommand.c_str(), tArgumentPointers.data(), mNumRanks, MPI_INFO_NULL, 0, mPlatoApp->getComm(), &tIntercom, MPI_ERRCODES_IGNORE);
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

