#include "OptimizationAlgorithmFactory.hpp"

OptimizationAlgorithmFactory::OptimizationAlgorithmFactory()
{

}

std::shared_ptr<OptimizationAlgorithm> OptimizationAlgorithmFactory::create(const InputData& aMetaData)
{
    auto tOptimizationAlgorithm = aMetaData.optimization_parameters().optimization_algorithm();  
    auto tOptimizationType = aMetaData.optimization_parameters().optimization_type();

    if(!tOptimizationAlgorithm.empty())
    {
        if(tOptimizationAlgorithm == "oc" || 
           tOptimizationAlgorithm == "mma" || 
           tOptimizationAlgorithm == "ksbc" || 
           tOptimizationAlgorithm == "ksal" )
            return std::make_shared<OptimizationAlgorithmPlato>(aMetaData.optimization_parameters());

        if(tOptimizationAlgorithm == "rol_linear_constraint" || 
           tOptimizationAlgorithm == "rol_bound_constraint" || 
           tOptimizationAlgorithm == "rol_augmented_lagrangian")
            return std::make_shared<OptimizationAlgorithmROL>(aMetaData.optimization_parameters());
    }
    if(tOptimizationType == OT_DAKOTA )
        return std::make_shared<OptimizationAlgorithmDakota>(aMetaData.optimization_parameters());

    return nullptr;
}