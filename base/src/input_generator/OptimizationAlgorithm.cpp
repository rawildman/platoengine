#include "OptimizationAlgorithm.hpp"
#include "pugixml.hpp"
#include "XMLGeneratorUtilities.hpp"

namespace XMLGen
{

OptimizationAlgorithm::OptimizationAlgorithm(const OptimizationParameters& aParameters)
{
    mMaxIterations = aParameters.max_iterations();
}

//****************************PLATO **********************************************//
OptimizationAlgorithmPlatoOC::OptimizationAlgorithmPlatoOC(const OptimizationParameters& aParameters)
: OptimizationAlgorithm(aParameters)
{
    mControlStagnationTolerance = aParameters.oc_control_stagnation_tolerance();
    mObjectiveStagnationTolerance = aParameters.oc_objective_stagnation_tolerance();
    mGradientTolerance = aParameters.oc_gradient_tolerance();
    mProblemUpdateFrequency = aParameters.problem_update_frequency();
}
void OptimizationAlgorithmPlatoOC::writeInterface(pugi::xml_node& aNode)
{
    auto tOptimizer = aNode.append_child("Optimizer");
    addChild(tOptimizer, "Package","OC");
    
    /*if(mControlStagnationTolerance != "" || 
       mObjectiveStagnationTolerance != "" || 
       mGradientTolerance != ""|| 
       mProblemUpdateFrequency != "")*/
    {   
        auto tOptions = tOptimizer.append_child("Options");
        if(mControlStagnationTolerance!="")    
            addChild(tOptions, "OCControlStagnationTolerance",mControlStagnationTolerance);
        if(mObjectiveStagnationTolerance != "")
            addChild(tOptions, "OCObjectiveStagnationTolerance",mObjectiveStagnationTolerance);
        if(mGradientTolerance != "")
            addChild(tOptions, "OCGradientTolerance",mGradientTolerance);
        if(mProblemUpdateFrequency != "")
            addChild(tOptions, "ProblemUpdateFrequency",mProblemUpdateFrequency);
    }
    auto tConvergence = tOptimizer.append_child("Convergence");
    addChild(tConvergence, "MaxIterations",mMaxIterations);

}

void OptimizationAlgorithmPlatoOC::writeAuxiliaryFiles(pugi::xml_node& aNode)
{

}

OptimizationAlgorithmPlatoKSBC::OptimizationAlgorithmPlatoKSBC(const OptimizationParameters& aParameters)
: OptimizationAlgorithm(aParameters)
{
    mMaxNumOuterIterations= aParameters.max_iterations(); ///<<<<<<<<<
    mTrustRegionExpansionFactor = aParameters.ks_trust_region_expansion_factor();
    mTrustRegionContractionFactor = aParameters.ks_trust_region_contraction_factor();
    mMaxTrustRegionIterations = aParameters.ks_max_trust_region_iterations();

    mInitialRadiusScale = aParameters.ks_initial_radius_scale();
    mMaxRadiusScale = aParameters.ks_max_radius_scale();
    mHessianType = aParameters.hessian_type();
    mMinTrustRegionRadius = aParameters.ks_min_trust_region_radius();
    mLimitedMemoryStorage = aParameters.limited_memory_storage();
    
    mOuterGradientTolerance = aParameters.ks_outer_gradient_tolerance();
    mOuterStationarityTolerance = aParameters.ks_outer_stationarity_tolerance();
    mOuterStagnationTolerance = aParameters.ks_outer_stagnation_tolerance();
    mOuterControlStagnationTolerance = aParameters.ks_outer_control_stagnation_tolerance();
    mOuterActualReductionTolerance = aParameters.ks_outer_actual_reduction_tolerance();
    
    mProblemUpdateFrequency = aParameters.problem_update_frequency();
    mDisablePostSmoothing = aParameters.ks_disable_post_smoothing();
    mTrustRegionRatioLow = aParameters.ks_trust_region_ratio_low();
    mTrustRegionRatioMid = aParameters.ks_trust_region_ratio_mid();
    mTrustRegionRatioUpper = aParameters.ks_trust_region_ratio_high();
}
void OptimizationAlgorithmPlatoKSBC::writeInterface(pugi::xml_node& aNode)
{
    auto tOptimizer = aNode.append_child("Optimizer");
    addChild(tOptimizer, "Package","KSBC");
    
    auto tOptions = tOptimizer.append_child("Options");
    addChild(tOptions, "MaxNumOuterIterations",mMaxNumOuterIterations);
    addChild(tOptions, "KSTrustRegionExpansionFactor",mTrustRegionExpansionFactor);
    addChild(tOptions, "KSTrustRegionContractionFactor",mTrustRegionContractionFactor);
    addChild(tOptions, "KSMaxTrustRegionIterations",mMaxTrustRegionIterations);
    addChild(tOptions, "KSInitialRadiusScale",mInitialRadiusScale);
    addChild(tOptions, "KSMaxRadiusScale",mMaxRadiusScale);
    addChild(tOptions, "HessianType",mHessianType);
    addChild(tOptions, "MinTrustRegionRadius",mMinTrustRegionRadius);
    addChild(tOptions, "LimitedMemoryStorage",mLimitedMemoryStorage);
    addChild(tOptions, "KSOuterGradientTolerance",mOuterGradientTolerance);
    addChild(tOptions, "KSOuterStationarityTolerance",mOuterStationarityTolerance);
    addChild(tOptions, "KSOuterStagnationTolerance",mOuterStagnationTolerance);
    addChild(tOptions, "KSOuterControlStagnationTolerance",mOuterControlStagnationTolerance);
    addChild(tOptions, "KSOuterActualReductionTolerance",mOuterActualReductionTolerance);
    addChild(tOptions, "ProblemUpdateFrequency",mProblemUpdateFrequency);
    addChild(tOptions, "DisablePostSmoothing",mDisablePostSmoothing);
    addChild(tOptions, "KSTrustRegionRatioLow",mTrustRegionRatioLow);
    addChild(tOptions, "KSTrustRegionRatioMid",mTrustRegionRatioMid);
    addChild(tOptions, "KSTrustRegionRatioUpper",mTrustRegionRatioUpper);

    auto tConvergence = tOptimizer.append_child("Convergence");
    addChild(tConvergence, "MaxIterations",mMaxIterations);

}

void OptimizationAlgorithmPlatoKSBC::writeAuxiliaryFiles(pugi::xml_node& aNode)
{

}

OptimizationAlgorithmPlatoKSAL::OptimizationAlgorithmPlatoKSAL(const OptimizationParameters& aParameters)
: OptimizationAlgorithm(aParameters)
{
   mMaxNumOuterIterations= aParameters.max_iterations(); ///<<<<<<<<<
    mTrustRegionExpansionFactor = aParameters.ks_trust_region_expansion_factor();
    mTrustRegionContractionFactor = aParameters.ks_trust_region_contraction_factor();
    mMaxTrustRegionIterations = aParameters.ks_max_trust_region_iterations();

    mInitialRadiusScale = aParameters.ks_initial_radius_scale();
    mMaxRadiusScale = aParameters.ks_max_radius_scale();
    mHessianType = aParameters.hessian_type();
    mMinTrustRegionRadius = aParameters.ks_min_trust_region_radius();
    mLimitedMemoryStorage = aParameters.limited_memory_storage();
    
    mOuterGradientTolerance = aParameters.ks_outer_gradient_tolerance();
    mOuterStationarityTolerance = aParameters.ks_outer_stationarity_tolerance();
    mOuterStagnationTolerance = aParameters.ks_outer_stagnation_tolerance();
    mOuterControlStagnationTolerance = aParameters.ks_outer_control_stagnation_tolerance();
    mOuterActualReductionTolerance = aParameters.ks_outer_actual_reduction_tolerance();
    
    mProblemUpdateFrequency = aParameters.problem_update_frequency();
    mDisablePostSmoothing = aParameters.ks_disable_post_smoothing();
    mTrustRegionRatioLow = aParameters.ks_trust_region_ratio_low();
    mTrustRegionRatioMid = aParameters.ks_trust_region_ratio_mid();
    mTrustRegionRatioUpper = aParameters.ks_trust_region_ratio_high();

    mPenaltyParam = aParameters.al_penalty_parameter();
    mPenaltyParamScaleFactor = aParameters.al_penalty_scale_factor();
}
void OptimizationAlgorithmPlatoKSAL::writeInterface(pugi::xml_node& aNode)
{
    auto tOptimizer = aNode.append_child("Optimizer");
    addChild(tOptimizer, "Package","KSBC");
    
    auto tOptions = tOptimizer.append_child("Options");
    addChild(tOptions, "MaxNumOuterIterations",mMaxNumOuterIterations);
    addChild(tOptions, "KSTrustRegionExpansionFactor",mTrustRegionExpansionFactor);
    addChild(tOptions, "KSTrustRegionContractionFactor",mTrustRegionContractionFactor);
    addChild(tOptions, "KSMaxTrustRegionIterations",mMaxTrustRegionIterations);
    addChild(tOptions, "KSInitialRadiusScale",mInitialRadiusScale);
    addChild(tOptions, "KSMaxRadiusScale",mMaxRadiusScale);
    addChild(tOptions, "HessianType",mHessianType);
    addChild(tOptions, "MinTrustRegionRadius",mMinTrustRegionRadius);
    addChild(tOptions, "LimitedMemoryStorage",mLimitedMemoryStorage);
    addChild(tOptions, "KSOuterGradientTolerance",mOuterGradientTolerance);
    addChild(tOptions, "KSOuterStationarityTolerance",mOuterStationarityTolerance);
    addChild(tOptions, "KSOuterStagnationTolerance",mOuterStagnationTolerance);
    addChild(tOptions, "KSOuterControlStagnationTolerance",mOuterControlStagnationTolerance);
    addChild(tOptions, "KSOuterActualReductionTolerance",mOuterActualReductionTolerance);
    addChild(tOptions, "ProblemUpdateFrequency",mProblemUpdateFrequency);
    addChild(tOptions, "DisablePostSmoothing",mDisablePostSmoothing);
    addChild(tOptions, "KSTrustRegionRatioLow",mTrustRegionRatioLow);
    addChild(tOptions, "KSTrustRegionRatioMid",mTrustRegionRatioMid);
    addChild(tOptions, "KSTrustRegionRatioUpper",mTrustRegionRatioUpper);

    addChild(tOptions, "AugLagPenaltyParam",mPenaltyParam);
    addChild(tOptions, "AugLagPenaltyParamScaleFactor",mPenaltyParamScaleFactor);
    
    auto tConvergence = tOptimizer.append_child("Convergence");
    addChild(tConvergence, "MaxIterations",mMaxIterations);

}

void OptimizationAlgorithmPlatoKSAL::writeAuxiliaryFiles(pugi::xml_node& aNode)
{

}

OptimizationAlgorithmPlatoMMA::OptimizationAlgorithmPlatoMMA(const OptimizationParameters& aParameters)
: OptimizationAlgorithm(aParameters)
{
    mMaxNumOuterIterations = aParameters.max_iterations(); //<<<<<<<<<<<<<<<<<
    mMoveLimit = aParameters.mma_move_limit();
    mAsymptoteExpansion = aParameters.mma_asymptote_expansion();
    mAsymptoteContraction = aParameters.mma_asymptote_contraction();
    mMaxNumSubProblemIter = aParameters.mma_max_sub_problem_iterations();
    mControlStagnationTolerance = aParameters.mma_control_stagnation_tolerance();
    mObjectiveStagnationTolerance = aParameters.mma_objective_stagnation_tolerance();
    mOutputSubProblemDiagnostics = aParameters.mma_output_subproblem_diagnostics();
    mSubProblemInitialPenalty = aParameters.mma_sub_problem_initial_penalty();
    mSubProblemPenaltyMultiplier = aParameters.mma_sub_problem_penalty_multiplier();
    mSubProblemFeasibilityTolerance = aParameters.mma_sub_problem_feasibility_tolerance();
    mUpdateFrequency = aParameters.problem_update_frequency();
    mUseIpoptForMMASubproblem = aParameters.mma_use_ipopt_sub_problem_solver();
}
void OptimizationAlgorithmPlatoMMA::writeInterface(pugi::xml_node& aNode)
{
    auto tOptimizer = aNode.append_child("Optimizer");
    addChild(tOptimizer, "Package","MMA");
    
    auto tOptions = tOptimizer.append_child("Options");
    
    addChild(tOptions, "FILL",mMaxNumOuterIterations);
    
    auto tConvergence = tOptimizer.append_child("Convergence");
    addChild(tConvergence, "MaxIterations",mMaxIterations);

}

void OptimizationAlgorithmPlatoMMA::writeAuxiliaryFiles(pugi::xml_node& aNode)
{

}




//****************************ROL **********************************************//
OptimizationAlgorithmROLLC::OptimizationAlgorithmROLLC(const OptimizationParameters& aParameters)
: OptimizationAlgorithm(aParameters)
{

}
void OptimizationAlgorithmROLLC::writeInterface(pugi::xml_node& aNode)
{

}

void OptimizationAlgorithmROLLC::writeAuxiliaryFiles(pugi::xml_node& aNode)
{

}

OptimizationAlgorithmROLBC::OptimizationAlgorithmROLBC(const OptimizationParameters& aParameters)
: OptimizationAlgorithm(aParameters)
{

}
void OptimizationAlgorithmROLBC::writeInterface(pugi::xml_node& aNode)
{

}

void OptimizationAlgorithmROLBC::writeAuxiliaryFiles(pugi::xml_node& aNode)
{

}

OptimizationAlgorithmROLAL::OptimizationAlgorithmROLAL(const OptimizationParameters& aParameters)
: OptimizationAlgorithm(aParameters)
{

}
void OptimizationAlgorithmROLAL::writeInterface(pugi::xml_node& aNode)
{

}

void OptimizationAlgorithmROLAL::writeAuxiliaryFiles(pugi::xml_node& aNode)
{

}

//****************************Dakota **********************************************//
OptimizationAlgorithmDakota::OptimizationAlgorithmDakota(const OptimizationParameters& aParameters)
: OptimizationAlgorithm(aParameters)
{

}
void OptimizationAlgorithmDakota::writeInterface(pugi::xml_node& aNode)
{

}

void OptimizationAlgorithmDakota::writeAuxiliaryFiles(pugi::xml_node& aNode)
{

}

}//namespace
