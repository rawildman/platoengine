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
 * Plato_Test_KelleySachsBounds.cpp
 *
 *  Created on: Dec 19, 2017
 */

#include "gtest/gtest.h"

#include "Plato_UnitTestUtils.hpp"

#include "Plato_KelleySachsBoundLightInterface.hpp"

#include "Plato_Circle.hpp"
#include "Plato_Rosenbrock.hpp"
#include "Plato_Himmelblau.hpp"
#include "Plato_ProxyVolume.hpp"
#include "Plato_GoldsteinPrice.hpp"
#include "Plato_ProxyCompliance.hpp"
#include "Plato_EpetraSerialDenseVector.hpp"
#include "Plato_EpetraSerialDenseMultiVector.hpp"
#include "Plato_StructuralTopologyOptimization.hpp"
#include "Plato_StructuralTopologyOptimizationProxyGoldResults.hpp"

namespace PlatoTest
{

TEST(PlatoTest, ReducedSpaceTrustRegionStageMngTestOne)
{
    // ************** ALLOCATE SIMPLE STRUCTURAL TOPOLOGY OPTIMIZATION SOLVER **************
    const double tPoissonRatio = 0.3;
    const double tElasticModulus = 1;
    const int tNumElementsXdirection = 30;
    const int tNumElementsYdirection = 10;
    std::shared_ptr<Plato::StructuralTopologyOptimization> tPDE = std::make_shared < Plato::StructuralTopologyOptimization
            > (tPoissonRatio, tElasticModulus, tNumElementsXdirection, tNumElementsYdirection);

    // ************** SET FORCE VECTOR **************
    const int tGlobalNumDofs = tPDE->getGlobalNumDofs();
    Epetra_SerialDenseVector tForce(tGlobalNumDofs);
    const int tDOFsIndex = 1;
    tForce[tDOFsIndex] = -1;
    tPDE->setForceVector(tForce);

    // ************** SET FIXED DEGREES OF FREEDOM (DOFs) VECTOR **************
    std::vector<double> tDofs = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 681};
    Epetra_SerialDenseVector tFixedDOFs(Epetra_DataAccess::Copy, tDofs.data(), tDofs.size());
    tPDE->setFixedDOFs(tFixedDOFs);

    // ************** ALLOCATE COMPLIANCE CRITERIA **************
    std::shared_ptr<Plato::ProxyCompliance<double>> tCompliance = std::make_shared<Plato::ProxyCompliance<double>>(tPDE);
    std::shared_ptr<Plato::CriterionList<double>> tObjectives = std::make_shared<Plato::CriterionList<double>>();
    tObjectives->add(tCompliance);

    // ********* ALLOCATE CORE DATA STRUCTURES *********
    double tBaseValue = 1;
    const size_t tNumVectors = 1;
    std::shared_ptr<Plato::DataFactory<double>> tDataFactory = std::make_shared<Plato::DataFactory<double>>();
    const size_t tNumControls = tPDE->getNumDesignVariables();
    Plato::EpetraSerialDenseMultiVector<double> tControl(tNumVectors, tNumControls, tBaseValue);
    tDataFactory->allocateControl(tControl);

    // ********* REDUCED SPACE TRUST REGION STAGE MANAGER *********
    Plato::ReducedSpaceTrustRegionStageMng<double> tStageMng(tDataFactory, tObjectives);

    // ********* TEST OBJECTIVE FUNCTION EVALUATION *********
    size_t tOrdinalGold = 0;
    EXPECT_EQ(tOrdinalGold, tStageMng.getNumObjectiveFunctionEvaluations());
    double tScalarValue = tStageMng.evaluateObjective(tControl);
    tOrdinalGold = 1;
    EXPECT_EQ(tOrdinalGold, tStageMng.getNumObjectiveFunctionEvaluations());

    double tScalarGold = 1;
    double tTolerance = 1e-4;
    EXPECT_NEAR(tScalarGold, tScalarValue, tTolerance);

    // ********* TEST GRADIENT EVALUATION *********
    tOrdinalGold = 0;
    EXPECT_EQ(tOrdinalGold, tStageMng.getNumObjectiveGradientEvaluations());
    Plato::EpetraSerialDenseMultiVector<double> tGradient(tNumVectors, tNumControls);
    tStageMng.cacheData();
    tStageMng.computeGradient(tControl, tGradient);
    tOrdinalGold = 1;
    EXPECT_EQ(tOrdinalGold, tStageMng.getNumObjectiveGradientEvaluations());

    const size_t tVectorIndex = 0;
    std::vector<double> tGoldGradient = TopoProxy::getGoldNormalizedFilteredGradient();
    assert(tGoldGradient.size() == tControl[tVectorIndex].size());
    for(size_t tIndex = 0; tIndex < tGoldGradient.size(); tIndex++)
    {
        EXPECT_NEAR(tGradient(tVectorIndex, tIndex), tGoldGradient[tIndex], tTolerance);
    }

    // ********* TEST APPLY VECTOR TO GAUSS NEWTON HESSIAN *********
    tBaseValue = 1;
    tOrdinalGold = 0;
    EXPECT_EQ(tOrdinalGold, tStageMng.getNumObjectiveHessianEvaluations());
    Plato::EpetraSerialDenseMultiVector<double> tVector(tNumVectors, tNumControls, tBaseValue);
    Plato::EpetraSerialDenseMultiVector<double> tHessianTimesVector(tNumVectors, tNumControls);
    tStageMng.applyVectorToHessian(tControl, tVector, tHessianTimesVector);
    tOrdinalGold = 1;
    EXPECT_EQ(tOrdinalGold, tStageMng.getNumObjectiveHessianEvaluations());

    std::vector<double> tGoldHessianTimesVector = TopoProxy::getGoldNormalizedHessianTimesVector();
    assert(tGoldHessianTimesVector.size() == tHessianTimesVector[tVectorIndex].size());
    for(size_t tIndex = 0; tIndex < tGoldHessianTimesVector.size(); tIndex++)
    {
        EXPECT_NEAR(tHessianTimesVector(tVectorIndex, tIndex), tGoldHessianTimesVector[tIndex], tTolerance);
    }

    // ********* TEST APPLY VECTOR TO PRECONDITIONER *********
    tOrdinalGold = 0;
    EXPECT_EQ(tOrdinalGold, tStageMng.getNumPreconditionerEvaluations());
    Plato::EpetraSerialDenseMultiVector<double> tPreconditionerTimesVector(tNumVectors, tNumControls);
    tStageMng.applyVectorToPreconditioner(tControl, tVector, tPreconditionerTimesVector);
    tOrdinalGold = 1;
    EXPECT_EQ(tOrdinalGold, tStageMng.getNumPreconditionerEvaluations());

    tBaseValue = 1;
    std::vector<double> tGoldPreconditionerTimesVector(tNumControls, tBaseValue);
    assert(tGoldPreconditionerTimesVector.size() == tPreconditionerTimesVector[tVectorIndex].size());
    for(size_t tIndex = 0; tIndex < tGoldPreconditionerTimesVector.size(); tIndex++)
    {
        EXPECT_NEAR(tPreconditionerTimesVector(tVectorIndex, tIndex), tGoldPreconditionerTimesVector[tIndex], tTolerance);
    }

    // ********* TEST APPLY VECTOR TO INVERSE PRECONDITIONER *********
    tOrdinalGold = 0;
    EXPECT_EQ(tOrdinalGold, tStageMng.getNumInversePreconditionerEvaluations());
    Plato::EpetraSerialDenseMultiVector<double> tInvPreconditionerTimesVector(tNumVectors, tNumControls);
    tStageMng.applyVectorToInvPreconditioner(tControl, tVector, tInvPreconditionerTimesVector);
    tOrdinalGold = 1;
    EXPECT_EQ(tOrdinalGold, tStageMng.getNumInversePreconditionerEvaluations());

    tBaseValue = 1;
    std::vector<double> tGoldInvPreconditionerTimesVector(tNumControls, tBaseValue);
    assert(tGoldPreconditionerTimesVector.size() == tInvPreconditionerTimesVector[tVectorIndex].size());
    for(size_t tIndex = 0; tIndex < tGoldInvPreconditionerTimesVector.size(); tIndex++)
    {
        EXPECT_NEAR(tInvPreconditionerTimesVector(tVectorIndex, tIndex), tGoldInvPreconditionerTimesVector[tIndex], tTolerance);
    }
}

TEST(PlatoTest, ReducedSpaceTrustRegionStageMngTestTwo)
{
    // ************** ALLOCATE SIMPLE STRUCTURAL TOPOLOGY OPTIMIZATION SOLVER **************
    const double tPoissonRatio = 0.3;
    const double tElasticModulus = 1;
    const int tNumElementsXdirection = 30;
    const int tNumElementsYdirection = 10;
    std::shared_ptr<Plato::StructuralTopologyOptimization> tPDE =
            std::make_shared<Plato::StructuralTopologyOptimization>(tPoissonRatio, tElasticModulus, tNumElementsXdirection, tNumElementsYdirection);

    // ************** SET FORCE VECTOR **************
    const int tGlobalNumDofs = tPDE->getGlobalNumDofs();
    Epetra_SerialDenseVector tForce(tGlobalNumDofs);
    const int tDOFsIndex = 1;
    tForce[tDOFsIndex] = -1;
    tPDE->setForceVector(tForce);

    // ************** SET FIXED DEGREES OF FREEDOM (DOFs) VECTOR **************
    std::vector<double> tDofs = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 681};
    Epetra_SerialDenseVector tFixedDOFs(Epetra_DataAccess::Copy, tDofs.data(), tDofs.size());
    tPDE->setFixedDOFs(tFixedDOFs);

    // ************** ALLOCATE COMPLIANCE AND VOLUME CRITERIA **************
    std::shared_ptr<Plato::ProxyVolume<double>> tVolume = std::make_shared<Plato::ProxyVolume<double>>(tPDE);
    std::shared_ptr<Plato::ProxyCompliance<double>> tCompliance = std::make_shared<Plato::ProxyCompliance<double>>(tPDE);
    std::shared_ptr<Plato::CriterionList<double>> tObjectives = std::make_shared<Plato::CriterionList<double>>();
    tObjectives->add(tCompliance);
    tObjectives->add(tVolume);
    size_t tOrdinalGold = 2;
    EXPECT_EQ(tOrdinalGold, tObjectives->size());

    // ********* ALLOCATE CORE DATA STRUCTURES *********
    double tBaseValue = 1;
    const size_t tNumVectors = 1;
    std::shared_ptr<Plato::DataFactory<double>> tDataFactory = std::make_shared<Plato::DataFactory<double>>();
    const size_t tNumControls = tPDE->getNumDesignVariables();
    Plato::EpetraSerialDenseMultiVector<double> tControl(tNumVectors, tNumControls, tBaseValue);
    tDataFactory->allocateControl(tControl);

    // ********* REDUCED SPACE TRUST REGION STAGE MANAGER *********
    Plato::ReducedSpaceTrustRegionStageMng<double> tStageMng(tDataFactory, tObjectives);

    // ********* TEST OBJECTIVE FUNCTION EVALUATION *********
    tOrdinalGold = 0;
    EXPECT_EQ(tOrdinalGold, tStageMng.getNumObjectiveFunctionEvaluations());
    double tScalarValue = tStageMng.evaluateObjective(tControl);
    tOrdinalGold = 1;
    EXPECT_EQ(tOrdinalGold, tStageMng.getNumObjectiveFunctionEvaluations());

    double tScalarGold = 1.6;
    double tTolerance = 1e-4;
    EXPECT_NEAR(tScalarGold, tScalarValue, tTolerance);

    // ********* TEST GRADIENT EVALUATION *********
    tOrdinalGold = 0;
    EXPECT_EQ(tOrdinalGold, tStageMng.getNumObjectiveGradientEvaluations());
    Plato::EpetraSerialDenseMultiVector<double> tGradient(tNumVectors, tNumControls);
    tStageMng.cacheData();
    tStageMng.computeGradient(tControl, tGradient);
    tOrdinalGold = 1;
    EXPECT_EQ(tOrdinalGold, tStageMng.getNumObjectiveGradientEvaluations());

    const size_t tVectorIndex = 0;
    std::vector<double> tGoldGradient = TopoProxy::getGoldNormalizedCompoundFilteredGradient();
    assert(tGoldGradient.size() == tControl[tVectorIndex].size());
    for(size_t tIndex = 0; tIndex < tGoldGradient.size(); tIndex++)
    {
        EXPECT_NEAR(tGradient(tVectorIndex, tIndex), tGoldGradient[tIndex], tTolerance);
    }

    // ********* TEST APPLY VECTOR TO GAUSS NEWTON HESSIAN *********
    tBaseValue = 1;
    tOrdinalGold = 0;
    EXPECT_EQ(tOrdinalGold, tStageMng.getNumObjectiveHessianEvaluations());
    Plato::EpetraSerialDenseMultiVector<double> tVector(tNumVectors, tNumControls, tBaseValue);
    Plato::EpetraSerialDenseMultiVector<double> tHessianTimesVector(tNumVectors, tNumControls);
    tStageMng.applyVectorToHessian(tControl, tVector, tHessianTimesVector);
    tOrdinalGold = 1;
    EXPECT_EQ(tOrdinalGold, tStageMng.getNumObjectiveHessianEvaluations());

    std::vector<double> tGoldHessianTimesVector = TopoProxy::getGoldNormalizedCompoundHessianTimesVector();
    assert(tGoldHessianTimesVector.size() == tHessianTimesVector[tVectorIndex].size());
    for(size_t tIndex = 0; tIndex < tGoldHessianTimesVector.size(); tIndex++)
    {
        EXPECT_NEAR(tHessianTimesVector(tVectorIndex, tIndex), tGoldHessianTimesVector[tIndex], tTolerance);
    }
}

}
