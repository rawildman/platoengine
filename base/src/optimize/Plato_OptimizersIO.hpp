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
 * Plato_OptimizersIO.hpp
 *
 *  Created on: Sep 17, 2018
 */

#pragma once

#include <vector>
#include <cassert>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <exception>
#include <stdexcept>

#include "Plato_Macros.hpp"
#include "Plato_OptimizersIO_Utilities.hpp"

namespace Plato
{
/******************************************************************************//**
 * @brief Diagnostic data for Optimality Criteria (OC) algorithm
**********************************************************************************/
template<typename ScalarType, typename OrdinalType = size_t>
struct OutputDataOC
{
    OrdinalType mNumIter;  /*!< number of outer iterations */
    OrdinalType mObjFuncCount;  /*!< number of objective function evaluations */

    ScalarType mObjFuncValue;  /*!< objective function value */
    ScalarType mNormObjFuncGrad;  /*!< norm of the objective function gradient */
    ScalarType mControlStagnationMeasure;  /*!< norm of the difference between two subsequent control fields */
    ScalarType mObjectiveStagnationMeasure;  /*!< measures stagnation in two subsequent objective function evaluations */
    std::vector<ScalarType> mConstraints;  /*!< residual value for each constraint */
};
// struct OutputDataOC

/******************************************************************************//**
 * @brief Print header for Optimality Criteria (OC) diagnostics file
 * @param [in] aData diagnostic data for optimality criteria algorithm
 * @param [in,out] aOutputFile output file
**********************************************************************************/
template<typename ScalarType, typename OrdinalType>
void print_oc_diagnostics_header(const Plato::OutputDataOC<ScalarType, OrdinalType>& aData,
                                 std::ofstream& aOutputFile)
{
    try
    {
        Plato::error::is_file_open(aOutputFile);
        Plato::error::is_vector_empty(aData.mConstraints);
    }
    catch(const std::invalid_argument& tErrorMsg)
    {
        THROWERR(tErrorMsg.what())
    }

    aOutputFile << std::scientific << std::setprecision(6) << std::right << "Iter" << std::setw(10) << "F-count"
            << std::setw(14) << "F(X)" << std::setw(16) << "Norm(F')" << std::setw(10);

    const OrdinalType tNumConstraints = aData.mConstraints.size();
    for(OrdinalType tIndex = 0; tIndex < tNumConstraints; tIndex++)
    {
        if(tIndex != static_cast<OrdinalType>(0))
        {
            aOutputFile << "H" << tIndex + static_cast<OrdinalType>(1) << "(X)" << std::setw(16);
        }
        else
        {
            const OrdinalType tWidth = tNumConstraints > static_cast<OrdinalType>(1) ? 11 : 16;
            aOutputFile << "H" << tIndex + static_cast<OrdinalType>(1) << "(X)" << std::setw(tWidth);
        }
    }

    aOutputFile << "abs(dX)" << std::setw(15) << "abs(dF)" << "\n" << std::flush;
}

/******************************************************************************//**
 * @brief Print diagnostics for Optimality Criteria (OC) algorithm
 * @param [in] aData diagnostic data for optimality criteria algorithm
 * @param [in,out] aOutputFile output file
**********************************************************************************/
template<typename ScalarType, typename OrdinalType>
void print_oc_diagnostics(const Plato::OutputDataOC<ScalarType, OrdinalType>& aData,
                          std::ofstream& aOutputFile)
{
    try
    {
        Plato::error::is_file_open(aOutputFile);
        Plato::error::is_vector_empty(aData.mConstraints);
    }
    catch(const std::invalid_argument& tErrorMsg)
    {
        THROWERR(tErrorMsg.what())
    }
    //assert(aData.mConstraints.size() > static_cast<OrdinalType>(0));

    aOutputFile << std::scientific << std::setprecision(6) << std::right << aData.mNumIter << std::setw(10)
            << aData.mObjFuncCount << std::setw(20) << aData.mObjFuncValue << std::setw(15) << aData.mNormObjFuncGrad
            << std::setw(15);

    const OrdinalType tNumConstraints = aData.mConstraints.size();
    for(OrdinalType tIndex = 0; tIndex < tNumConstraints; tIndex++)
    {
        aOutputFile << aData.mConstraints[tIndex] << std::setw(15);
    }

    aOutputFile << aData.mControlStagnationMeasure << std::setw(15) << aData.mObjectiveStagnationMeasure << "\n"
            << std::flush;
}

} // namespace Plato
