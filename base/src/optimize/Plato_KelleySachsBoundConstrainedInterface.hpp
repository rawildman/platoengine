/*
 * Plato_KelleySachsBoundConstrainedInterface.hpp
 *
 *  Created on: Dec 21, 2017
 */

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

#ifndef PLATO_KELLEYSACHSBOUNDCONSTRAINEDINTERFACE_HPP_
#define PLATO_KELLEYSACHSBOUNDCONSTRAINEDINTERFACE_HPP_

#include <string>
#include <memory>
#include <sstream>

#include "Plato_Parser.hpp"
#include "Plato_Interface.hpp"
#include "Plato_DataFactory.hpp"
#include "Plato_CriterionList.hpp"
#include "Plato_AlgebraFactory.hpp"
#include "Plato_EngineObjective.hpp"
#include "Plato_OptimizerInterface.hpp"
#include "Plato_StandardMultiVector.hpp"
#include "Plato_OptimizerEngineStageData.hpp"
#include "Plato_KelleySachsBoundConstrained.hpp"
#include "Plato_TrustRegionAlgorithmDataMng.hpp"
#include "Plato_ReducedSpaceTrustRegionStageMng.hpp"

namespace Plato
{

template<typename ScalarType, typename OrdinalType = size_t>
class KelleySachsBoundConstrainedInterface : public Plato::OptimizerInterface<ScalarType, OrdinalType>
{
public:
    /******************************************************************************/
    explicit KelleySachsBoundConstrainedInterface(Plato::Interface* aInterface,
                                                  const MPI_Comm & aComm,
                                                  const Plato::optimizer::algorithm_t & aType) :
        Plato::OptimizerInterface<ScalarType, OrdinalType>::OptimizerInterface(aInterface, aComm),
        mType(aType) { }

    virtual ~KelleySachsBoundConstrainedInterface() = default;

    /******************************************************************************/
    Plato::optimizer::algorithm_t algorithm() const
    /******************************************************************************/
    {
        return (mType);
    }

    /******************************************************************************/
    void initialize()
    /******************************************************************************/
    {
        auto tOptimizerNode = this->getOptimizerNode(this->mInterface);

        Plato::Parse::parseOptimizerStages(tOptimizerNode, this->mInputData);
    }

    /******************************************************************************/
    void run()
    /******************************************************************************/
    {
        this->initialize();

        // ********* ALLOCATE LINEAR ALGEBRA FACTORY ********* //
        Plato::AlgebraFactory<ScalarType, OrdinalType> tLinearAlgebraFactory;

        // ********* ALLOCATE BASELINE DATA STRUCTURES FOR OPTIMIZER ********* //
        std::shared_ptr<Plato::DataFactory<ScalarType, OrdinalType>> tDataFactory =
                std::make_shared<Plato::DataFactory<ScalarType, OrdinalType>>();
        this->allocateBaselineDataStructures(tLinearAlgebraFactory, *tDataFactory);

        // ********* ALLOCATE DATA MANAGER ********* //
        std::shared_ptr<Plato::TrustRegionAlgorithmDataMng<ScalarType, OrdinalType>> tDataMng =
                std::make_shared<Plato::TrustRegionAlgorithmDataMng<ScalarType, OrdinalType>>(tDataFactory);

        // ********* SET INITIAL GUESS ********* //
        this->setInitialGuess(tLinearAlgebraFactory, *tDataMng);

        // ********* SET UPPER AND LOWER BOUNDS ********* //
        this->setLowerBounds(tLinearAlgebraFactory, *tDataFactory, *tDataMng);
        this->setUpperBounds(tLinearAlgebraFactory, *tDataFactory, *tDataMng);

        // ********* ALLOCATE OBJECTIVE FUNCTION ********* //
        std::shared_ptr<Plato::EngineObjective<ScalarType, OrdinalType>> tObjective =
          std::make_shared<Plato::EngineObjective<ScalarType, OrdinalType>>(*tDataFactory, this->mInputData, this->mInterface, this);
        std::shared_ptr<Plato::CriterionList<ScalarType, OrdinalType>> tCriterionList =
                std::make_shared<Plato::CriterionList<ScalarType, OrdinalType>>();
        tCriterionList->add(tObjective);

        // ********* ALLOCATE REDUCED SPACE FORMULATION MANAGER FOR TRUST REGION ALGORITHM ********* //
        std::shared_ptr<Plato::ReducedSpaceTrustRegionStageMng<ScalarType, OrdinalType>> tStageMng =
                std::make_shared<Plato::ReducedSpaceTrustRegionStageMng<ScalarType, OrdinalType>>(tDataFactory, tCriterionList);
        this->setHessianMethod(*tStageMng);

        // ********* ALLOCATE KELLEY-SACHS BOUND CONSTRAINED OPTIMIZATION ALGORITHM ********* //
        Plato::KelleySachsBoundConstrained<ScalarType, OrdinalType> tAlgorithm(tDataFactory, tDataMng, tStageMng);
        this->setParameters(tAlgorithm);
        tAlgorithm.solve();
    }

    /******************************************************************************//**
     * @brief All optimizing is done so do any optional final
     * stages. Called only once from the interface.
    **********************************************************************************/
    void finalize()
    /******************************************************************************/
    {
        this->mInterface->finalize(this->mInputData.getFinalizationStageName());
    }

private:
    /******************************************************************************//**
     * @brief Set numerical method use to compute application of vector to Hessian operator
     * @param [in,out] aStageMng interface to criteria value, gradient and Hessian evaluations
     **********************************************************************************/
    void setHessianMethod(Plato::ReducedSpaceTrustRegionStageMng<ScalarType, OrdinalType> & aStageMng)
    {
        if(this->mInputData.getHessianType() == "lbfgs")
        {
            aStageMng.setHaveHessian(true);
            aStageMng.setHessianLBFGS(this->mInputData.getLimitedMemoryStorage());
        }
        else if(this->mInputData.getHessianType() == "analytical")
        {
            aStageMng.setHaveHessian(true);
        }
        else
        {
            aStageMng.setHaveHessian(false);
        }
    }

    /******************************************************************************/
    void setParameters(Plato::KelleySachsBoundConstrained<ScalarType, OrdinalType> & aAlgorithm)
    /******************************************************************************/
    {
        aAlgorithm.enableDiagnostics();

        OrdinalType tMaxNumIterations = this->mInputData.getMaxNumIterations();
        OrdinalType tMaxTrustRegionIterations = this->mInputData.getKSMaxTrustRegionIterations();
        ScalarType tContractionScaleFactor = this->mInputData.getKSTrustRegionContractionFactor();
        ScalarType tExpansionScaleFactor = this->mInputData.getKSTrustRegionExpansionFactor();
        ScalarType tOuterGradientTolerance = this->mInputData.getKSOuterGradientTolerance();
        ScalarType tOuterStationarityTolerance = this->mInputData.getKSOuterStationarityTolerance();
        ScalarType tOuterObjectiveStagnationTolerance = this->mInputData.getKSOuterStagnationTolerance();
        ScalarType tOuterControlStagnationTolerance = this->mInputData.getKSOuterControlStagnationTolerance();
        ScalarType tOuterActualReductionTolerance = this->mInputData.getKSOuterActualReductionTolerance();
        ScalarType tInitialRadiusScale = this->mInputData.getKSInitialRadiusScale();
        ScalarType tMaxRadiusScale = this->mInputData.getKSMaxRadiusScale();
        OrdinalType tProblemUpdateFrequency = this->mInputData.getProblemUpdateFrequency();
        bool tDisablePostSmoothing = this->mInputData.getDisablePostSmoothing();
        ScalarType tTrustRegionRatioLow = this->mInputData.getKSTrustRegionRatioLow();
        ScalarType tTrustRegionRatioMid = this->mInputData.getKSTrustRegionRatioMid();
        ScalarType tTrustRegionRatioUpper = this->mInputData.getKSTrustRegionRatioUpper();
        ScalarType tMinTrustRegionRadius = this->mInputData.getMinTrustRegionRadius();
        ScalarType tMaxTrustRegionRadius = this->mInputData.getMaxTrustRegionRadius();

        aAlgorithm.setMaxNumIterations(tMaxNumIterations);
        aAlgorithm.setTrustRegionContraction(tContractionScaleFactor);
        aAlgorithm.setTrustRegionExpansion(tExpansionScaleFactor);
        aAlgorithm.setMaxNumTrustRegionSubProblemIterations(tMaxTrustRegionIterations);
        aAlgorithm.setGradientTolerance(tOuterGradientTolerance);
        aAlgorithm.setStationarityTolerance(tOuterStationarityTolerance);
        aAlgorithm.setControlStagnationTolerance(tOuterControlStagnationTolerance);
        aAlgorithm.setActualReductionTolerance(tOuterActualReductionTolerance);
        aAlgorithm.setObjectiveStagnationTolerance(tOuterObjectiveStagnationTolerance);
        aAlgorithm.setScaleOfUnitControlForInitialTrustRegionRadius(tInitialRadiusScale);
        aAlgorithm.setScaleOfUnitControlForMaxTrustRegionRadius(tMaxRadiusScale);
        aAlgorithm.setProblemUpdateFrequency(tProblemUpdateFrequency);
        aAlgorithm.setMaxTrustRegionRadius(tMaxTrustRegionRadius);
        aAlgorithm.setMinTrustRegionRadius(tMinTrustRegionRadius);
        if(tDisablePostSmoothing)
        {
            aAlgorithm.disablePostSmoothing();
        }
        aAlgorithm.setActualOverPredictedReductionLowerBound(tTrustRegionRatioLow);
        aAlgorithm.setActualOverPredictedReductionMidBound(tTrustRegionRatioMid);
        aAlgorithm.setActualOverPredictedReductionUpperBound(tTrustRegionRatioUpper);
    }
    /******************************************************************************/


    /******************************************************************************/
    void setUpperBounds(const Plato::AlgebraFactory<ScalarType, OrdinalType> & aAlgebraFactory,
                        Plato::DataFactory<ScalarType, OrdinalType> & aDataFactory,
                        Plato::TrustRegionAlgorithmDataMng<ScalarType, OrdinalType> & aDataMng)
    /******************************************************************************/
    {

        // ********* SET UPPER BOUNDS *********
        const OrdinalType tCONTROL_VECTOR_INDEX = 0;
        std::string tControlName = this->mInputData.getControlName(tCONTROL_VECTOR_INDEX);
        const OrdinalType tNumControls = this->mInterface->size(tControlName);
        std::shared_ptr<Plato::Vector<ScalarType, OrdinalType>> tUpperBoundVector =
                aAlgebraFactory.createVector(this->mComm, tNumControls, this->mInterface);

        ScalarType tUpperBoundValue = std::numeric_limits<ScalarType>::max();
        tUpperBoundVector->fill(tUpperBoundValue);
        aDataFactory.allocateUpperBoundVector(*tUpperBoundVector);

        if(mType == Plato::optimizer::algorithm_t::KELLEY_SACHS_BOUND_CONSTRAINED)
        {
            // ********* GET UPPER BOUNDS INFORMATION *********
            std::vector<ScalarType> tInputBoundsData(tNumControls);
            Plato::getUpperBoundsInputData(this->mInputData, this->mInterface, tInputBoundsData);
            Plato::copy(tInputBoundsData, *tUpperBoundVector);
        } // ELSE, PROBLEM IS UNCONSTRAINED

        aDataMng.setControlUpperBounds(tCONTROL_VECTOR_INDEX, *tUpperBoundVector);
    }

    /******************************************************************************/
    void setLowerBounds(const Plato::AlgebraFactory<ScalarType, OrdinalType> & aAlgebraFactory,
                        Plato::DataFactory<ScalarType, OrdinalType> & aDataFactory,
                        Plato::TrustRegionAlgorithmDataMng<ScalarType, OrdinalType> & aDataMng)
    /******************************************************************************/
    {
        // ********* SET LOWER BOUNDS *********
        const OrdinalType tCONTROL_VECTOR_INDEX = 0;
        std::string tControlName = this->mInputData.getControlName(tCONTROL_VECTOR_INDEX);
        const OrdinalType tNumControls = this->mInterface->size(tControlName);
        std::shared_ptr<Plato::Vector<ScalarType, OrdinalType>> tLowerBoundVector =
                aAlgebraFactory.createVector(this->mComm, tNumControls, this->mInterface);

        ScalarType tLowerBoundValue = static_cast<ScalarType>(-1) * std::numeric_limits<ScalarType>::max();
        tLowerBoundVector->fill(tLowerBoundValue);
        aDataFactory.allocateLowerBoundVector(*tLowerBoundVector);

        if(mType == Plato::optimizer::algorithm_t::KELLEY_SACHS_BOUND_CONSTRAINED)
        {
            // ********* GET LOWER BOUNDS INFORMATION *********
            std::vector<ScalarType> tInputBoundsData(tNumControls);
            Plato::getLowerBoundsInputData(this->mInputData, this->mInterface, tInputBoundsData);
            Plato::copy(tInputBoundsData, *tLowerBoundVector);
        } // ELSE, PROBLEM IS UNCONSTRAINED

        aDataMng.setControlLowerBounds(tCONTROL_VECTOR_INDEX, *tLowerBoundVector);
    }

    /******************************************************************************/
    void setInitialGuess(const Plato::AlgebraFactory<ScalarType, OrdinalType> & aAlgebraFactory,
                         Plato::TrustRegionAlgorithmDataMng<ScalarType, OrdinalType> & aDataMng)
    /******************************************************************************/
    {
        // ********* Allocate Plato::Vector of controls *********
        const OrdinalType tCONTROL_VECTOR_INDEX = 0;
        std::string tControlName = this->mInputData.getControlName(tCONTROL_VECTOR_INDEX);
        const OrdinalType tNumControls = this->mInterface->size(tControlName);
        std::vector<ScalarType> tInputIntitalGuessData(tNumControls);
        std::shared_ptr<Plato::Vector<ScalarType, OrdinalType>> tVector =
                aAlgebraFactory.createVector(this->mComm, tNumControls, this->mInterface);

        // ********* Set initial guess for each control vector *********
        Plato::getInitialGuessInputData(tControlName, this->mInputData, this->mInterface, tInputIntitalGuessData);
        Plato::copy(tInputIntitalGuessData, *tVector);
        aDataMng.setInitialGuess(tCONTROL_VECTOR_INDEX, *tVector);
    }

    /******************************************************************************/
    void allocateBaselineDataStructures(const Plato::AlgebraFactory<ScalarType, OrdinalType> & aAlgebraFactory,
                                        Plato::DataFactory<ScalarType, OrdinalType> & aDataFactory)
    /******************************************************************************/
    {
        // ********* Allocate control vectors baseline data structures *********
        const OrdinalType tNumVectors = this->mInputData.getNumControlVectors();
        assert(tNumVectors > static_cast<OrdinalType>(0));
        Plato::StandardMultiVector<ScalarType, OrdinalType> tMultiVector;
        for(OrdinalType tIndex = 0; tIndex < tNumVectors; tIndex++)
        {
            std::string tControlName = this->mInputData.getControlName(tIndex);
            const OrdinalType tNumControls = this->mInterface->size(tControlName);
            std::shared_ptr<Plato::Vector<ScalarType, OrdinalType>> tVector =
                    aAlgebraFactory.createVector(this->mComm, tNumControls, this->mInterface);
            tMultiVector.add(tVector);
        }
        aDataFactory.allocateControl(tMultiVector);
        std::shared_ptr<Plato::ReductionOperations<ScalarType, OrdinalType>> tReductionOperations =
                aAlgebraFactory.createReduction(this->mComm, this->mInterface);
        aDataFactory.allocateControlReductionOperations(*tReductionOperations);

        Plato::CommWrapper tCommWrapper(this->mComm);
        aDataFactory.setCommWrapper(tCommWrapper);
    }

private:
    Plato::optimizer::algorithm_t mType;

private:
    KelleySachsBoundConstrainedInterface(const Plato::KelleySachsBoundConstrainedInterface<ScalarType, OrdinalType>&);
    Plato::KelleySachsBoundConstrainedInterface<ScalarType, OrdinalType> & operator=(const Plato::KelleySachsBoundConstrainedInterface<ScalarType, OrdinalType>&);
}; // class KelleySachsBoundConstrainedInterface

} //namespace Plato

#endif /* PLATO_KELLEYSACHSBOUNDCONSTRAINEDINTERFACE_HPP_ */
