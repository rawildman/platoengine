#ifndef PLATO_FUNCTIONAL_ROLOBJECTIVE
#define PLATO_FUNCTIONAL_ROLOBJECTIVE

#include <ROL_Objective.hpp>
#include <ROL_StdVector.hpp>

#include "Function.hpp"

namespace Plato::Functional
{

class ROLObjectiveFunction : public ROL::Objective<double>
{
   public:
    using ROLPlatoFunction =
        Plato::Functional::Function<double, ROL::StdVector<double>, const ROL::StdVector<double> &>;

    ///@brief Construct a new ROLObjectiveFunction object
    ///
    ///@param aROLPlatoFunction
    explicit ROLObjectiveFunction(ROLPlatoFunction aROLPlatoFunction);

    ///@brief Compute the value using the set of controls and given tolerance
    ///
    ///@param aControls
    ///@param aTolerance
    ///@return double
    double value(const ROL::Vector<double> &aControls, double &aTolerance) override;

    ///@brief Compute the gradient from the set of controls and given tolerance. Populate the aGradient parameter
    ///
    ///@param aGradient Modify this variable with the gradient
    ///@param aControls
    ///@param aTolerance
    void gradient(ROL::Vector<double> &aGradient, const ROL::Vector<double> &aControls, double &aTolerance) override;

   private:
    ROLPlatoFunction mFunction;
};
}  // namespace Plato::Functional

#endif
