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
 * Plato_Test_AugmentedLagrangian.cpp
 *
 *  Created on: Oct 21, 2017
 */

#include "gtest/gtest.h"

#include "Plato_UnitTestUtils.hpp"

#include "Plato_Radius.hpp"
#include "Plato_Circle.hpp"
#include "Plato_Rosenbrock.hpp"
#include "Plato_GoldsteinPrice.hpp"
#include "Plato_Himmelblau.hpp"
#include "Plato_ShiftedEllipse.hpp"

#include "Plato_AnalyticalHessian.hpp"
#include "Plato_LinearOperatorList.hpp"
#include "Plato_AnalyticalGradient.hpp"
#include "Plato_GradientOperatorList.hpp"
#include "Plato_LinearCriterionHessian.hpp"
#include "Plato_CentralDifferenceHessian.hpp"
#include "Plato_IdentityPreconditioner.hpp"

namespace PlatoTest
{

TEST(PlatoTest, LinearCriterionHessian)
{
    const size_t tNumVectors = 1;
    const size_t tNumControls = 2;
    Plato::StandardMultiVector<double> tOutput(tNumVectors, tNumControls);
    double tValue = 0.5;
    Plato::StandardMultiVector<double> tVector(tNumVectors, tNumControls, tValue);
    tValue = 2;
    Plato::StandardMultiVector<double> tControl(tNumVectors, tNumControls, tValue);

    // ********* COMPUTE LINEAR CRITERION HESSIAN TIMES VECTOR *********
    Plato::LinearCriterionHessian<double> tHessian;
    tHessian.apply(tControl, tVector, tOutput);
    Plato::StandardMultiVector<double> tGold(tNumVectors, tNumControls);
    PlatoTest::checkMultiVectorData(tOutput, tGold);
}

TEST(PlatoTest, CentralDifferenceHessian)
{
    // ********* ALLOCATE DATA FACTORY *********
    std::shared_ptr<Plato::DataFactory<double>> tDataFactory = std::make_shared<Plato::DataFactory<double>>();
    const size_t tNumControls = 2;
    tDataFactory->allocateControl(tNumControls);

    // ********* ALLOCATE TRUE CRITERION *********
    std::shared_ptr<Plato::Rosenbrock<double>> tCriterion = std::make_shared<Plato::Rosenbrock<double>>();

    // ********* ALLOCATE HESSIAN APPROXIMATION AND DATA STRUCTURES FOR TEST *********
    double tValue = 0.5;
    const size_t tNumVectors = 1;
    Plato::StandardMultiVector<double> tVector(tNumVectors, tNumControls, tValue);
    tValue = 2;
    Plato::StandardMultiVector<double> tOutput(tNumVectors, tNumControls);
    Plato::StandardMultiVector<double> tControl(tNumVectors, tNumControls, tValue);
    Plato::CentralDifferenceHessian<double> tHessian(tDataFactory, tCriterion);

    // ********* COMPUTE HESSIAN APPROXIMATION *********
    tHessian.apply(tControl, tVector, tOutput);

    // ********* COMPUTE TRUE HESSIAN *********
    Plato::StandardMultiVector<double> tTrueHessianTimesVector(tNumVectors, tNumControls);
    tCriterion->hessian(tControl, tVector, tTrueHessianTimesVector);
    PlatoTest::checkMultiVectorData(tOutput, tTrueHessianTimesVector);
}

TEST(PlatoTest, RosenbrockCriterion)
{
    double tValue = 2;
    const size_t tNumVectors = 1;
    const size_t tNumControls = 2;
    Plato::StandardMultiVector<double> tControl(tNumVectors, tNumControls, tValue);

    Plato::Rosenbrock<double> tCriterion;
    // TEST OBJECTIVE FUNCTION EVALUATION
    double tObjectiveValue = tCriterion.value(tControl);
    const double tGoldValue = 401;
    const double tTolerance = 1e-6;
    EXPECT_NEAR(tGoldValue, tObjectiveValue, tTolerance);

    // TEST GRADIENT EVALUATION FUNCTION
    Plato::StandardMultiVector<double> tGradient(tNumVectors, tNumControls);
    tCriterion.gradient(tControl, tGradient);
    Plato::StandardMultiVector<double> tGoldVector(tNumVectors, tNumControls);
    const size_t tVectorIndex = 0;
    tGoldVector(tVectorIndex, 0) = 1602;
    tGoldVector(tVectorIndex, 1) = -400;
    PlatoTest::checkMultiVectorData(tGradient, tGoldVector);

    // TEST HESSIAN TIMES VECTOR FUNCTION
    tValue = 1;
    Plato::StandardMultiVector<double> tVector(tNumVectors, tNumControls, tValue);
    Plato::StandardMultiVector<double> tHessianTimesVector(tNumVectors, tNumControls);
    tCriterion.hessian(tControl, tVector, tHessianTimesVector);
    tGoldVector(tVectorIndex, 0) = 3202;
    tGoldVector(tVectorIndex, 1) = -600;
    PlatoTest::checkMultiVectorData(tHessianTimesVector, tGoldVector);
}

TEST(PlatoTest, CircleCriterion)
{
    const double tValue = 1;
    const size_t tNumVectors = 1;
    const size_t tNumControls = 2;
    Plato::StandardMultiVector<double> tControl(tNumVectors, tNumControls, tValue);

    Plato::Circle<double> tCriterion;

    // TEST OBJECTIVE FUNCTION EVALUATION
    double tObjectiveValue = tCriterion.value(tControl);
    const double tGoldValue = 2;
    const double tTolerance = 1e-6;
    EXPECT_NEAR(tGoldValue, tObjectiveValue, tTolerance);

    // TEST GRADIENT EVALUATION FUNCTION
    Plato::StandardMultiVector<double> tGradient(tNumVectors, tNumControls);
    tCriterion.gradient(tControl, tGradient);
    Plato::StandardMultiVector<double> tGoldVector(tNumVectors, tNumControls);
    const size_t tVectorIndex = 0;
    tGoldVector(tVectorIndex, 1) = -4;
    PlatoTest::checkMultiVectorData(tGradient, tGoldVector);

    // TEST HESSIAN TIMES VECTOR FUNCTION
    Plato::StandardMultiVector<double> tVector(tNumVectors, tNumControls, tValue);
    tVector(tVectorIndex, 1) = -2.;
    Plato::StandardMultiVector<double> tHessianTimesVector(tNumVectors, tNumControls);
    tCriterion.hessian(tControl, tVector, tHessianTimesVector);
    tGoldVector(tVectorIndex, 0) = 2.;
    tGoldVector(tVectorIndex, 1) = -8.;
    PlatoTest::checkMultiVectorData(tHessianTimesVector, tGoldVector);
}

TEST(PlatoTest, RadiusCriterion)
{
    const double tValue = 0.5;
    const size_t tNumVectors = 1;
    const size_t tNumControls = 2;
    Plato::StandardMultiVector<double> tControl(tNumVectors, tNumControls, tValue);

    Plato::Radius<double> tCriterion;

    // TEST OBJECTIVE FUNCTION EVALUATION
    double tObjectiveValue = tCriterion.value(tControl);
    const double tGoldValue = -0.5;
    const double tTolerance = 1e-6;
    EXPECT_NEAR(tGoldValue, tObjectiveValue, tTolerance);

    // TEST GRADIENT EVALUATION FUNCTION
    Plato::StandardMultiVector<double> tGradient(tNumVectors, tNumControls);
    tCriterion.gradient(tControl, tGradient);
    Plato::StandardMultiVector<double> tGoldVector(tNumVectors, tNumControls);
    Plato::fill(1., tGoldVector);
    PlatoTest::checkMultiVectorData(tGradient, tGoldVector);

    // TEST HESSIAN TIMES VECTOR FUNCTION
    Plato::StandardMultiVector<double> tVector(tNumVectors, tNumControls, tValue);
    const size_t tVectorIndex = 0;
    tVector(tVectorIndex, 1) = -2.;
    Plato::StandardMultiVector<double> tHessianTimesVector(tNumVectors, tNumControls);
    tCriterion.hessian(tControl, tVector, tHessianTimesVector);
    tGoldVector(tVectorIndex, 0) = 1.0;
    tGoldVector(tVectorIndex, 1) = -4.0;
    PlatoTest::checkMultiVectorData(tHessianTimesVector, tGoldVector);
}

TEST(PlatoTest, AnalyticalGradient)
{
    const double tValue = 1;
    const size_t tNumVectors = 1;
    const size_t tNumControls = 2;
    const size_t tVectorIndex = 0;
    Plato::StandardMultiVector<double> tOutput(tNumVectors, tNumControls);
    Plato::StandardMultiVector<double> tControl(tNumVectors, tNumControls, tValue);

    std::shared_ptr<Plato::Circle<double>> tCriterion = std::make_shared<Plato::Circle<double>>();
    Plato::AnalyticalGradient<double> tGradient(tCriterion);

    // TEST COMPUTE FUNCTION
    tGradient.compute(tControl, tOutput);

    Plato::StandardMultiVector<double> tGold(tNumVectors, tNumControls);
    tGold(tVectorIndex, 0) = 0.0;
    tGold(tVectorIndex, 1) = -4.0;
    PlatoTest::checkMultiVectorData(tOutput, tGold);
}

TEST(PlatoTest, AnalyticalHessian)
{
    const double tValue = 1;
    const size_t tNumVectors = 1;
    const size_t tNumControls = 2;
    const size_t tVectorIndex = 0;
    Plato::StandardMultiVector<double> tVector(tNumVectors, tNumControls, tValue);
    tVector(tVectorIndex, 1) = -2.;
    Plato::StandardMultiVector<double> tControl(tNumVectors, tNumControls, tValue);
    Plato::StandardMultiVector<double> tHessianTimesVector(tNumVectors, tNumControls);

    std::shared_ptr<Plato::Circle<double>> tCriterion = std::make_shared<Plato::Circle<double>>();
    Plato::AnalyticalHessian<double> tHessian(tCriterion);

    // TEST APPLY VECTOR TO HESSIAN OPERATOR FUNCTION
    tHessian.apply(tControl, tVector, tHessianTimesVector);

    Plato::StandardMultiVector<double> tGoldVector(tNumVectors, tNumControls);
    tGoldVector(tVectorIndex, 0) = 2.;
    tGoldVector(tVectorIndex, 1) = -8.;
    PlatoTest::checkMultiVectorData(tHessianTimesVector, tGoldVector);
}

TEST(PlatoTest, Preconditioner)
{
    Plato::IdentityPreconditioner<double> tPreconditioner;

    const double tValue = 1;
    const size_t tNumVectors = 1;
    const size_t tNumControls = 2;
    const size_t tVectorIndex = 0;
    Plato::StandardMultiVector<double> tOutput(tNumVectors, tNumControls);
    Plato::StandardMultiVector<double> tVector(tNumVectors, tNumControls, tValue);
    tVector(tVectorIndex, 1) = -2.;
    Plato::StandardMultiVector<double> tControl(tNumVectors, tNumControls, tValue);

    // TEST APPLY PRECONDITIONER AND APPLY INVERSE PRECONDITIONER FUNCTIONS
    tPreconditioner.applyInvPreconditioner(tControl, tVector, tOutput);
    PlatoTest::checkMultiVectorData(tOutput, tVector);
    Plato::fill(0., tOutput);
    tPreconditioner.applyPreconditioner(tControl, tVector, tOutput);
    PlatoTest::checkMultiVectorData(tOutput, tVector);
}

TEST(PlatoTest, CriterionList)
{
    Plato::CriterionList<double> tList;
    size_t tGoldInteger = 0;
    EXPECT_EQ(tGoldInteger, tList.size());

    std::shared_ptr<Plato::Circle<double>> tCircle = std::make_shared<Plato::Circle<double>>();
    std::shared_ptr<Plato::Radius<double>> tRadius = std::make_shared<Plato::Radius<double>>();
    tList.add(tCircle);
    tGoldInteger = 1;
    EXPECT_EQ(tGoldInteger, tList.size());
    tList.add(tRadius);
    tGoldInteger = 2;
    EXPECT_EQ(tGoldInteger, tList.size());

    // ** TEST FIRST CRITERION OBJECTIVE **
    const double tValue = 1;
    const size_t tNumVectors = 1;
    const size_t tNumControls = 2;
    Plato::StandardMultiVector<double> tControl(tNumVectors, tNumControls, tValue);

    size_t tCriterionIndex = 0;
    double tOutput = tList[tCriterionIndex].value(tControl);

    double tGoldScalar = 2;
    double tTolerance = 1e-6;
    EXPECT_NEAR(tGoldScalar, tOutput, tTolerance);

    // TEST FIRST CRITERION GRADIENT
    Plato::StandardMultiVector<double> tGradient(tNumVectors, tNumControls);
    tList[tCriterionIndex].gradient(tControl, tGradient);
    Plato::StandardMultiVector<double> tGoldVector(tNumVectors, tNumControls);
    const size_t tVectorIndex = 0;
    tGoldVector(tVectorIndex, 1) = -4;
    PlatoTest::checkMultiVectorData(tGradient, tGoldVector);

    // TEST FIRST CRITERION HESSIAN TIMES VECTOR FUNCTION
    Plato::StandardMultiVector<double> tVector(tNumVectors, tNumControls, tValue);
    tVector(tVectorIndex, 1) = -2.;
    Plato::StandardMultiVector<double> tHessianTimesVector(tNumVectors, tNumControls);
    tList[tCriterionIndex].hessian(tControl, tVector, tHessianTimesVector);
    tGoldVector(tVectorIndex, 0) = 2.;
    tGoldVector(tVectorIndex, 1) = -8.;
    PlatoTest::checkMultiVectorData(tHessianTimesVector, tGoldVector);

    // ** TEST SECOND CRITERION OBJECTIVE **
    tCriterionIndex = 1;
    Plato::fill(0.5, tControl);
    tOutput = tList[tCriterionIndex].value(tControl);
    tGoldScalar = -0.5;
    EXPECT_NEAR(tGoldScalar, tOutput, tTolerance);

    // TEST SECOND CRITERION GRADIENT
    Plato::fill(0., tGradient);
    tList[tCriterionIndex].gradient(tControl, tGradient);
    Plato::fill(1., tGoldVector);
    PlatoTest::checkMultiVectorData(tGradient, tGoldVector);

    // TEST SECOND HESSIAN TIMES VECTOR FUNCTION
    Plato::fill(0.5, tVector);
    tVector(tVectorIndex, 1) = -2.;
    Plato::fill(0., tHessianTimesVector);
    tList[tCriterionIndex].hessian(tControl, tVector, tHessianTimesVector);
    tGoldVector(tVectorIndex, 0) = 1.0;
    tGoldVector(tVectorIndex, 1) = -4.0;
    PlatoTest::checkMultiVectorData(tHessianTimesVector, tGoldVector);

    // **** TEST CREATE FUNCTION ****
    std::shared_ptr<Plato::CriterionList<double>> tCopy = tList.create();
    // FIRST OBJECTIVE
    tCriterionIndex = 0;
    Plato::fill(1.0, tControl);
    tOutput = tCopy->operator [](tCriterionIndex).value(tControl);
    tGoldScalar = 2;
    EXPECT_NEAR(tGoldScalar, tOutput, tTolerance);
    // SECOND OBJECTIVE
    tCriterionIndex = 1;
    Plato::fill(0.5, tControl);
    tOutput = tCopy->operator [](tCriterionIndex).value(tControl);
    tGoldScalar = -0.5;
    EXPECT_NEAR(tGoldScalar, tOutput, tTolerance);
}

TEST(PlatoTest, GradientOperatorList)
{
    std::shared_ptr<Plato::Circle<double>> tCircle = std::make_shared<Plato::Circle<double>>();
    std::shared_ptr<Plato::AnalyticalGradient<double>> tCircleGradient =
            std::make_shared<Plato::AnalyticalGradient<double>>(tCircle);
    std::shared_ptr<Plato::Radius<double>> tRadius = std::make_shared<Plato::Radius<double>>();
    std::shared_ptr<Plato::AnalyticalGradient<double>> tRadiusGradient =
            std::make_shared<Plato::AnalyticalGradient<double>>(tRadius);
    Plato::GradientOperatorList<double> tList;

    // ********* TEST ADD FUNCTION *********
    tList.add(tCircleGradient);
    size_t tIntegerGold = 1;
    EXPECT_EQ(tIntegerGold, tList.size());

    tList.add(tRadiusGradient);
    tIntegerGold = 2;
    EXPECT_EQ(tIntegerGold, tList.size());

    // ********* ALLOCATE DATA STRUCTURES FOR TEST *********
    double tValue = 1;
    const size_t tNumVectors = 1;
    const size_t tNumControls = 2;
    Plato::StandardMultiVector<double> tControl(tNumVectors, tNumControls, tValue);

    // ********* TEST OPERATOR[] - FIRST CRITERION *********
    size_t tVectorIndex = 0;
    Plato::StandardMultiVector<double> tOutput(tNumVectors, tNumControls);
    size_t tGradientOperatorIndex = 0;
    tList[tGradientOperatorIndex].compute(tControl, tOutput);

    Plato::StandardMultiVector<double> tGoldVector(tNumVectors, tNumControls);
    tGoldVector(tVectorIndex, 0) = 0.0;
    tGoldVector(tVectorIndex, 1) = -4.0;
    PlatoTest::checkMultiVectorData(tOutput, tGoldVector);

    // ********* TEST OPERATOR[] - SECOND CRITERION *********
    tValue = 0.5;
    Plato::fill(tValue, tControl);
    Plato::fill(0., tOutput);
    tGradientOperatorIndex = 1;
    tList[tGradientOperatorIndex].compute(tControl, tOutput);
    tGoldVector(tVectorIndex, 0) = 1.0;
    tGoldVector(tVectorIndex, 1) = 1.0;
    PlatoTest::checkMultiVectorData(tOutput, tGoldVector);

    // ********* TEST PTR - FIRST CRITERION *********
    tValue = 1.0;
    Plato::fill(tValue, tControl);
    Plato::fill(0., tOutput);
    tGradientOperatorIndex = 0;
    tList.ptr(tGradientOperatorIndex)->compute(tControl, tOutput);
    tGoldVector(tVectorIndex, 0) = 0.0;
    tGoldVector(tVectorIndex, 1) = -4.0;
    PlatoTest::checkMultiVectorData(tOutput, tGoldVector);

    // ********* TEST PTR - SECOND CRITERION *********
    tValue = 0.5;
    Plato::fill(tValue, tControl);
    Plato::fill(0., tOutput);
    tGradientOperatorIndex = 1;
    tList.ptr(tGradientOperatorIndex)->compute(tControl, tOutput);
    tGoldVector(tVectorIndex, 0) = 1.0;
    tGoldVector(tVectorIndex, 1) = 1.0;
    PlatoTest::checkMultiVectorData(tOutput, tGoldVector);

    // ********* TEST CREATE - FIRST CRITERION *********
    std::shared_ptr<Plato::GradientOperatorList<double>> tListCopy = tList.create();

    tValue = 1.0;
    Plato::fill(tValue, tControl);
    Plato::fill(0., tOutput);
    tGradientOperatorIndex = 0;
    tListCopy->ptr(tGradientOperatorIndex)->compute(tControl, tOutput);
    tGoldVector(tVectorIndex, 0) = 0.0;
    tGoldVector(tVectorIndex, 1) = -4.0;
    PlatoTest::checkMultiVectorData(tOutput, tGoldVector);

    // ********* TEST CREATE - SECOND CRITERION *********
    tValue = 0.5;
    Plato::fill(tValue, tControl);
    Plato::fill(0., tOutput);
    tGradientOperatorIndex = 1;
    tListCopy->ptr(tGradientOperatorIndex)->compute(tControl, tOutput);
    tGoldVector(tVectorIndex, 0) = 1.0;
    tGoldVector(tVectorIndex, 1) = 1.0;
    PlatoTest::checkMultiVectorData(tOutput, tGoldVector);
}

TEST(PlatoTest, LinearOperatorList)
{
    // ********* ALLOCATE RADIUS AND CIRCLE CRITERIA *********
    std::shared_ptr<Plato::Circle<double>> tCircle = std::make_shared<Plato::Circle<double>>();
    std::shared_ptr<Plato::AnalyticalHessian<double>> tCircleHessian =
            std::make_shared<Plato::AnalyticalHessian<double>>(tCircle);

    std::shared_ptr<Plato::Radius<double>> tRadius = std::make_shared<Plato::Radius<double>>();
    std::shared_ptr<Plato::AnalyticalHessian<double>> tRadiusHessian =
            std::make_shared<Plato::AnalyticalHessian<double>>(tRadius);

    // ********* TEST ADD FUNCTION *********
    Plato::LinearOperatorList<double> tList;
    tList.add(tCircleHessian);
    size_t tIntegerGold = 1;
    EXPECT_EQ(tIntegerGold, tList.size());

    tList.add(tRadiusHessian);
    tIntegerGold = 2;
    EXPECT_EQ(tIntegerGold, tList.size());

    // ********* ALLOCATE DATA STRUCTURES FOR TEST *********
    double tValue = 1;
    const size_t tNumVectors = 1;
    const size_t tNumControls = 2;
    Plato::StandardMultiVector<double> tControl(tNumVectors, tNumControls, tValue);

    // ********* TEST OPERATOR[] - FIRST CRITERION *********
    size_t tVectorIndex = 0;
    Plato::StandardMultiVector<double> tVector(tNumVectors, tNumControls, tValue);
    tVector(tVectorIndex, 1) = -2.;
    Plato::StandardMultiVector<double> tOutput(tNumVectors, tNumControls);
    size_t tLinearOperatorIndex = 0;
    tList[tLinearOperatorIndex].apply(tControl, tVector, tOutput);

    Plato::StandardMultiVector<double> tGoldVector(tNumVectors, tNumControls);
    tGoldVector(tVectorIndex, 0) = 2.;
    tGoldVector(tVectorIndex, 1) = -8.;
    PlatoTest::checkMultiVectorData(tOutput, tGoldVector);

    // ********* TEST OPERATOR[] - SECOND CRITERION *********
    Plato::fill(0., tOutput);
    tValue = 0.5;
    Plato::fill(tValue, tVector);
    tVector(tVectorIndex, 1) = -2.;
    Plato::fill(tValue, tControl);
    tLinearOperatorIndex = 1;
    tList[tLinearOperatorIndex].apply(tControl, tVector, tOutput);
    tGoldVector(tVectorIndex, 0) = 1.0;
    tGoldVector(tVectorIndex, 1) = -4.0;
    PlatoTest::checkMultiVectorData(tOutput, tGoldVector);

    // ********* TEST PTR - FIRST CRITERION *********
    Plato::fill(0., tOutput);
    tValue = 1.0;
    Plato::fill(tValue, tVector);
    tVector(tVectorIndex, 1) = -2.;
    Plato::fill(tValue, tControl);
    tLinearOperatorIndex = 0;
    tList.ptr(tLinearOperatorIndex)->apply(tControl, tVector, tOutput);
    tGoldVector(tVectorIndex, 0) = 2.;
    tGoldVector(tVectorIndex, 1) = -8.;
    PlatoTest::checkMultiVectorData(tOutput, tGoldVector);

    // ********* TEST PTR - SECOND CRITERION *********
    Plato::fill(0., tOutput);
    tValue = 0.5;
    Plato::fill(tValue, tVector);
    tVector(tVectorIndex, 1) = -2.;
    Plato::fill(tValue, tControl);
    tLinearOperatorIndex = 1;
    tList.ptr(tLinearOperatorIndex)->apply(tControl, tVector, tOutput);
    tGoldVector(tVectorIndex, 0) = 1.0;
    tGoldVector(tVectorIndex, 1) = -4.0;
    PlatoTest::checkMultiVectorData(tOutput, tGoldVector);

    // ********* TEST CREATE - FIRST CRITERION *********
    std::shared_ptr<Plato::LinearOperatorList<double>> tListCopy = tList.create();

    Plato::fill(0., tOutput);
    tValue = 1.0;
    Plato::fill(tValue, tVector);
    tVector(tVectorIndex, 1) = -2.;
    Plato::fill(tValue, tControl);
    tLinearOperatorIndex = 0;
    tListCopy->ptr(tLinearOperatorIndex)->apply(tControl, tVector, tOutput);
    tGoldVector(tVectorIndex, 0) = 2.;
    tGoldVector(tVectorIndex, 1) = -8.;
    PlatoTest::checkMultiVectorData(tOutput, tGoldVector);

    // ********* TEST CREATE - SECOND CRITERION *********
    Plato::fill(0., tOutput);
    tValue = 0.5;
    Plato::fill(tValue, tVector);
    tVector(tVectorIndex, 1) = -2.;
    Plato::fill(tValue, tControl);
    tLinearOperatorIndex = 1;
    tListCopy->ptr(tLinearOperatorIndex)->apply(tControl, tVector, tOutput);
    tGoldVector(tVectorIndex, 0) = 1.0;
    tGoldVector(tVectorIndex, 1) = -4.0;
    PlatoTest::checkMultiVectorData(tOutput, tGoldVector);
}

} // namespace PlatoTest
