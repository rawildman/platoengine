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

#pragma once

#include "Plato_OptimizerInterface.hpp"

#include "ROL_Bounds.hpp"
#include "ROL_Solver.hpp"

#include "Plato_ReducedObjectiveROL.hpp"
#include "Plato_ReducedConstraintROL.hpp"
#include "Plato_DistributedVectorROL.hpp"

#include <mpi.h>

namespace Plato
{

template<typename ScalarType, typename OrdinalType = size_t>
class ROLInterface : public OptimizerInterface<ScalarType,OrdinalType>
{
public:
    ROLInterface(Plato::Interface* aInterface, const MPI_Comm & aComm):
        OptimizerInterface<ScalarType,OrdinalType>(aInterface,aComm){ }

    virtual ~ROLInterface() = default;

    void initialize() final override 
    {
        OptimizerInterface<ScalarType,OrdinalType>::initialize();
        mOutputBuffer = getOutputBuffer();
    }

    
private:
    std::ofstream mOutputFile;
    std::streambuf *getOutputBuffer() 
    {
        int tMyRank = -1;
        MPI_Comm_rank(this->mComm, &tMyRank);
        assert(tMyRank >= static_cast<int>(0));
        if(tMyRank == static_cast<int>(0))
        {
            if (this->mInputData.getOutputDiagnosticsToFile()) {
                mOutputFile.open("ROL_output.txt", std::ofstream::out);
                return mOutputFile.rdbuf();
            }
        }
        return std::cout.rdbuf();
    }

protected:

    std::streambuf *mOutputBuffer;

    /******************************************************************************/
    void printControl(const ROL::Ptr<ROL::Problem<ScalarType>> & aOptimizationProblem)
    /******************************************************************************/
    {
        int tMyRank = -1;
        MPI_Comm_rank(this->mComm, &tMyRank);
        assert(tMyRank >= static_cast<int>(0));
        if(tMyRank == static_cast<int>(0))
        {
            const bool tOutputControlToFile = this->mInputData.getOutputControlToFile();
            if(tOutputControlToFile == true)
            {
                std::ofstream tOutputFile;
                tOutputFile.open("ROL_control_output.txt");
                ROL::Ptr<ROL::Vector<ScalarType>> tSolutionPtr = aOptimizationProblem->getPrimalOptimizationVector();
                Plato::DistributedVectorROL<ScalarType> & tSolution =
                        dynamic_cast<Plato::DistributedVectorROL<ScalarType>&>(tSolutionPtr.operator*());
                std::vector<ScalarType> & tData = tSolution.vector();
                for(OrdinalType tIndex = 0; tIndex < tData.size(); tIndex++)
                {
                    tOutputFile << tData[tIndex] << "\n";
                }
                tOutputFile.close();
            }
        }
    }

    /******************************************************************************/
    void setBounds(const std::vector<ScalarType> & aInputs, Plato::DistributedVectorROL<ScalarType> & aBounds)
    /******************************************************************************/
    {
        assert(aInputs.empty() == false);
        if(aInputs.size() == static_cast<size_t>(1))
        {
            const ScalarType tValue = aInputs[0];
            aBounds.fill(tValue);
        }
        else
        {
            assert(aInputs.size() == static_cast<size_t>(aBounds.dimension()));
            aBounds.setVector(aInputs);
        }
    }

    /******************************************************************************/
    void setInitialGuess(const std::string & aMyName, Plato::DistributedVectorROL<ScalarType> & aControl)
    /******************************************************************************/
    {
        std::string tInitializationStageName = this->mInputData.getInitializationStageName();
        if(tInitializationStageName.empty() == false)
        {
            // Use user-defined stage to compute initial guess
            Teuchos::ParameterList tPlatoInitializationStageParameterList;
            tPlatoInitializationStageParameterList.set(aMyName, aControl.vector().data());
            this->mInterface->compute(tInitializationStageName, tPlatoInitializationStageParameterList);
        }
        else
        {
            // Use user-defined values to compute initial guess. Hence, a stage was not defined by the user.
            std::vector<ScalarType> tInitialGuess = this->mInputData.getInitialGuess();
            assert(tInitialGuess.empty() == false);
            if(tInitialGuess.size() == static_cast<size_t>(1))
            {
                const ScalarType tValue = tInitialGuess[0];
                aControl.fill(tValue);
            }
            else
            {
                assert(tInitialGuess.size() == static_cast<size_t>(aControl.dimension()));
                aControl.setVector(tInitialGuess);
            }
        }
    }

    Teuchos::RCP<Teuchos::ParameterList> updateParameterListFromRolInputsFile()
    {
        std::string tFileName = this->mInputData.getInputFileName();
        Teuchos::RCP<Teuchos::ParameterList> tParameterList = Teuchos::rcp(new Teuchos::ParameterList);
        Teuchos::updateParametersFromXmlFile(tFileName, tParameterList.ptr());
        return tParameterList;
    }
    
    void checkGradient(const ROL::Ptr<ROL::Problem<ScalarType>> & aOptimizationProblem)
    {
        auto tPerturbationScale = this->mInputData.getROLPerturbationScale();
        auto tCheckGradientSteps = this->mInputData.getROLCheckGradientSteps();
        std::ofstream tOutputFile;
        tOutputFile.open("ROL_gradient_check_output.txt");
        auto tObjective = aOptimizationProblem->getObjective();
        auto tControl = aOptimizationProblem->getPrimalOptimizationVector();
        auto tPerturbation = tControl->clone();
        tPerturbation->randomize(-tPerturbationScale, tPerturbationScale);      
		tObjective->checkGradient(*tControl, *tPerturbation,true,tOutputFile,tCheckGradientSteps);
        tOutputFile.close();
    }
};

} // namespace Plato
