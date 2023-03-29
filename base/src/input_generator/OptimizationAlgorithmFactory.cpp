#include "OptimizationAlgorithmFactory.hpp"
#include "OptimizationAlgorithm.hpp"

using namespace XMLGen;
namespace director
{

std::shared_ptr<OptimizationAlgorithm> OptimizationAlgorithmFactory::create(const XMLGen::InputData& aMetaData)
{
    const auto tOptimizationAlgorithm = aMetaData.optimization_parameters().optimization_algorithm();  
    const auto tOptimizationType = aMetaData.optimization_parameters().optimization_type();

    if(!tOptimizationAlgorithm.empty())
    {
        if(tOptimizationAlgorithm == "oc" )
            return std::make_shared<OptimizationAlgorithmPlatoOC>(aMetaData.optimization_parameters());
        if(tOptimizationAlgorithm == "ksbc" )
            return std::make_shared<OptimizationAlgorithmPlatoKSBC>(aMetaData.optimization_parameters());
        if(tOptimizationAlgorithm == "ksal" )
            return std::make_shared<OptimizationAlgorithmPlatoKSAL>(aMetaData.optimization_parameters());
    }

    return nullptr;
}

}// namespace
