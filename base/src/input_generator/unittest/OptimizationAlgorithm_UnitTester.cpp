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

TEST(PlatoTestXMLGenerator, OptimizationAlgorithm)
{
    XMLGen::InputData tMetaData;
    XMLGen::OptimizationParameters tOptimizationParameters;
    tOptimizationParameters.optimization_type("OT_DAKOTA");
    tOptimizationParameters.optimization_algorithm("oc");
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

    std::vector<std::string> tKeys = {
        "Package",
        "Options",
	"Convergence"};
    std::vector<std::string> tValues = {
        "Dakota asdf",
        "",
	""
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOptimizer);

    auto tOptions = tOptimizer.child("Options");
    ASSERT_FALSE(tOptions.empty());
    tKeys = {
        "Pe",
        "Ops",
        "Coce"};
    tValues = {
        "Daasdf",
        "asdf",
        "asdf"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOptions);

    auto tConvergence = tOptimizer.child("Convergence");
    ASSERT_FALSE(tConvergence.empty());
    tKeys = {
        "MaxIterations"};
    tValues = {
        "15"
        };
    PlatoTestXMLGenerator::test_children(tKeys, tValues, tOptions);

    tConvergence = tConvergence.next_sibling("Convergence");


    tOptions = tOptions.next_sibling("Options");
    ASSERT_TRUE(tOptimizer.empty());

    tOptimizer = tOptimizer.next_sibling("Optimizer");
    ASSERT_TRUE(tOptimizer.empty());


    
}

}//namespace
