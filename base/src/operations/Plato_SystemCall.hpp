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
 * Plato_SystemCall.hpp
 *
 *  Created on: Oct 23, 2019
 */

#pragma once

#include "Plato_LocalOperation.hpp"
#include <vector>
#include <string>

class PlatoApp;

namespace Plato
{

class InputData;

/******************************************************************************//**
 * \brief Call a shell script
**********************************************************************************/
class SystemCall : public Plato::LocalOp
{
public:
    /******************************************************************************//**
     * \brief Constructor
     * \param [in] aPlatoApp PLATO application
     * \param [in] aNode input metadata
     * \param [in] aUnitTestBool set to true only if used in a unit test environment 
    **********************************************************************************/
    SystemCall(PlatoApp* aPlatoApp, Plato::InputData & aNode,
               bool aUnitTestBool = false);

    /******************************************************************************//**
     * \brief perform local operation to call a shell script.
    **********************************************************************************/
    void operator()();

    /******************************************************************************//**
     * \brief Return local operation's argument list
     * \param [out] aLocalArgs argument list
    **********************************************************************************/
    void getArguments(std::vector<Plato::LocalArg> & aLocalArgs);

    int numRanks() const { return mNumRanks; }
    bool onChange() const { return mOnChange; }
    bool appendInput() const { return mAppendInput; }

    std::string name() const{ return mName;}
    std::string command() const{ return mStringCommand;}

    std::vector<std::string> options() const {return mOptions; }
    std::vector<std::string> arguments() const {return mArguments; }
    std::vector<std::string> inputNames() const {return mInputNames; }
    std::string commandPlusArguments() const {return mCommandPlusArguments; }

    void unitTest(const bool aUnitTestBool) { mUnitTest = aUnitTestBool;}

private:
    /******************************************************************************//**
     * \brief Append input values to the end of the argument list.
     * \param [out] aArguments command argument list
    **********************************************************************************/
    void appendOptionsAndValues(std::vector<std::string>& aArguments);

    /******************************************************************************//**
     * \brief If the parameterd changed since the last call to the shell script, save the new parameters.
     * \return boolean flag indicating if parameters changed since the last call to the shell script. 
    **********************************************************************************/
    bool saveParameters();

    /******************************************************************************//**
     * \brief perform system call to a shell script.
    **********************************************************************************/
    void performSystemCall();

    /******************************************************************************//**
     * \brief Check if the parameters were modified locally; if modified, save the new parameters.
     * \return boolean flag indicating if parameters changed since the last call to the shell script. 
    **********************************************************************************/
    bool checkForLocalParameterChanges();

    /******************************************************************************//**
     * \brief Check if conditions to perform the system call to a shell script are met.
     * \param [in] aDidParametersChanged boolean flag indicating if parameters have changed since last call.
     * \return boolean flag (true=perform system call; false=do not perform system call, condition are not met)
    **********************************************************************************/
    bool shouldEnginePerformSystemCall(bool aDidParametersChanged);

    void setInputSharedDataNames(Plato::InputData& aNode);
    void setArguments(Plato::InputData& aNode);

    virtual void executeCommand(const std::vector<std::string> &aArguments);
    
private:
    bool mOnChange;
    bool mAppendInput;
    bool mUnitTest;
    
    std::string mName;

    std::vector<std::string> mOptions;
    std::vector<std::string> mArguments;
    std::vector<std::string> mInputNames;
    std::vector<std::vector<double>> mSavedParameters;
    
protected:
    int mNumRanks = 1;
    std::string mStringCommand;
    std::string mCommandPlusArguments;
};
// class SystemCall;

class SystemCallMPI : public SystemCall {
public:
SystemCallMPI(PlatoApp* aPlatoApp, Plato::InputData & aNode, bool aUnitTestBool = false);
private:
void executeCommand(const std::vector<std::string> &arguments) override;
};

}
// namespace Plato
