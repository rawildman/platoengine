#ifndef PLATO_FUNCTIONAL_OPTIMIZATIONPROBLEM
#define PLATO_FUNCTIONAL_OPTIMIZATIONPROBLEM

#include <string_view>

#include "PlatoProblem.hpp"
#include "ROL_Problem.hpp"

namespace Plato::Functional
{

class OptimizationProblem
{
   public:
    ///@brief Construct a new Optimization Problem object
    ///@param aInputFile filename of the block structured plato input file
    explicit OptimizationProblem(const std::string_view aInputFile);

    ///@brief Run gradient check on the parsed/defined ROL problem
    void gradientCheck() const;

    ///@brief Run constraint check on the parsed/defined ROL problem
    void constraintCheck() const;

    ///@brief Run a gradient check using a linear pseudo-objective so that errors in the parameter sensitivities might
    ///be evident
    void sensitivityCheck() const;

    ///@brief Run a gradient check on any individual objectives before aggregation
    ///@param aInput validated parsed input to construct objective functions
    void gradientCheckIndividualObjectives(const std::vector<Plato::objective>& aInput) const;

    ///@brief Run the optimization problem
    void optimize();

   private:
    Plato::Functional::PlatoProblem mProblem;
    ROL::Ptr<ROL::Problem<double>> mROLProblem;
    ROL::Solver<double> mROLSolver;

    ///@brief Helper function that creates a perturbation of the initial value for use in diagnostic checks
    ///
    ///@return ROL::StdVector<double>
    ROL::StdVector<double> generatePerturbation() const;
};

}  // namespace Plato::Functional

#endif
