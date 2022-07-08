/*
 * Plato_ROLSPGInterface.hpp
 *
 *  Created on: May 3, 2021
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

#ifndef PLATO_ROLSPGINTERFACE_HPP_
#define PLATO_ROLSPGINTERFACE_HPP_

#include <mpi.h>

#include <string>
#include <memory>
#include <sstream>
#include <cassert>
#include <fstream>

#include "Teuchos_XMLParameterListHelpers.hpp"

#include "ROL_Bounds.hpp"
#include "ROL_Solver.hpp"

#include "Plato_Parser.hpp"
#include "Plato_Interface.hpp"
#include "Plato_OptimizerInterface.hpp"
#include "Plato_SerialVectorROL.hpp"
#include "Plato_OptimizerUtilities.hpp"
#include "Plato_ReducedObjectiveROL.hpp"
#include "Plato_ReducedConstraintROL.hpp"
#include "Plato_DistributedVectorROL.hpp"
#include "Plato_OptimizerEngineStageData.hpp"

namespace Plato
{

template<typename ScalarType, typename OrdinalType = size_t>
class ROLSPGInterface : public Plato::OptimizerInterface<ScalarType, OrdinalType>
{
public:
    /******************************************************************************/
    ROLSPGInterface(Plato::Interface* aInterface, const MPI_Comm & aComm) :
        Plato::OptimizerInterface<ScalarType, OrdinalType>::OptimizerInterface(aInterface, aComm) { }

    virtual ~ROLSPGInterface() = default;

    /******************************************************************************/
    Plato::optimizer::algorithm_t algorithm() const
    /******************************************************************************/
    {
        return (Plato::optimizer::algorithm_t::ROL_SPG);
    }

    /******************************************************************************/
    void initialize()
    /******************************************************************************/
    {
        Plato::initialize<ScalarType, OrdinalType>(mInterface, mInputData
                                                   this->mOptimizerIndex);
    }

    /******************************************************************************/
    void run()
    /******************************************************************************/
    {
        this->initialize();

        /****************************** GET NUMBER OF DESIGN VARIABLES ******************************/
        const size_t tMY_CONTROL_INDEX = 0;
        std::string tMyName = mInputData.getControlName(tMY_CONTROL_INDEX);
        const OrdinalType tMyNumControls = mInterface->size(tMyName);

        /************************************ SET CONTROL BOUNDS ************************************/
        Teuchos::RCP<Plato::DistributedVectorROL<ScalarType>> tControlLowerBounds =
                Teuchos::rcp(new Plato::DistributedVectorROL<ScalarType>(mComm, tMyNumControls));
        std::vector<ScalarType> tLowerBounds = mInputData.getLowerBoundValues();
        this->setBounds(tLowerBounds, tControlLowerBounds.operator*());

        Teuchos::RCP<Plato::DistributedVectorROL<ScalarType>> tControlUpperBounds =
                Teuchos::rcp(new Plato::DistributedVectorROL<ScalarType>(mComm, tMyNumControls));
        std::vector<ScalarType> tUpperBounds = mInputData.getUpperBoundValues();
        this->setBounds(tUpperBounds, tControlUpperBounds.operator*());
        Teuchos::RCP<ROL::BoundConstraint<ScalarType>> tControlBoundsMng =
                Teuchos::rcp(new ROL::Bounds<ScalarType>(tControlLowerBounds, tControlUpperBounds));

        // ********* ALLOCATE LINEAR ALGEBRA FACTORY ********* //
        Plato::AlgebraFactory<ScalarType, OrdinalType> tLinearAlgebraFactory;

        // ********* ALLOCATE OPTIMIZER'S BASELINE DATA STRUCTURES *********
        std::shared_ptr<Plato::DataFactory<ScalarType, OrdinalType>> tDataFactory =
                std::make_shared<Plato::DataFactory<ScalarType, OrdinalType>>();
        this->allocateBaselineDataStructures(tLinearAlgebraFactory, *tDataFactory);
        // ********* ALLOCATE OPTIMIZER'S DATA MANAGER *********
        std::shared_ptr<Plato::OptimalityCriteriaDataMng<ScalarType, OrdinalType>> tDataMng =
                std::make_shared<Plato::OptimalityCriteriaDataMng<ScalarType, OrdinalType>>(tDataFactory);

        // ********* SET LOWER AND UPPER BOUNDS FOR CONTROLS *********
        this->setLowerBounds(tLinearAlgebraFactory, *tDataFactory, *tDataMng);
        this->setUpperBounds(tLinearAlgebraFactory, *tDataFactory, *tDataMng);

        /******************************** SET CONTROL INITIAL GUESS *********************************/
        Teuchos::RCP<Plato::DistributedVectorROL<ScalarType>> tControls =
                Teuchos::rcp(new Plato::DistributedVectorROL<ScalarType>(mComm, tMyNumControls));
        this->setInitialGuess(tMyName, tControls.operator*());

        /****************************** SET DUAL VECTOR *******************************/
        const OrdinalType tNumConstraints = mInputData.getNumConstraints();
        Teuchos::RCP<Plato::SerialVectorROL<ScalarType>> tDual = Teuchos::rcp(new Plato::SerialVectorROL<ScalarType>(tNumConstraints));

        /********************************* SET OPTIMIZATION PROBLEM *********************************/
        Teuchos::RCP<ROL::Objective<ScalarType>> tObjective = Teuchos::rcp(new Plato::ReducedObjectiveROL<ScalarType>(mInputData, mInterface));
        Teuchos::RCP<ROL::Constraint<ScalarType>> tInequality = Teuchos::rcp(new Plato::ReducedConstraintROL<ScalarType>(mInputData, mInterface));
        ROL::Ptr<ROL::Problem<ScalarType>> tOptimizationProblem =
            ROL::makePtr<ROL::Problem<ScalarType>>(tObjective, tControls);
        tOptimizationProblem->addBoundConstraint(tControlBoundsMng);
        tOptimizationProblem->addLinearConstraint("Constraint", tInequality, tDual);

        /******************************** SOLVE OPTIMIZATION PROBLEM ********************************/
        this->solve(tOptimizationProblem);
    }

    /******************************************************************************//**
     * @brief All optimizing is done so do any optional final
     * stages. Called only once from the interface.
    **********************************************************************************/
    void finalize()
    /******************************************************************************/
    {
        mInterface->finalize();
    }

private:
    /******************************************************************************/
    void allocateBaselineDataStructures(const Plato::AlgebraFactory<ScalarType, OrdinalType> & aAlgebraFactory,
                                        Plato::DataFactory<ScalarType, OrdinalType> & aDataFactory)
    /******************************************************************************/
    {
        const OrdinalType tNumDuals = mInputData.getNumConstraints();
        Plato::StandardVector<ScalarType, OrdinalType> tDuals(tNumDuals);
        aDataFactory.allocateDual(tDuals);

        // ********* Allocate control vectors baseline data structures *********
        const OrdinalType tNumVectors = mInputData.getNumControlVectors();
        assert(tNumVectors > static_cast<OrdinalType>(0));
        Plato::StandardMultiVector<ScalarType, OrdinalType> tMultiVector;
        for(OrdinalType tIndex = 0; tIndex < tNumVectors; tIndex++)
        {
            std::string tControlName = mInputData.getControlName(tIndex);
            const OrdinalType tNumControls = mInterface->size(tControlName);
            std::shared_ptr<Plato::Vector<ScalarType, OrdinalType>> tVector =
                    aAlgebraFactory.createVector(mComm, tNumControls, mInterface);
            tMultiVector.add(tVector);
        }
        aDataFactory.allocateControl(tMultiVector);
        std::shared_ptr<Plato::ReductionOperations<ScalarType, OrdinalType>> tReductionOperations =
                aAlgebraFactory.createReduction(mComm, mInterface);
        aDataFactory.allocateControlReductionOperations(*tReductionOperations);

        Plato::CommWrapper tCommWrapper(mComm);
        aDataFactory.setCommWrapper(tCommWrapper);
    }
    /******************************************************************************/
    void setUpperBounds(const Plato::AlgebraFactory<ScalarType, OrdinalType> & aAlgebraFactory,
                        Plato::DataFactory<ScalarType, OrdinalType> & aDataFactory,
                        Plato::OptimalityCriteriaDataMng<ScalarType, OrdinalType> & aDataMng)
    /******************************************************************************/
    {
        const OrdinalType tCONTROL_VECTOR_INDEX = 0;
        std::string tControlName = mInputData.getControlName(tCONTROL_VECTOR_INDEX);
        const OrdinalType tNumControls = mInterface->size(tControlName);
        std::vector<ScalarType> tInputBoundsData(tNumControls);

        // ********* GET UPPER BOUNDS INFORMATION *********
        Plato::getUpperBoundsInputData(mInputData, mInterface, tInputBoundsData);

        // ********* SET UPPER BOUNDS FOR OPTIMIZER *********
        std::shared_ptr<Plato::Vector<ScalarType, OrdinalType>> tUpperBoundVector =
                aAlgebraFactory.createVector(mComm, tNumControls, mInterface);
        aDataFactory.allocateUpperBoundVector(*tUpperBoundVector);
        Plato::copy(tInputBoundsData, *tUpperBoundVector);
        aDataMng.setControlUpperBounds(tCONTROL_VECTOR_INDEX, *tUpperBoundVector);
    }

    /******************************************************************************/
    void setLowerBounds(const Plato::AlgebraFactory<ScalarType, OrdinalType> & aAlgebraFactory,
                        Plato::DataFactory<ScalarType, OrdinalType> & aDataFactory,
                        Plato::OptimalityCriteriaDataMng<ScalarType, OrdinalType> & aDataMng)
    /******************************************************************************/
    {
        const OrdinalType tCONTROL_VECTOR_INDEX = 0;
        std::string tControlName = mInputData.getControlName(tCONTROL_VECTOR_INDEX);
        const OrdinalType tNumControls = mInterface->size(tControlName);
        std::vector<ScalarType> tInputBoundsData(tNumControls);

        // ********* GET LOWER BOUNDS INFORMATION *********
        Plato::getLowerBoundsInputData(mInputData, mInterface, tInputBoundsData);

        // ********* SET LOWER BOUNDS FOR OPTIMIZER *********
        std::shared_ptr<Plato::Vector<ScalarType, OrdinalType>> tLowerBoundVector =
                aAlgebraFactory.createVector(mComm, tNumControls, mInterface);
        aDataFactory.allocateLowerBoundVector(*tLowerBoundVector);
        Plato::copy(tInputBoundsData, *tLowerBoundVector);
        aDataMng.setControlLowerBounds(tCONTROL_VECTOR_INDEX, *tLowerBoundVector);
    }
    /******************************************************************************/
    void solve(const ROL::Ptr<ROL::Problem<ScalarType>> & aOptimizationProblem)
    /******************************************************************************/
    {
        std::string tFileName = mInputData.getInputFileName();
        Teuchos::RCP<Teuchos::ParameterList> tParameterList = Teuchos::rcp(new Teuchos::ParameterList);
        Teuchos::updateParametersFromXmlFile(tFileName, tParameterList.ptr());
        aOptimizationProblem->setProjectionAlgorithm(*tParameterList);
        aOptimizationProblem->finalize(false, true, std::cout);
        if(mInputData.getCheckGradient() == true)
        {
            /**************************** CHECK DERIVATIVES ****************************/
            aOptimizationProblem->check(true, std::cout);
        }
        else
        {
            /************************ SOLVE OPTIMIZATION PROBLEM ***********************/
            ROL::Solver<ScalarType> tOptimizer(aOptimizationProblem, *tParameterList);
            tOptimizer.solve(this->mOutputStream);
        }

        this->printControl(aOptimizationProblem);
    }

private:
    ROLSPGInterface(const Plato::ROLSPGInterface<ScalarType> & aRhs);
    Plato::ROLSPGInterface<ScalarType> & operator=(const Plato::ROLSPGInterface<ScalarType> & aRhs);
};

} // namespace Plato

#endif /* PLATO_ROLSPGINTERFACE_HPP_ */
