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
 * Plato_EngineObjective.tcc
 *
 *  Created on: Dec 21, 2017
 */

#pragma once

#include <vector>
#include <string>
#include <memory>
#include <cassert>

#include "Plato_Interface.hpp"
#include "Plato_Criterion.hpp"
#include "Plato_DataFactory.hpp"
#include "Plato_MultiVector.hpp"
#include "Plato_DistributedVector.hpp"
#include "Plato_OptimizerFactory.hpp"
#include "Plato_OptimizerEngineStageData.hpp"

namespace Plato
{

/******************************************************************************//**
 * @brief Evaluate one or multiple third-party application objective functions
 * @param [in] aControl const reference to 2D container of optimization variables
**********************************************************************************/

template<typename ScalarType, typename OrdinalType>
ScalarType
Plato::EngineObjective<ScalarType, OrdinalType>::
value(const Plato::MultiVector<ScalarType, OrdinalType> & aControl)
{
    assert(mInterface != nullptr);

    std::cerr << __FILE__ << "  " << __FUNCTION__ << "  " << __LINE__ << "  "
              << "optimizerIndex " << "  ";

    for( size_t i=0; i<this->mOptimizerIndex.size(); ++i )
        std::cerr << this->mOptimizerIndex[i] << "  ";

    std::cerr << "mHasInnerLoop " << this->mHasInnerLoop << "  "
              << std::endl;

    // Check to see if there is a nested inner loop which should be
    // executed before the outer loop.
    if( this->mHasInnerLoop )
    {
        MPI_Comm tLocalComm;
        mInterface->getLocalComm(tLocalComm);

        // ARS - Before the inner loop executes what needs to come
        // from the outer loop?

        // ARS - Update the shared data.

        // Read the first optimizer for the inner loop.
        std::vector< size_t > tOptimizerIndex = this->mOptimizerIndex;
        tOptimizerIndex.push_back(0);

        // For the inner loop use the factory to create the optimizer.
        Plato::OptimizerFactory<ScalarType, OrdinalType> tOptimizerFactory;

        Plato::OptimizerInterface<ScalarType, OrdinalType>*
            tOptimizer = tOptimizerFactory.create(mInterface, tLocalComm,
                                                  tOptimizerIndex );

        while( tOptimizer != nullptr )
        {
            tOptimizer->optimize();

            // ARS - Completed the inner loop, what needs to get to
            // the outer loop?

            // ********* Regenerate the data?? ********* //
            std::string tMyStageName("Regenerate");
            std::vector<std::string> tStageNames;
            tStageNames.push_back(tMyStageName);

            // mInterface->compute(tStageNames, *mParameterList);

            // Delete the current optimizer and check for another.
            delete tOptimizer;

            tOptimizer = tOptimizerFactory.create(mInterface, tLocalComm);
        }

        // else
        {
            // mInterface->handleExceptions();
        }
    }

    // Normal evaluation of the objective.

    // ********* Set view to each control vector entry ********* //
    this->setControls(aControl);

    // ********* Set view to objective function value ********* //
    std::string tObjectiveValueName =
      mEngineInputData.getObjectiveValueOutputName();
    ScalarType tObjectiveValue = 0;

    mParameterList->set(tObjectiveValueName, &tObjectiveValue);

    // ********* Compute objective function value ********* //
    std::string tMyStageName = mEngineInputData.getObjectiveValueStageName();
    assert(tMyStageName.empty() == false);

    std::vector<std::string> tStageNames;
    tStageNames.push_back(tMyStageName);

    mInterface->compute(tStageNames, *mParameterList);

    std::cerr << __FILE__ << "  " << __FUNCTION__ << "  " << __LINE__ << "  "
              << "tMyStageName  " << tMyStageName << "  "
              << "tObjectiveValueName  " << tObjectiveValueName << "  "
              << "tObjectiveValue  " << tObjectiveValue << "  "
              << std::endl;

    return (tObjectiveValue);
}

} // namespace Plato
