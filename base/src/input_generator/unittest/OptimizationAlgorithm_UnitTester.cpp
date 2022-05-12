//OptimizationAlgorithm_UnitTester.cpp

#include "OptimizationAlgorithm.hpp"
#include "OptimizationAlgorithmFactory.hpp"

#include <gtest/gtest.h>

#include "pugixml.hpp"
#include "XMLGenerator_UnitTester_Tools.hpp"
#include "FileObject.hpp"
#include "XMLGeneratorUtilities.hpp"
#include "SharedData.hpp"
#include "Performer.hpp"
#include "Stage.hpp"

namespace PlatoTestXMLGenerator
{

TEST(PlatoTestXMLGenerator, OptimizationAlgorithmFactoryGeneratePlatoOC_CheckOptions_AllIgnore)
{
    XMLGen::InputData tMetaData;
    XMLGen::OptimizationParameters tOptimizationParameters;
    
    tOptimizationParameters.append("optimization_algorithm" ,"oc");
    
    tMetaData.set(tOptimizationParameters);
   
    director::OptimizationAlgorithmFactory tFactory;
    ASSERT_NO_THROW(tFactory.create(tMetaData));

    std::shared_ptr<director::OptimizationAlgorithm> tAlgo = tFactory.create(tMetaData);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAlgo->writeInterface(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOptimizer = tDocument.child("Optimizer");
    ASSERT_FALSE(tOptimizer.empty());

    auto tOptions = tOptimizer.child("Options");
    ASSERT_FALSE(tOptions.empty());
    std::vector<std::string> tKeys = {
        "OCControlStagnationTolerance",
        "OCObjectiveStagnationTolerance",
        "OCGradientTolerance",
        "ProblemUpdateFrequency"};
    std::vector<std::string> tValues = {
        "IGNORE",
        "IGNORE",
        "IGNORE",
        "IGNORE"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOptions);

    auto tConvergence = tOptimizer.child("Convergence");
    ASSERT_FALSE(tConvergence.empty());
    tKeys = {
        "MaxIterations"};
    tValues = {
        "IGNORE"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tConvergence);

    tConvergence = tConvergence.next_sibling("Convergence");
    ASSERT_TRUE(tConvergence.empty());

    tOptions = tOptions.next_sibling("Options");
    ASSERT_TRUE(tOptions.empty());

    tOptimizer = tOptimizer.next_sibling("Optimizer");
    ASSERT_TRUE(tOptimizer.empty());
}

TEST(PlatoTestXMLGenerator, OptimizationAlgorithmFactoryGeneratePlatoOC_CheckOptions)
{
    XMLGen::InputData tMetaData;
    XMLGen::OptimizationParameters tOptimizationParameters;
    
    tOptimizationParameters.append("optimization_algorithm" ,"oc");
    tOptimizationParameters.append("oc_control_stagnation_tolerance" ,"1e-2");
    tOptimizationParameters.append("oc_objective_stagnation_tolerance" ,"1e-5");
    tOptimizationParameters.append("oc_gradient_tolerance" ,"1e-8");
    tOptimizationParameters.append("problem_update_frequency" ,"5");
    tOptimizationParameters.append("max_iterations" ,"10");
    
    tMetaData.set(tOptimizationParameters);
   
    director::OptimizationAlgorithmFactory tFactory;
    ASSERT_NO_THROW(tFactory.create(tMetaData));

    std::shared_ptr<director::OptimizationAlgorithm> tAlgo = tFactory.create(tMetaData);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAlgo->writeInterface(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOptimizer = tDocument.child("Optimizer");
    ASSERT_FALSE(tOptimizer.empty());

    auto tOptions = tOptimizer.child("Options");
    ASSERT_FALSE(tOptions.empty());
    std::vector<std::string> tKeys = {
        "OCControlStagnationTolerance",
        "OCObjectiveStagnationTolerance",
        "OCGradientTolerance",
        "ProblemUpdateFrequency"};
    std::vector<std::string> tValues = {
        "1e-2",
        "1e-5",
        "1e-8",
        "5"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOptions);

    auto tConvergence = tOptimizer.child("Convergence");
    ASSERT_FALSE(tConvergence.empty());
    tKeys = {
        "MaxIterations"};
    tValues = {
        "10"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tConvergence);

    tConvergence = tConvergence.next_sibling("Convergence");

    tOptions = tOptions.next_sibling("Options");
    ASSERT_TRUE(tOptions.empty());

    tOptimizer = tOptimizer.next_sibling("Optimizer");
    ASSERT_TRUE(tOptimizer.empty());
}

TEST(PlatoTestXMLGenerator, OptimizationAlgorithmFactoryGeneratePlatoKSBC_CheckOptions)
{
    XMLGen::InputData tMetaData;
    XMLGen::OptimizationParameters tOptimizationParameters;
    
    tOptimizationParameters.append("optimization_algorithm" ,"ksbc");
    
    tOptimizationParameters.append("max_iterations" ,"10");
    tOptimizationParameters.append("ks_trust_region_expansion_factor" ,"4");
    tOptimizationParameters.append("ks_trust_region_contraction_factor" ,".5");
    tOptimizationParameters.append("ks_max_trust_region_iterations" ,"5");

    tOptimizationParameters.append("ks_initial_radius_scale" ,".25");
    tOptimizationParameters.append("ks_max_radius_scale" ,".9");
    tOptimizationParameters.append("hessian_type" ,"zero");
    tOptimizationParameters.append("ks_min_trust_region_radius" ,"1e-12");
    tOptimizationParameters.append("limited_memory_storage" ,"8");
    
    tOptimizationParameters.append("ks_outer_gradient_tolerance" ,"1e-11");
    tOptimizationParameters.append("ks_outer_stationarity_tolerance" ,"1e-11");
    tOptimizationParameters.append("ks_outer_stagnation_tolerance" ,"1e-11");
    tOptimizationParameters.append("ks_outer_control_stagnation_tolerance" ,"1e-11");
    tOptimizationParameters.append("ks_outer_actual_reduction_tolerance" ,"1e-11");
    
    tOptimizationParameters.append("problem_update_frequency" ,"5");
    tOptimizationParameters.append("ks_disable_post_smoothing" ,"true");
    tOptimizationParameters.append("ks_trust_region_ratio_low" ,".05");
    tOptimizationParameters.append("ks_trust_region_ratio_mid" ,".2");
    tOptimizationParameters.append("ks_trust_region_ratio_high" ,".45");
    

    tMetaData.set(tOptimizationParameters);
   
    director::OptimizationAlgorithmFactory tFactory;
    ASSERT_NO_THROW(tFactory.create(tMetaData));

    std::shared_ptr<director::OptimizationAlgorithm> tAlgo = tFactory.create(tMetaData);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAlgo->writeInterface(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOptimizer = tDocument.child("Optimizer");
    ASSERT_FALSE(tOptimizer.empty());

    auto tOptions = tOptimizer.child("Options");
    ASSERT_FALSE(tOptions.empty());
    
    std::vector<std::string> tKeys = {
        "MaxNumOuterIterations",
        "KSTrustRegionExpansionFactor",
        "KSTrustRegionContractionFactor",
        "KSMaxTrustRegionIterations",
        "KSInitialRadiusScale",
        "KSMaxRadiusScale",
        "HessianType",
        "MinTrustRegionRadius",
        "LimitedMemoryStorage",
        "KSOuterGradientTolerance",
        "KSOuterStationarityTolerance",
        "KSOuterStagnationTolerance",
        "KSOuterControlStagnationTolerance",
        "KSOuterActualReductionTolerance",
        "ProblemUpdateFrequency",
        "DisablePostSmoothing",
        "KSTrustRegionRatioLow",
        "KSTrustRegionRatioMid",
        "KSTrustRegionRatioUpper"
        };

    std::vector<std::string> tValues = {
        "10",
        "4",
        ".5",
        "5",
        ".25",
        ".9",
        "zero",
        "1e-12",
        "8",
        "1e-11",
        "1e-11",
        "1e-11",
        "1e-11",
        "1e-11",
        "5",
        "true",
        ".05",
        ".2",
        ".45"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOptions);

    auto tConvergence = tOptimizer.child("Convergence");
    ASSERT_FALSE(tConvergence.empty());
    tKeys = {
        "MaxIterations"};
    tValues = {
        "10"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tConvergence);

    tConvergence = tConvergence.next_sibling("Convergence");

    tOptions = tOptions.next_sibling("Options");
    ASSERT_TRUE(tOptions.empty());

    tOptimizer = tOptimizer.next_sibling("Optimizer");
    ASSERT_TRUE(tOptimizer.empty());
}

TEST(PlatoTestXMLGenerator, OptimizationAlgorithmFactoryGeneratePlatoKSAL_CheckOptions)
{
    XMLGen::InputData tMetaData;
    XMLGen::OptimizationParameters tOptimizationParameters;
    
    tOptimizationParameters.append("optimization_algorithm" ,"ksal");
    
    tOptimizationParameters.append("max_iterations" ,"10");
    tOptimizationParameters.append("ks_trust_region_expansion_factor" ,"4");
    tOptimizationParameters.append("ks_trust_region_contraction_factor" ,".5");
    tOptimizationParameters.append("ks_max_trust_region_iterations" ,"5");

    tOptimizationParameters.append("ks_initial_radius_scale" ,".25");
    tOptimizationParameters.append("ks_max_radius_scale" ,".9");
    tOptimizationParameters.append("hessian_type" ,"zero");
    tOptimizationParameters.append("ks_min_trust_region_radius" ,"1e-12");
    tOptimizationParameters.append("limited_memory_storage" ,"8");
    
    tOptimizationParameters.append("ks_outer_gradient_tolerance" ,"1e-11");
    tOptimizationParameters.append("ks_outer_stationarity_tolerance" ,"1e-11");
    tOptimizationParameters.append("ks_outer_stagnation_tolerance" ,"1e-11");
    tOptimizationParameters.append("ks_outer_control_stagnation_tolerance" ,"1e-11");
    tOptimizationParameters.append("ks_outer_actual_reduction_tolerance" ,"1e-11");
    
    tOptimizationParameters.append("problem_update_frequency" ,"5");
    tOptimizationParameters.append("ks_disable_post_smoothing" ,"true");
    tOptimizationParameters.append("ks_trust_region_ratio_low" ,".05");
    tOptimizationParameters.append("ks_trust_region_ratio_mid" ,".2");
    tOptimizationParameters.append("ks_trust_region_ratio_high" ,".45");

    tOptimizationParameters.append("al_penalty_parameter" ,".2");
    tOptimizationParameters.append("al_penalty_scale_factor" ,".45");
    
    tMetaData.set(tOptimizationParameters);
   
    director::OptimizationAlgorithmFactory tFactory;
    ASSERT_NO_THROW(tFactory.create(tMetaData));

    std::shared_ptr<director::OptimizationAlgorithm> tAlgo = tFactory.create(tMetaData);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAlgo->writeInterface(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOptimizer = tDocument.child("Optimizer");
    ASSERT_FALSE(tOptimizer.empty());

    auto tOptions = tOptimizer.child("Options");
    ASSERT_FALSE(tOptions.empty());
    
    std::vector<std::string> tKeys = {
        "MaxNumOuterIterations",
        "KSTrustRegionExpansionFactor",
        "KSTrustRegionContractionFactor",
        "KSMaxTrustRegionIterations",
        "KSInitialRadiusScale",
        "KSMaxRadiusScale",
        "HessianType",
        "MinTrustRegionRadius",
        "LimitedMemoryStorage",
        "KSOuterGradientTolerance",
        "KSOuterStationarityTolerance",
        "KSOuterStagnationTolerance",
        "KSOuterControlStagnationTolerance",
        "KSOuterActualReductionTolerance",
        "ProblemUpdateFrequency",
        "DisablePostSmoothing",
        "KSTrustRegionRatioLow",
        "KSTrustRegionRatioMid",
        "KSTrustRegionRatioUpper",
        "AugLagPenaltyParam",
        "AugLagPenaltyParamScaleFactor"
        };

    std::vector<std::string> tValues = {
        "10",
        "4",
        ".5",
        "5",
        ".25",
        ".9",
        "zero",
        "1e-12",
        "8",
        "1e-11",
        "1e-11",
        "1e-11",
        "1e-11",
        "1e-11",
        "5",
        "true",
        ".05",
        ".2",
        ".45",
        "1",
        "1.05"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOptions);

    auto tConvergence = tOptimizer.child("Convergence");
    ASSERT_FALSE(tConvergence.empty());
    tKeys = {
        "MaxIterations"};
    tValues = {
        "10"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tConvergence);

    tConvergence = tConvergence.next_sibling("Convergence");

    tOptions = tOptions.next_sibling("Options");
    ASSERT_TRUE(tOptions.empty());

    tOptimizer = tOptimizer.next_sibling("Optimizer");
    ASSERT_TRUE(tOptimizer.empty());
}

TEST(PlatoTestXMLGenerator, OptimizationAlgorithmFactoryGeneratePlatoMMA_CheckOptions)
{
    XMLGen::InputData tMetaData;
    XMLGen::OptimizationParameters tOptimizationParameters;
    
    tOptimizationParameters.append("optimization_algorithm" ,"mma");
    tOptimizationParameters.append("max_iterations" ,"10");
    tOptimizationParameters.append("mma_move_limit" ,"4");
    tOptimizationParameters.append("mma_asymptote_expansion" ,".5");
    tOptimizationParameters.append("mma_asymptote_contraction" ,"5");
    tOptimizationParameters.append("mma_max_sub_problem_iterations" ,".25");
    tOptimizationParameters.append("mma_sub_problem_initial_penalty" ,".9");
    tOptimizationParameters.append("mma_sub_problem_penalty_multiplier" ,"1");
    tOptimizationParameters.append("mma_sub_problem_feasibility_tolerance" ,"1e-12");
    tOptimizationParameters.append("mma_use_ipopt_sub_problem_solver" ,"true");
    tOptimizationParameters.append("mma_control_stagnation_tolerance" ,"1e-11");
    tOptimizationParameters.append("mma_objective_stagnation_tolerance" ,"1e-11");
    tOptimizationParameters.append("problem_update_frequency" ,"55");
    tOptimizationParameters.append("mma_output_subproblem_diagnostics" ,"1e-11");
    
    tMetaData.set(tOptimizationParameters);
   
    director::OptimizationAlgorithmFactory tFactory;
    ASSERT_NO_THROW(tFactory.create(tMetaData));

    std::shared_ptr<director::OptimizationAlgorithm> tAlgo = tFactory.create(tMetaData);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAlgo->writeInterface(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOptimizer = tDocument.child("Optimizer");
    ASSERT_FALSE(tOptimizer.empty());

    auto tOptions = tOptimizer.child("Options");
    ASSERT_FALSE(tOptions.empty());
    
    std::vector<std::string> tKeys = {
        "MaxNumOuterIterations",
        "MoveLimit",
        "AsymptoteExpansion",
        "AsymptoteContraction",
        "MaxNumSubProblemIter",
        "ControlStagnationTolerance",
        "ObjectiveStagnationTolerance",
        "OutputSubProblemDiagnostics",
        "SubProblemInitialPenalty",
        "SubProblemPenaltyMultiplier",
        "SubProblemFeasibilityTolerance",
        "UpdateFrequency",
        "UseIpoptForMMASubproblem"
        };

   std::vector<std::string> tValues = {
        "10",
        "4",
        ".5",
        "5",
        ".25",
        "1e-11",
        "1e-11",
        "1e-11",
        ".9",
        "1",
        "1e-12",
        "55",
        "true"};
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOptions);

    auto tConvergence = tOptimizer.child("Convergence");
    ASSERT_FALSE(tConvergence.empty());
    tKeys = {
        "MaxIterations"};
    tValues = {
        "10"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tConvergence);

    tConvergence = tConvergence.next_sibling("Convergence");

    tOptions = tOptions.next_sibling("Options");
    ASSERT_TRUE(tOptions.empty());

    tOptimizer = tOptimizer.next_sibling("Optimizer");
    ASSERT_TRUE(tOptimizer.empty());
}

TEST(PlatoTestXMLGenerator, OptimizationAlgorithmFactoryGeneratePlatoOC_CheckOutput)
{
    XMLGen::InputData tMetaData;
    XMLGen::OptimizationParameters tOptimizationParameters;
    
    tOptimizationParameters.append("optimization_algorithm" ,"oc");
    
    tMetaData.set(tOptimizationParameters);
   
    director::OptimizationAlgorithmFactory tFactory;
    ASSERT_NO_THROW(tFactory.create(tMetaData));

    std::shared_ptr<director::OptimizationAlgorithm> tAlgo = tFactory.create(tMetaData);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAlgo->writeInterface(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOptimizer = tDocument.child("Optimizer");
    ASSERT_FALSE(tOptimizer.empty());

    auto tOutput = tOptimizer.child("Output");
    ASSERT_FALSE(tOutput.empty());
    std::vector<std::string> tKeys = {
        "OutputStage"};
    std::vector<std::string>  tValues = {
        "Output To File"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOutput);

    tOutput = tOutput.next_sibling("Output");
    ASSERT_TRUE(tOutput.empty());

    tOptimizer = tOptimizer.next_sibling("Optimizer");
    ASSERT_TRUE(tOptimizer.empty());
}

TEST(PlatoTestXMLGenerator, OptimizationAlgorithmFactoryGeneratePlatoOC_CheckOptimizationVariables)
{
    // CREATE ALGORITHM
    XMLGen::InputData tMetaData;
    XMLGen::OptimizationParameters tOptimizationParameters;
    tOptimizationParameters.append("optimization_algorithm" ,"oc");
    tMetaData.set(tOptimizationParameters);
   
    director::OptimizationAlgorithmFactory tFactory;
    std::shared_ptr<director::OptimizationAlgorithm> tAlgo = tFactory.create(tMetaData);

    // CREATE STAGES
    std::shared_ptr<director::Performer> tPerformerMain = std::make_shared<director::Performer>("platomain_1","platomain");
    std::vector<std::shared_ptr<director::Performer>> tUserPerformers = {tPerformerMain};

    std::vector<std::shared_ptr<director::Operation>> tNullOperations = {nullptr};

    std::shared_ptr<director::SharedData> tControlSharedData = std::make_shared<director::SharedDataNodalField>("Control",tPerformerMain,tUserPerformers);
    std::shared_ptr<director::Stage> tInitialGuessStage = std::make_shared<director::Stage>("Initial Guess",tNullOperations,nullptr,tControlSharedData);

    std::shared_ptr<director::SharedData> tLowerBoundValueSharedData = std::make_shared<director::SharedDataGlobal>("Lower Bound Value","1",tPerformerMain,tUserPerformers);
    std::shared_ptr<director::SharedData> tLowerBoundVectorSharedData = std::make_shared<director::SharedDataNodalField>("Lower Bound Vector",tPerformerMain,tUserPerformers);
    std::shared_ptr<director::Stage> tLowerBoundStage = std::make_shared<director::Stage>("Set Lower Bounds",tNullOperations,tLowerBoundValueSharedData,tLowerBoundVectorSharedData);

    std::shared_ptr<director::SharedData> tUpperBoundValueSharedData = std::make_shared<director::SharedDataGlobal>("Upper Bound Value","1",tPerformerMain,tUserPerformers);
    std::shared_ptr<director::SharedData> tUpperBoundVectorSharedData = std::make_shared<director::SharedDataNodalField>("Upper Bound Vector",tPerformerMain,tUserPerformers);
    std::shared_ptr<director::Stage> tUpperBoundStage = std::make_shared<director::Stage>("Set Upper Bounds",tNullOperations,tUpperBoundValueSharedData,tUpperBoundVectorSharedData);

    // WRITE INTERFACE
    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAlgo->writeInterface(tDocument,tInitialGuessStage,tUpperBoundStage,tLowerBoundStage));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOptimizer = tDocument.child("Optimizer");
    ASSERT_FALSE(tOptimizer.empty());

    auto tOV = tOptimizer.child("OptimizationVariables");
    ASSERT_FALSE(tOV.empty());
    std::vector<std::string> tKeys = {
        "ValueName",
        "InitializationStage",
        "LowerBoundValueName",
        "LowerBoundVectorName",
        "SetLowerBoundsStage",
        "UpperBoundValueName",
        "UpperBoundVectorName",
        "SetUpperBoundsStage"
        };
    std::vector<std::string>  tValues = {
        "Control",
        "Initial Guess",
        "Lower Bound Value",
        "Lower Bound Vector",
        "Set Lower Bounds",
        "Upper Bound Value",
        "Upper Bound Vector",
        "Set Upper Bounds"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOV);

    tOV = tOV.next_sibling("OptimizationVariables");
    ASSERT_TRUE(tOV.empty());

    tOptimizer = tOptimizer.next_sibling("Optimizer");
    ASSERT_TRUE(tOptimizer.empty());
}

TEST(PlatoTestXMLGenerator, OptimizationAlgorithmFactoryGeneratePlatoOC_CheckObjective)
{
    // CREATE ALGORITHM
    XMLGen::InputData tMetaData;
    XMLGen::OptimizationParameters tOptimizationParameters;
    tOptimizationParameters.append("optimization_algorithm" ,"oc");
    tMetaData.set(tOptimizationParameters);
   
    director::OptimizationAlgorithmFactory tFactory;
    std::shared_ptr<director::OptimizationAlgorithm> tAlgo = tFactory.create(tMetaData);

    // CREATE STAGES
    std::shared_ptr<director::Performer> tPerformerMain = std::make_shared<director::Performer>("platomain_1","platomain");
    std::vector<std::shared_ptr<director::Performer>> tUserPerformers = {tPerformerMain};

    std::vector<std::shared_ptr<director::Operation>> tNullOperations = {nullptr};

    std::shared_ptr<director::SharedData> tObjectiveValueSharedData = std::make_shared<director::SharedDataGlobal>("Objective Value","1",tPerformerMain,tUserPerformers);
    std::shared_ptr<director::Stage> tObjectiveValueStage = std::make_shared<director::Stage>("Compute Objective Value",tNullOperations,nullptr,tObjectiveValueSharedData);

    std::shared_ptr<director::SharedData> tObjectiveGradientSharedData = std::make_shared<director::SharedDataNodalField>("Objective Gradient",tPerformerMain,tUserPerformers);
    std::shared_ptr<director::Stage> tObjectiveGradientStage = std::make_shared<director::Stage>("Compute Objective Gradient",tNullOperations,nullptr,tObjectiveGradientSharedData);

    // WRITE INTERFACE
    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAlgo->writeInterface(tDocument,nullptr,nullptr,nullptr,tObjectiveValueStage,tObjectiveGradientStage));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOptimizer = tDocument.child("Optimizer");
    ASSERT_FALSE(tOptimizer.empty());

    auto tObjective = tOptimizer.child("Objective");
    ASSERT_FALSE(tObjective.empty());
    std::vector<std::string> tKeys = {
        "ValueStageName",
        "ValueName",
        "GradientStageName",
        "GradientName"
        };
    std::vector<std::string>  tValues = {
        "Compute Objective Value",
        "Objective Value",
        "Compute Objective Gradient",
        "Objective Gradient"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tObjective);

    tObjective = tObjective.next_sibling("Objective");
    ASSERT_TRUE(tObjective.empty());

    tOptimizer = tOptimizer.next_sibling("Optimizer");
    ASSERT_TRUE(tOptimizer.empty());
}

TEST(PlatoTestXMLGenerator, OptimizationAlgorithmFactoryGeneratePlatoOC_CheckSingleConstraint)
{
    // CREATE ALGORITHM
    XMLGen::InputData tMetaData;
    XMLGen::OptimizationParameters tOptimizationParameters;
    tOptimizationParameters.append("optimization_algorithm" ,"oc");
    tMetaData.set(tOptimizationParameters);
   
    director::OptimizationAlgorithmFactory tFactory;
    std::shared_ptr<director::OptimizationAlgorithm> tAlgo = tFactory.create(tMetaData);

    // CREATE STAGES
    std::shared_ptr<director::Performer> tPerformerMain = std::make_shared<director::Performer>("platomain_1","platomain");
    std::vector<std::shared_ptr<director::Performer>> tUserPerformers = {tPerformerMain};

    std::vector<std::shared_ptr<director::Operation>> tNullOperations = {nullptr};

    std::shared_ptr<director::SharedData> tConstraintValueSharedData = std::make_shared<director::SharedDataGlobal>("Constraint Value 1","1",tPerformerMain,tUserPerformers);
    std::shared_ptr<director::Stage> tConstraintValueStage = std::make_shared<director::Stage>("Compute Constraint Value 1",tNullOperations,nullptr,tConstraintValueSharedData);

    std::shared_ptr<director::SharedData> tConstraintGradientSharedData = std::make_shared<director::SharedDataNodalField>("Constraint Gradient 1",tPerformerMain,tUserPerformers);
    std::shared_ptr<director::Stage> tConstraintGradientStage = std::make_shared<director::Stage>("Compute Constraint Gradient 1",tNullOperations,nullptr,tConstraintGradientSharedData);

    // WRITE INTERFACE
    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAlgo->writeInterface(tDocument,nullptr,nullptr,nullptr,nullptr,nullptr,tConstraintValueStage,tConstraintGradientStage));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOptimizer = tDocument.child("Optimizer");
    ASSERT_FALSE(tOptimizer.empty());

    auto tConstraint = tOptimizer.child("Constraint");
    ASSERT_FALSE(tConstraint.empty());
    std::vector<std::string> tKeys = {
        "ValueStageName",
        "ValueName",
        "GradientStageName",
        "GradientName",
        "NormalizedTargetValue",
        "AbsoluteTargetValue",
        "ReferenceValueName",
        "ReferenceValue"
        };
    std::vector<std::string>  tValues = {
        "Compute Constraint Value 1",
        "Constraint Value 1",
        "Compute Constraint Gradient 1",
        "Constraint Gradient 1",
        "NEED TO ADD",
        "NEED TO ADD",
        "NEED TO ADD",
        "NEED TO ADD"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tConstraint);

    tConstraint = tConstraint.next_sibling("Constraint");
    ASSERT_TRUE(tConstraint.empty());

    tOptimizer = tOptimizer.next_sibling("Optimizer");
    ASSERT_TRUE(tOptimizer.empty());
}

TEST(PlatoTestXMLGenerator, OptimizationAlgorithmFactoryGeneratePlatoOC_CheckBoundConstraint)
{
    // CREATE ALGORITHM
    XMLGen::InputData tMetaData;
    XMLGen::OptimizationParameters tOptimizationParameters;
    tOptimizationParameters.append("optimization_algorithm" ,"oc");
    tMetaData.set(tOptimizationParameters);
   
    director::OptimizationAlgorithmFactory tFactory;
    std::shared_ptr<director::OptimizationAlgorithm> tAlgo = tFactory.create(tMetaData);

    // WRITE INTERFACE
    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAlgo->writeInterface(tDocument));
    ASSERT_FALSE(tDocument.empty());

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOptimizer = tDocument.child("Optimizer");
    ASSERT_FALSE(tOptimizer.empty());

    auto tBounds = tOptimizer.child("BoundConstraint");
    ASSERT_FALSE(tBounds.empty());
    std::vector<std::string> tKeys = {
        "Upper",
        "Lower"
        };
    std::vector<std::string>  tValues = {
        "1.0",
        "0.0"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tBounds);

    tBounds = tBounds.next_sibling("BoundConstraint");
    ASSERT_TRUE(tBounds.empty());

    tOptimizer = tOptimizer.next_sibling("Optimizer");
    ASSERT_TRUE(tOptimizer.empty());
}

}
