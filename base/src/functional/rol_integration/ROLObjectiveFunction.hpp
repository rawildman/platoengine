#ifndef PLATO_FUNCTIONAL_ROLOBJECTIVE
#define PLATO_FUNCTIONAL_ROLOBJECTIVE

#include <ROL_Objective.hpp>
#include <ROL_Vector.hpp>

#include "DynamicVector.hpp"
#include "Function.hpp"

namespace plato::functional::rol_integration
{
class ROLObjectiveFunction : public ROL::Objective<double>
{
   public:
    using ROLPlatoFunction = core::Function<double,
                                                         linear_algebra::DynamicVector<double>,
                                                         const linear_algebra::DynamicVector<double> &>;

    ///@brief Construct a new ROLObjectiveFunction object
    explicit ROLObjectiveFunction(ROLPlatoFunction aROLPlatoFunction);

    ///@brief Compute the value using the set of controls and given tolerance
    double value(const ROL::Vector<double> &aControls, double &aTolerance) override;

    ///@brief Compute the gradient from the set of controls and given tolerance. Populate the aGradient parameter
    ///@param aGradient Modify this variable with the gradient
    void gradient(ROL::Vector<double> &aGradient, const ROL::Vector<double> &aControls, double &aTolerance) override;

   private:
    ROLPlatoFunction mFunction;
};
}  // namespace glato::functional::rol_integration

#endif
