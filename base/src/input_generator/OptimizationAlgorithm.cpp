#include "OptimizationAlgorithm.hpp"
#include "pugixml.hpp"
#include "XMLGeneratorUtilities.hpp"

namespace XMLGen
{

OptimizationAlgorithm::OptimizationAlgorithm(const OptimizationParameters& aParameters)
{
    mMaxIterations = aParameters.max_iterations();
}

void OptimizationAlgorithm::appendOutputStage(pugi::xml_node& aNode)
{
    auto tOutput = aNode.append_child("Output");
    addChild(tOutput,"OutputStage","Output To File");
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
    
    auto tOptions = tOptimizer.append_child("Options");    
    addChildCheckEmpty(tOptions, "OCControlStagnationTolerance",mControlStagnationTolerance);
    addChildCheckEmpty(tOptions, "OCObjectiveStagnationTolerance",mObjectiveStagnationTolerance);
    addChildCheckEmpty(tOptions, "OCGradientTolerance",mGradientTolerance);
    addChildCheckEmpty(tOptions, "ProblemUpdateFrequency",mProblemUpdateFrequency);

    auto tConvergence = tOptimizer.append_child("Convergence");
    addChildCheckEmpty(tConvergence, "MaxIterations",mMaxIterations);

    appendOutputStage(tOptimizer);
    
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
    addChildCheckEmpty(tOptions, "MaxNumOuterIterations",mMaxNumOuterIterations);
    addChildCheckEmpty(tOptions, "KSTrustRegionExpansionFactor",mTrustRegionExpansionFactor);
    addChildCheckEmpty(tOptions, "KSTrustRegionContractionFactor",mTrustRegionContractionFactor);
    addChildCheckEmpty(tOptions, "KSMaxTrustRegionIterations",mMaxTrustRegionIterations);
    addChildCheckEmpty(tOptions, "KSInitialRadiusScale",mInitialRadiusScale);
    addChildCheckEmpty(tOptions, "KSMaxRadiusScale",mMaxRadiusScale);
    addChildCheckEmpty(tOptions, "HessianType",mHessianType);
    addChildCheckEmpty(tOptions, "MinTrustRegionRadius",mMinTrustRegionRadius);
    addChildCheckEmpty(tOptions, "LimitedMemoryStorage",mLimitedMemoryStorage);
    addChildCheckEmpty(tOptions, "KSOuterGradientTolerance",mOuterGradientTolerance);
    addChildCheckEmpty(tOptions, "KSOuterStationarityTolerance",mOuterStationarityTolerance);
    addChildCheckEmpty(tOptions, "KSOuterStagnationTolerance",mOuterStagnationTolerance);
    addChildCheckEmpty(tOptions, "KSOuterControlStagnationTolerance",mOuterControlStagnationTolerance);
    addChildCheckEmpty(tOptions, "KSOuterActualReductionTolerance",mOuterActualReductionTolerance);
    addChildCheckEmpty(tOptions, "ProblemUpdateFrequency",mProblemUpdateFrequency);
    addChildCheckEmpty(tOptions, "DisablePostSmoothing",mDisablePostSmoothing);
    addChildCheckEmpty(tOptions, "KSTrustRegionRatioLow",mTrustRegionRatioLow);
    addChildCheckEmpty(tOptions, "KSTrustRegionRatioMid",mTrustRegionRatioMid);
    addChildCheckEmpty(tOptions, "KSTrustRegionRatioUpper",mTrustRegionRatioUpper);

    auto tConvergence = tOptimizer.append_child("Convergence");
    addChildCheckEmpty(tConvergence, "MaxIterations",mMaxIterations);

    appendOutputStage(tOptimizer);
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
    addChild(tOptimizer, "Package","KSAL");
    
    auto tOptions = tOptimizer.append_child("Options");
    addChildCheckEmpty(tOptions, "MaxNumOuterIterations",mMaxNumOuterIterations);
    addChildCheckEmpty(tOptions, "KSTrustRegionExpansionFactor",mTrustRegionExpansionFactor);
    addChildCheckEmpty(tOptions, "KSTrustRegionContractionFactor",mTrustRegionContractionFactor);
    addChildCheckEmpty(tOptions, "KSMaxTrustRegionIterations",mMaxTrustRegionIterations);
    addChildCheckEmpty(tOptions, "KSInitialRadiusScale",mInitialRadiusScale);
    addChildCheckEmpty(tOptions, "KSMaxRadiusScale",mMaxRadiusScale);
    addChildCheckEmpty(tOptions, "HessianType",mHessianType);
    addChildCheckEmpty(tOptions, "MinTrustRegionRadius",mMinTrustRegionRadius);
    addChildCheckEmpty(tOptions, "LimitedMemoryStorage",mLimitedMemoryStorage);
    addChildCheckEmpty(tOptions, "KSOuterGradientTolerance",mOuterGradientTolerance);
    addChildCheckEmpty(tOptions, "KSOuterStationarityTolerance",mOuterStationarityTolerance);
    addChildCheckEmpty(tOptions, "KSOuterStagnationTolerance",mOuterStagnationTolerance);
    addChildCheckEmpty(tOptions, "KSOuterControlStagnationTolerance",mOuterControlStagnationTolerance);
    addChildCheckEmpty(tOptions, "KSOuterActualReductionTolerance",mOuterActualReductionTolerance);
    addChildCheckEmpty(tOptions, "ProblemUpdateFrequency",mProblemUpdateFrequency);
    addChildCheckEmpty(tOptions, "DisablePostSmoothing",mDisablePostSmoothing);
    addChildCheckEmpty(tOptions, "KSTrustRegionRatioLow",mTrustRegionRatioLow);
    addChildCheckEmpty(tOptions, "KSTrustRegionRatioMid",mTrustRegionRatioMid);
    addChildCheckEmpty(tOptions, "KSTrustRegionRatioUpper",mTrustRegionRatioUpper);

    addChildCheckEmpty(tOptions, "AugLagPenaltyParam",mPenaltyParam);
    addChildCheckEmpty(tOptions, "AugLagPenaltyParamScaleFactor",mPenaltyParamScaleFactor);
    
    auto tConvergence = tOptimizer.append_child("Convergence");
    addChildCheckEmpty(tConvergence, "MaxIterations",mMaxIterations);

    appendOutputStage(tOptimizer);
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
    addChildCheckEmpty(tOptions, "MaxNumOuterIterations",mMaxNumOuterIterations);
    addChildCheckEmpty(tOptions, "MoveLimit",mMoveLimit);
    addChildCheckEmpty(tOptions, "AsymptoteExpansion",mAsymptoteExpansion);
    addChildCheckEmpty(tOptions, "AsymptoteContraction",mAsymptoteContraction);
    addChildCheckEmpty(tOptions, "MaxNumSubProblemIter",mMaxNumSubProblemIter);
    addChildCheckEmpty(tOptions, "ControlStagnationTolerance",mControlStagnationTolerance);
    addChildCheckEmpty(tOptions, "ObjectiveStagnationTolerance",mObjectiveStagnationTolerance);
    addChildCheckEmpty(tOptions, "OutputSubProblemDiagnostics",mOutputSubProblemDiagnostics);
    addChildCheckEmpty(tOptions, "SubProblemInitialPenalty",mSubProblemInitialPenalty);
    addChildCheckEmpty(tOptions, "SubProblemPenaltyMultiplier",mSubProblemPenaltyMultiplier);
    addChildCheckEmpty(tOptions, "SubProblemFeasibilityTolerance",mSubProblemFeasibilityTolerance);
    addChildCheckEmpty(tOptions, "UpdateFrequency",mUpdateFrequency);
    addChildCheckEmpty(tOptions, "UseIpoptForMMASubproblem",mUseIpoptForMMASubproblem);

    auto tConvergence = tOptimizer.append_child("Convergence");
    addChildCheckEmpty(tConvergence, "MaxIterations",mMaxIterations);

    appendOutputStage(tOptimizer);
}

void OptimizationAlgorithmPlatoMMA::writeAuxiliaryFiles(pugi::xml_node& aNode)
{

}

/*OC
 std::vector<std::string> tKeys =
        {"ValueName", "InitializationStage", "FilteredName", "LowerBoundValueName", "LowerBoundVectorName",
         "UpperBoundValueName", "UpperBoundVectorName", "SetLowerBoundsStage", "SetUpperBoundsStage"};
    std::vector<std::string> tValues;
    if(aMetaData.optimization_parameters().optimizationType() == OT_TOPOLOGY)
    {
        tValues =
            {"Control", "Initial Guess", "Topology", "Lower Bound Value", "Lower Bound Vector",
             "Upper Bound Value", "Upper Bound Vector", "Set Lower Bounds", "Set Upper Bounds"};
    }
    else if(aMetaData.optimization_parameters().optimizationType() == OT_SHAPE)
    {
        tValues =
            {"Design Parameters", "Initialize Design Parameters", "Topology", "Lower Bound Value", "Lower Bound Vector",
             "Upper Bound Value", "Upper Bound Vector", "Set Lower Bounds", "Set Upper Bounds"};
    }
  <OptimizationVariables>
    <ValueName>Control</ValueName>
    <InitializationStage>Initial Guess</InitializationStage>
    <FilteredName>Topology</FilteredName>
    <LowerBoundValueName>Lower Bound Value</LowerBoundValueName>
    <LowerBoundVectorName>Lower Bound Vector</LowerBoundVectorName>
    <UpperBoundValueName>Upper Bound Value</UpperBoundValueName>
    <UpperBoundVectorName>Upper Bound Vector</UpperBoundVectorName>
    <SetLowerBoundsStage>Set Lower Bounds</SetLowerBoundsStage>
    <SetUpperBoundsStage>Set Upper Bounds</SetUpperBoundsStage>
  </OptimizationVariables>
  <Objective>
    <GradientStageName>Compute Objective Gradient</GradientStageName>
    <ValueStageName>Compute Objective Value</ValueStageName>
    <GradientName>Objective Gradient</GradientName>
    <ValueName>Objective Value</ValueName>
  </Objective>
  <Constraint>
    <GradientName>Constraint Gradient 1</GradientName>
    <NormalizedTargetValue>0.3</NormalizedTargetValue>
    <ValueStageName>Compute Constraint Value 1</ValueStageName>
    <ReferenceValueName>Design Volume</ReferenceValueName>
    <GradientStageName>Compute Constraint Gradient 1</GradientStageName>
    <ValueName>Constraint Value 1</ValueName>
  </Constraint>
  <BoundConstraint>
    <Upper>1.0</Upper>
    <Lower>0.0</Lower>
  </BoundConstraint>
</Optimizer>



*/



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
