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
    mPackage = "OC";
    mControlStagnationTolerance = aParameters.oc_control_stagnation_tolerance();
    mObjectiveStagnationTolerance = aParameters.oc_objective_stagnation_tolerance();
    mGradientTolerance = aParameters.oc_gradient_tolerance();
    mProblemUpdateFrequency = aParameters.problem_update_frequency();



}
void OptimizationAlgorithmPlatoOC::writeInterface(pugi::xml_node& aNode)
{
    auto tOptimizer = aNode.append_child("Optimizer");
    addChild(tOptimizer, "Package",mPackage);
    
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

}
void OptimizationAlgorithmPlatoKSBC::writeInterface(pugi::xml_node& aNode)
{

}

void OptimizationAlgorithmPlatoKSBC::writeAuxiliaryFiles(pugi::xml_node& aNode)
{

}

//****************************ROL **********************************************//
OptimizationAlgorithmROL::OptimizationAlgorithmROL(const OptimizationParameters& aParameters)
: OptimizationAlgorithm(aParameters)
{

}
void OptimizationAlgorithmROL::writeInterface(pugi::xml_node& aNode)
{

}

void OptimizationAlgorithmROL::writeAuxiliaryFiles(pugi::xml_node& aNode)
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
