#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <algorithm>

#include "Plato_Interface.hpp"
#include "Plato_DistributedVectorROL.hpp"
#include "Plato_OptimizerEngineStageData.hpp"
#include "Plato_ReducedObjectiveROL.hpp"

namespace Plato
{

/******************************************************************************//**
 * \brief PLATO Engine interface to a ROL reduced objective function
 * \tparam scalar type, e.g. double, float, etc.
**********************************************************************************/
template<typename ScalarType>
class ReducedStochasticObjectiveROL : public ReducedObjectiveROL<ScalarType>
{
public:
    using Base = ROL::Objective<ScalarType>;

    /******************************************************************************//**
     * \brief Constructor
     * \param [in] aInputData XML input data
     * \param [in] aInterface PLATO Engine interface
     * \pre @a aInterface must not be `nullptr`
    **********************************************************************************/
    ReducedStochasticObjectiveROL(const Plato::OptimizerEngineStageData & aInputData, Plato::Interface* aInterface) :
        ReducedObjectiveROL<ScalarType>(aInputData, aInterface)
    {
    }

    ReducedStochasticObjectiveROL(const Plato::ReducedStochasticObjectiveROL<ScalarType> & aRhs) = delete;
    Plato::ReducedStochasticObjectiveROL<ScalarType> & operator=(const Plato::ReducedStochasticObjectiveROL<ScalarType> & aRhs) = delete;
    ReducedStochasticObjectiveROL(Plato::ReducedStochasticObjectiveROL<ScalarType> && aRhs) = delete;
    Plato::ReducedStochasticObjectiveROL<ScalarType> & operator=(Plato::ReducedStochasticObjectiveROL<ScalarType> && aRhs) = delete;

    /******************************************************************************//**
     * \brief Updates physics and enables continuation of app-based parameters.
     * \param [in] aControl design variables
     * \param [in] aFlag indicates if vector of design variables was updated
     * \param [in] aIteration outer loop optimization iteration
    **********************************************************************************/
    using ROL::Objective<ScalarType>::update;
    void update(const ROL::Vector<ScalarType> & aControl, ROL::UpdateType aUpdateType, int aIteration = -1) override
    {
        ReducedObjectiveROL<ScalarType>::update(aControl, aUpdateType, aIteration);
    }

    /******************************************************************************//**
     * \brief Returns current objective value
     * \param [in] aControl design variables
     * \param [in] aTolerance inexactness tolerance
     * \return objective function value
    **********************************************************************************/
    ScalarType value(const ROL::Vector<ScalarType> & aControl, ScalarType & aTolerance) override
    {
        updateStochasticParameters();
        return ReducedObjectiveROL<ScalarType>::value(aControl, aTolerance);
    }

    /******************************************************************************//**
     * \brief Returns current gradient value
     * \param [out] aGradient objective function gradient
     * \param [in] aControl design variables
     * \param [in] aTolerance inexactness tolerance
    **********************************************************************************/
    void gradient(ROL::Vector<ScalarType> & aGradient, const ROL::Vector<ScalarType> & aControl, ScalarType & aTolerance) override
    {
        updateStochasticParameters();
        ReducedObjectiveROL<ScalarType>::gradient(aGradient, aControl, aTolerance);
    }
    /******************************************************************************//**
     * \brief Returns current hessian applied to a vector
     * \param [out] aHessVec objective function Hessian applied to a vector
     * \param [in] aVector design variable direction vector
     * \param [in] aControl design variables
     * \param [in] aTolerance inexactness tolerance
     **********************************************************************************/
    void hessVec(ROL::Vector<ScalarType> & aHessVec, const ROL::Vector<ScalarType> & aVector, const ROL::Vector<ScalarType> & aControl, ScalarType & aTolerance) override
    {
        updateStochasticParameters();
        ReducedObjectiveROL<ScalarType>::hessVec(aHessVec, aVector, aControl, aTolerance);
    }

private:
    void updateStochasticParameters()
    {
        this->unsetComputedStateFlags(); // TODO: Figure out how to cache states based on stochastic parameters
        assert(this->getParameter().size() == this->engineInputData().getStochasticParameterNames().size());
        for(int i = 0; i < this->getParameter().size(); ++i)
        {
            const double tStochasticParameterValue = this->getParameter()[i];
            const std::string& tStochasticParameterName = this->engineInputData().getStochasticParameterNames().at(i);
            this->interface()->setParameterOnOperation(
                StageName{this->engineInputData().getObjectiveValueStageName()}, 
                OperationName{this->engineInputData().getObjectiveValueParametersOperationName()},
                ParameterName{tStochasticParameterName},
                tStochasticParameterValue);
            this->interface()->setParameterOnOperation(
                StageName{this->engineInputData().getObjectiveGradientStageName()}, 
                OperationName{this->engineInputData().getObjectiveGradientParametersOperationName()},
                ParameterName{tStochasticParameterName}, 
                tStochasticParameterValue);
            if(!this->engineInputData().getObjectiveHessianStageName().empty())
            {
                this->interface()->setParameterOnOperation(
                    StageName{this->engineInputData().getObjectiveHessianStageName()}, 
                    OperationName{this->engineInputData().getObjectiveHessianParametersOperationName()},
                    ParameterName{tStochasticParameterName}, 
                    tStochasticParameterValue);
            }
        }
   }
};
// class ReducedStochasticObjectiveROL

}
// namespace Plato
