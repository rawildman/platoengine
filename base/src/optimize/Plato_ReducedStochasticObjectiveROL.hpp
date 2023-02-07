#pragma once

#include "Plato_Interface.hpp"
#include "Plato_DistributedVectorROL.hpp"
#include "Plato_OptimizerEngineStageData.hpp"
#include "Plato_ReducedObjectiveROL.hpp"

#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <algorithm>

namespace Plato
{

/// Implements the ROL::Objective interface, specialized for stochastic problems.
/// In particular, maps stochastic parameters from ROL::Objective::getParameters
/// to internal parameters set on Operations. These are communicated through
/// shared data.
template<typename ScalarType>
class ReducedStochasticObjectiveROL : public ReducedObjectiveROL<ScalarType>
{
public:
    ReducedStochasticObjectiveROL(const Plato::OptimizerEngineStageData & aInputData, Plato::Interface* aInterface) :
        ReducedObjectiveROL<ScalarType>(aInputData, aInterface)
    {
    }

    ReducedStochasticObjectiveROL(const Plato::ReducedStochasticObjectiveROL<ScalarType> & aRhs) = delete;
    Plato::ReducedStochasticObjectiveROL<ScalarType> & operator=(const Plato::ReducedStochasticObjectiveROL<ScalarType> & aRhs) = delete;
    ReducedStochasticObjectiveROL(Plato::ReducedStochasticObjectiveROL<ScalarType> && aRhs) = delete;
    Plato::ReducedStochasticObjectiveROL<ScalarType> & operator=(Plato::ReducedStochasticObjectiveROL<ScalarType> && aRhs) = delete;

    using ROL::Objective<ScalarType>::update;
    void update(const ROL::Vector<ScalarType> & aControl, ROL::UpdateType aUpdateType, int aIteration = -1) override
    {
        ReducedObjectiveROL<ScalarType>::update(aControl, aUpdateType, aIteration);
    }

    ScalarType value(const ROL::Vector<ScalarType> & aControl, ScalarType & aTolerance) override
    {
        updateStochasticParameters();
        return ReducedObjectiveROL<ScalarType>::value(aControl, aTolerance);
    }

    void gradient(ROL::Vector<ScalarType> & aGradient, const ROL::Vector<ScalarType> & aControl, ScalarType & aTolerance) override
    {
        updateStochasticParameters();
        ReducedObjectiveROL<ScalarType>::gradient(aGradient, aControl, aTolerance);
    }

    void hessVec(ROL::Vector<ScalarType> & aHessVec, const ROL::Vector<ScalarType> & aVector, const ROL::Vector<ScalarType> & aControl, ScalarType & aTolerance) override
    {
        updateStochasticParameters();
        ReducedObjectiveROL<ScalarType>::hessVec(aHessVec, aVector, aControl, aTolerance);
    }

    void validate() const
    {
        for(const std::string& tParameter : this->engineInputData().getStochasticParameterNames())
        {
            validateStageOperationAndParameter(
                StageName{this->engineInputData().getObjectiveValueStageName()}, 
                OperationName{this->engineInputData().getObjectiveValueParametersOperationName()},
                ParameterName{tParameter});
            validateStageOperationAndParameter(
                StageName{this->engineInputData().getObjectiveGradientStageName()}, 
                OperationName{this->engineInputData().getObjectiveGradientParametersOperationName()},
                ParameterName{tParameter});
            if(!this->engineInputData().getObjectiveHessianStageName().empty())
            {
                validateStageOperationAndParameter(
                    StageName{this->engineInputData().getObjectiveHessianStageName()}, 
                    OperationName{this->engineInputData().getObjectiveHessianParametersOperationName()},
                    ParameterName{tParameter});
            }
        }

    }

private:
    void validateStageOperationAndParameter(
        const StageName& aStageName,
        const OperationName& aOperationName, 
        const ParameterName& aParameterName) const
    {
        if(!this->interface()->hasStageOperationAndParameter(aStageName, aOperationName, aParameterName))
        {
            this->interface()->registerException(ParsingException(
                R"(While setting an Operation Parameter, couldn't find requested Stage ")" + aStageName.mValue 
                + R"(", Operation ")" + aOperationName.mValue
                + R"(" and/or Parameter ")" + aParameterName.mValue
                + R"(". Please check interface file.)"));
        }
    }

    void updateStochasticParameters()
    {
        this->unsetComputedStateFlags(); // TODO: Figure out how to cache states based on stochastic parameters
        assert(this->getParameter().size() == this->engineInputData().getStochasticParameterNames().size());
        for(size_t i = 0; i < this->getParameter().size(); ++i)
        {
            const double tStochasticParameterValue = this->getParameter()[i];
            const std::string& tStochasticParameterName = this->engineInputData().getStochasticParameterNames().at(i);
            this->debugOutput("   Updating stochastic parameter with name " + tStochasticParameterName + " to " + std::to_string(tStochasticParameterValue));
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

}
// namespace Plato
