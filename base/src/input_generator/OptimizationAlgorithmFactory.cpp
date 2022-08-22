#include "OptimizationAlgorithmFactory.hpp"
#include "OptimizationAlgorithm.hpp"
using namespace XMLGen;
namespace director
{

OptimizationAlgorithmFactory::OptimizationAlgorithmFactory()
{

}

std::shared_ptr<OptimizationAlgorithm> OptimizationAlgorithmFactory::create(const XMLGen::InputData& aMetaData)
{
    auto tOptimizationAlgorithm = aMetaData.optimization_parameters().optimization_algorithm();  
    auto tOptimizationType = aMetaData.optimization_parameters().optimization_type();

    if(!tOptimizationAlgorithm.empty())
    {
        if(tOptimizationAlgorithm == "oc" )
            return std::make_shared<OptimizationAlgorithmPlatoOC>(aMetaData.optimization_parameters());
        if(tOptimizationAlgorithm == "ksbc" )
            return std::make_shared<OptimizationAlgorithmPlatoKSBC>(aMetaData.optimization_parameters());
        if(tOptimizationAlgorithm == "ksal" )
            return std::make_shared<OptimizationAlgorithmPlatoKSAL>(aMetaData.optimization_parameters());
        if(tOptimizationAlgorithm == "mma" )
            return std::make_shared<OptimizationAlgorithmPlatoMMA>(aMetaData.optimization_parameters());
        // if(tOptimizationAlgorithm == "rol_linear_constraint")
        //     return std::make_shared<OptimizationAlgorithmROLLC>(aMetaData.optimization_parameters());
        // if(tOptimizationAlgorithm == "rol_bound_constraint")
        //     return std::make_shared<OptimizationAlgorithmROLBC>(aMetaData.optimization_parameters());
        // if(tOptimizationAlgorithm == "rol_augmented_lagrangian")
        //     return std::make_shared<OptimizationAlgorithmROLAL>(aMetaData.optimization_parameters());
    }
    // if(tOptimizationType == "OT_DAKOTA" )
    //     return std::make_shared<OptimizationAlgorithmDakota>(aMetaData.optimization_parameters());

    return nullptr;
}

}// namespace
