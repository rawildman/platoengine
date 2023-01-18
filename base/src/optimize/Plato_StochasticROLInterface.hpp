#pragma once

#include "Plato_ROLInterface.hpp"

#include "ROL_BatchManager.hpp"
#include "ROL_MonteCarloGenerator.hpp"
#include "ROL_StochasticProblem.hpp"
#include "ROL_Uniform.hpp"

namespace Plato
{

template<typename ScalarType, typename OrdinalType = size_t>
class StochasticROLInterface: public ROLInterface<ScalarType,OrdinalType>
{
public:
    StochasticROLInterface(Plato::Interface* aInterface, const MPI_Comm & aComm, const Plato::optimizer::algorithm_t& aType):
        ROLInterface<ScalarType, OrdinalType>(aInterface, aComm, aType)
    {
    }

    void run() override
    {
        // TODO: Integrate this with base class's implementation to not repeat code
        this->initialize();
        this->validateDistributionNames();
        this->initializeDistributions();
        constexpr OrdinalType tCONTROL_VECTOR_INDEX = 0;
        const std::string tControlName = this->mInputData.getControlName(tCONTROL_VECTOR_INDEX);
        const OrdinalType tNumControls = this->mInterface->size(tControlName);
        auto tControlBoundsMng = this->setControlBounds(tNumControls);
        
        /******************************** SET CONTROL INITIAL GUESS *********************************/
        Teuchos::RCP<Plato::DistributedVectorROL<ScalarType>> tControls =
                Teuchos::rcp(new Plato::DistributedVectorROL<ScalarType>(this->mComm, tNumControls));
        this->setInitialGuess(tControlName, *tControls);
        
        /********************************* SET OPTIMIZATION PROBLEM *********************************/
        Teuchos::RCP<ROL::Objective<ScalarType>> tObjective = Teuchos::rcp(new Plato::ReducedStochasticObjectiveROL<ScalarType>(this->mInputData, this->mInterface));
        ROL::Ptr<ROL::Problem<ScalarType>> tOptimizationProblem = ROL::makePtr<ROL::Problem<ScalarType>>(tObjective, tControls);
                
        tOptimizationProblem->addBoundConstraint(tControlBoundsMng);
        this->createOptimizationProblemLinearConstraint(*tOptimizationProblem);
        
        ROL::Ptr<ROL::StochasticProblem<ScalarType>> tStochasticOptimizationProblem = ROL::makePtr<ROL::StochasticProblem<ScalarType>>(*tOptimizationProblem);

        auto tParameterList = this->updateParameterListFromRolInputsFile();

        tStochasticOptimizationProblem->makeObjectiveStochastic(*tParameterList, mSampler);

        const bool tLumpConstraints = ( this->algorithm() == Plato::optimizer::algorithm_t::ROL_LINEAR_CONSTRAINT ? false : true );
        constexpr bool tPrintToStream = true;

        tStochasticOptimizationProblem->finalize(tLumpConstraints, tPrintToStream, this->mOutputFile);

        this->solve(tStochasticOptimizationProblem);
        this->finalize();
    }

private:
    /// Checks that each stochastic parameter has a matching distribution defined in the distributions
    /// input file. If a parameter does not have a distribution, an exception is registered.
    void validateDistributionNames()
    {
        const std::string& tDistributionsFile = this->mInputData.getROLStochasticDistributionsFile();
        const Teuchos::RCP<Teuchos::ParameterList> tDistributionParameters = 
            Teuchos::getParametersFromXmlFile(tDistributionsFile);
        for(const auto& tParameterName : this->mInputData.getStochasticParameterNames())
        {
            if(!tDistributionParameters->isSublist(tParameterName))
            {
                this->mInterface->registerException(
                    ParsingException("Could not find distribution with name " 
                    + tParameterName + ". Check input file " + tDistributionsFile));
            }
        }
    }
    /// @brief Construct all probability distributions defined in an input file and construct the
    ///  random sampler from those distributions.
    /// @pre validateDistributionNames should be called first. Otherwise, non-existent distributions 
    ///  default to uniform on [0, 1], which may give unexpected results.
    void initializeDistributions()
    {
        mNumSamples = this->mInputData.getROLStochasticNumberOfSamples();
        const std::string& tDistributionsFile = this->mInputData.getROLStochasticDistributionsFile();
        const Teuchos::RCP<Teuchos::ParameterList> tDistributionParameters = 
            Teuchos::getParametersFromXmlFile(tDistributionsFile);
        for(const auto& tParameterName : this->mInputData.getStochasticParameterNames())
        {
            Teuchos::ParameterList tDistributionForParameter;
            tDistributionForParameter.sublist("SOL").sublist("Distribution") = tDistributionParameters->sublist(tParameterName);
            ROL::Ptr<ROL::Distribution<ScalarType>> tDistribution = 
                ROL::DistributionFactory<ScalarType>(tDistributionForParameter);
            mSampleDistributions.push_back(tDistribution);
        }
        // Default values from ROL
        constexpr bool tUseSA = false;
        constexpr bool tAdaptive = false;
        constexpr int tNumNewSamples = 0;
        mSampler = ROL::makePtr<ROL::MonteCarloGenerator<ScalarType>>(mNumSamples, mSampleDistributions, mBatchManager, 
            tUseSA, tAdaptive, tNumNewSamples, this->mInputData.getROLStochasticSamplerSeed());
    }

private:
    int mNumSamples = 2;
    std::vector<ROL::Ptr<ROL::Distribution<ScalarType>>> mSampleDistributions;
    ROL::Ptr<ROL::BatchManager<ScalarType>> mBatchManager = ROL::makePtr<ROL::BatchManager<ScalarType>>();
    ROL::Ptr<ROL::SampleGenerator<ScalarType>> mSampler;
};

} // namespace Plato
