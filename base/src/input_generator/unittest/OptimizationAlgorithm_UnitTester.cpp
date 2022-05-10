//OptimizationAlgorithm_UnitTester.cpp

#include "OptimizationAlgorithm.hpp"
#include "OptimizationAlgorithmFactory.hpp"

#include <gtest/gtest.h>

#include "pugixml.hpp"
#include "XMLGenerator_UnitTester_Tools.hpp"
#include "XMLGeneratorFileObject.hpp"
#include "XMLGeneratorUtilities.hpp"

namespace PlatoTestXMLGenerator
{

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
    tDocument.save_file("testinterface.xml","");

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOptimizer = tDocument.child("Optimizer");
    ASSERT_FALSE(tOptimizer.empty());

    std::vector<std::string> tKeys = {
        "Package",
        "Options",
	    "Convergence"};
    std::vector<std::string> tValues = {
        "OC",
        "",
	    ""
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOptimizer);

    auto tOptions = tOptimizer.child("Options");
    ASSERT_FALSE(tOptions.empty());
    tKeys = {
        "OCControlStagnationTolerance",
        "OCObjectiveStagnationTolerance",
        "OCGradientTolerance",
        "ProblemUpdateFrequency"};
    tValues = {
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
    tDocument.save_file("testinterface.xml","");

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOptimizer = tDocument.child("Optimizer");
    ASSERT_FALSE(tOptimizer.empty());

    std::vector<std::string> tKeys = {
        "Package",
        "Options",
	    "Convergence"};
    std::vector<std::string> tValues = {
        "KSBC",
        "",
	    ""
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOptimizer);

    auto tOptions = tOptimizer.child("Options");
    ASSERT_FALSE(tOptions.empty());
    
    tKeys = {
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

    tValues = {
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

    tOptimizationParameters.append("al_penalty_param" ,".2");
    tOptimizationParameters.append("al_penalty_param_scale_factor" ,".45");
    
    tMetaData.set(tOptimizationParameters);
   
    XMLGen::OptimizationAlgorithmFactory tFactory;
    ASSERT_NO_THROW(tFactory.create(tMetaData));

    std::shared_ptr<XMLGen::OptimizationAlgorithm> tAlgo = tFactory.create(tMetaData);

    pugi::xml_document tDocument;
    ASSERT_NO_THROW(tAlgo->writeInterface(tDocument));
    ASSERT_FALSE(tDocument.empty());
    tDocument.save_file("testinterface.xml","");

    // TEST RESULTS AGAINST GOLD VALUES
    auto tOptimizer = tDocument.child("Optimizer");
    ASSERT_FALSE(tOptimizer.empty());

    std::vector<std::string> tKeys = {
        "Package",
        "Options",
	    "Convergence"};
    std::vector<std::string> tValues = {
        "KSBC",
        "",
	    ""
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOptimizer);

    auto tOptions = tOptimizer.child("Options");
    ASSERT_FALSE(tOptions.empty());
    
    tKeys = {
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

    tValues = {
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

}//namespace
