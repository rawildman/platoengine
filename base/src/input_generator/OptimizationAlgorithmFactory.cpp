#include "OptimizationAlgorithmFactory.hpp"
#include "OptimizationAlgorithm.hpp"

namespace XMLGen
{

OptimizationAlgorithmFactory::OptimizationAlgorithmFactory()
{

}

std::shared_ptr<OptimizationAlgorithm> OptimizationAlgorithmFactory::create(const InputData& aMetaData)
{
    auto tOptimizationAlgorithm = aMetaData.optimization_parameters().optimization_algorithm();  
    auto tOptimizationType = aMetaData.optimization_parameters().optimization_type();

    if(!tOptimizationAlgorithm.empty())
    {
        if(tOptimizationAlgorithm == "oc" )
            return std::make_shared<OptimizationAlgorithmPlatoOC>(aMetaData.optimization_parameters());
        if(tOptimizationAlgorithm == "ksbc" )
            return std::make_shared<OptimizationAlgorithmPlatoKSBC>(aMetaData.optimization_parameters());

//           tOptimizationAlgorithm == "mma" || 
  //         tOptimizationAlgorithm == "ksal" )

        if(tOptimizationAlgorithm == "rol_linear_constraint" || 
           tOptimizationAlgorithm == "rol_bound_constraint" || 
           tOptimizationAlgorithm == "rol_augmented_lagrangian")
            return std::make_shared<OptimizationAlgorithmROL>(aMetaData.optimization_parameters());
    }
    if(tOptimizationType == "OT_DAKOTA" )
        return std::make_shared<OptimizationAlgorithmDakota>(aMetaData.optimization_parameters());

    return nullptr;
}

}// namespace
