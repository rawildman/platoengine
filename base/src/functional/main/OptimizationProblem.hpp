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
    /// be evident
    void sensitivityCheck() const;

    ///@brief Run the optimization problem
    void optimize();

    /// @brief The dimension of the design parameters being optimized.
    [[nodiscard]] int dimension() const;

   private:
    Plato::Functional::PlatoProblem mProblem;
    ROL::Ptr<ROL::Problem<double>> mROLProblem;
    ROL::Solver<double> mROLSolver;
};

}  // namespace Plato::Functional

#endif
