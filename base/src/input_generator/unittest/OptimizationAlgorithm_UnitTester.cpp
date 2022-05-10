//OptimizationAlgorithm_UnitTester.cpp

#include "OptimizationAlgorithm.hpp"
#include "OptimizationAlgorithmFactory.hpp"

#include <gtest/gtest.h>

#include "pugixml.hpp"
#include "XMLGenerator_UnitTester_Tools.hpp"
#include "XMLGeneratorFileObject.hpp"
#include "XMLGeneratorUtilities.hpp"
#include "XMLGeneratorSharedData.hpp"
#include "XMLGeneratorPerformer.hpp"

namespace PlatoTestXMLGenerator
{

TEST(PlatoTestXMLGenerator, OptimizationAlgorithmFactoryGeneratePlatoOCCheckOutput)
{
    XMLGen::InputData tMetaData;
    XMLGen::OptimizationParameters tOptimizationParameters;
    
    tOptimizationParameters.append("optimization_algorithm" ,"oc");
    
    tMetaData.set(tOptimizationParameters);
   
    XMLGen::OptimizationAlgorithmFactory tFactory;
    ASSERT_NO_THROW(tFactory.create(tMetaData));

    std::shared_ptr<XMLGen::OptimizationAlgorithm> tAlgo = tFactory.create(tMetaData);

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

TEST(PlatoTestXMLGenerator, OptimizationAlgorithmFactoryGeneratePlatoOCCheckOptimizationVariables)
{
    XMLGen::InputData tMetaData;
    XMLGen::OptimizationParameters tOptimizationParameters;
    
    tOptimizationParameters.append("optimization_algorithm" ,"oc");
    
    tMetaData.set(tOptimizationParameters);
   
    XMLGen::OptimizationAlgorithmFactory tFactory;
    ASSERT_NO_THROW(tFactory.create(tMetaData));

    std::shared_ptr<XMLGen::OptimizationAlgorithm> tAlgo = tFactory.create(tMetaData);

    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformer = std::make_shared<XMLGen::XMLGeneratorPerformer>("plato_services","plato_services",1,16,2);
    std::shared_ptr<XMLGen::XMLGeneratorPerformer> tPerformerMain = std::make_shared<XMLGen::XMLGeneratorPerformer>("platomain_1","platomain");
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorPerformer>> tUserPerformers = {tPerformerMain,tPerformer};
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData0 = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("Lower Bound Value","1",tPerformerMain,tUserPerformers);
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData1 = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("Lower Bound Vector","5",tPerformerMain,tUserPerformers);
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData2 = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("Upper Bound Value","1",tPerformerMain,tUserPerformers);
    std::shared_ptr<XMLGen::XMLGeneratorSharedData> tSharedData3 = std::make_shared<XMLGen::XMLGeneratorSharedDataGlobal>("Upper Bound Vector","5",tPerformerMain,tUserPerformers);
    
    std::vector<std::shared_ptr<XMLGen::XMLGeneratorSharedData>> tSharedData = {tSharedData0, tSharedData1, tSharedData2, tSharedData3};

    tAlgo->assignSharedData(tSharedData);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAlgo->writeInterface(tDocument));
    ASSERT_FALSE(tDocument.empty());

    tDocument.save_file("testinterface.xml","  ");
    // TEST RESULTS AGAINST GOLD VALUES
    auto tOptimizer = tDocument.child("Optimizer");
    ASSERT_FALSE(tOptimizer.empty());

    auto tOV = tOptimizer.child("OptimizationVariables");
    ASSERT_FALSE(tOV.empty());
    std::vector<std::string> tKeys = {
        "LowerBoundValueName",
        "LowerBoundVectorName",
        "UpperBoundValueName",
        "UpperBoundVectorName"
        };
    std::vector<std::string>  tValues = {
        "Lower Bound Value",
        "Lower Bound Vector",
        "Upper Bound Value",
        "Upper Bound Vector"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOV);

    tOV = tOV.next_sibling("OptimizationVariables");
    ASSERT_TRUE(tOV.empty());

    tOptimizer = tOptimizer.next_sibling("Optimizer");
    ASSERT_TRUE(tOptimizer.empty());

}


TEST(PlatoTestXMLGenerator, OptimizationAlgorithmFactoryGeneratePlatoOCInterfaceAllIgnore)
{
    XMLGen::InputData tMetaData;
    XMLGen::OptimizationParameters tOptimizationParameters;
    
    tOptimizationParameters.append("optimization_algorithm" ,"oc");
    
    tMetaData.set(tOptimizationParameters);
   
    XMLGen::OptimizationAlgorithmFactory tFactory;
    ASSERT_NO_THROW(tFactory.create(tMetaData));

    std::shared_ptr<XMLGen::OptimizationAlgorithm> tAlgo = tFactory.create(tMetaData);

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

    auto tOutput = tOptimizer.child("Output");
    ASSERT_FALSE(tOutput.empty());
    tKeys = {
        "OutputStage"};
    tValues = {
        "Output To File"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOutput);

    tOutput = tOutput.next_sibling("Output");
    ASSERT_TRUE(tOutput.empty());

    tConvergence = tConvergence.next_sibling("Convergence");
    ASSERT_TRUE(tConvergence.empty());

    tOptions = tOptions.next_sibling("Options");
    ASSERT_TRUE(tOptions.empty());

    tOptimizer = tOptimizer.next_sibling("Optimizer");
    ASSERT_TRUE(tOptimizer.empty());

}


TEST(PlatoTestXMLGenerator, OptimizationAlgorithmFactoryGeneratePlatoOCInterface)
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
   
    XMLGen::OptimizationAlgorithmFactory tFactory;
    ASSERT_NO_THROW(tFactory.create(tMetaData));

    std::shared_ptr<XMLGen::OptimizationAlgorithm> tAlgo = tFactory.create(tMetaData);

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

TEST(PlatoTestXMLGenerator, OptimizationAlgorithmFactoryGeneratePlatoKSBCInterface)
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
   
    XMLGen::OptimizationAlgorithmFactory tFactory;
    ASSERT_NO_THROW(tFactory.create(tMetaData));

    std::shared_ptr<XMLGen::OptimizationAlgorithm> tAlgo = tFactory.create(tMetaData);

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

TEST(PlatoTestXMLGenerator, OptimizationAlgorithmFactoryGeneratePlatoKSALInterface)
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
   
    XMLGen::OptimizationAlgorithmFactory tFactory;
    ASSERT_NO_THROW(tFactory.create(tMetaData));

    std::shared_ptr<XMLGen::OptimizationAlgorithm> tAlgo = tFactory.create(tMetaData);

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

TEST(PlatoTestXMLGenerator, OptimizationAlgorithmFactoryGeneratePlatoMMAInterface)
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
   
    XMLGen::OptimizationAlgorithmFactory tFactory;
    ASSERT_NO_THROW(tFactory.create(tMetaData));

    std::shared_ptr<XMLGen::OptimizationAlgorithm> tAlgo = tFactory.create(tMetaData);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAlgo->writeInterface(tDocument));
    ASSERT_FALSE(tDocument.empty());
    tDocument.save_file("testinterface.xml","  ");

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


}//namespace
