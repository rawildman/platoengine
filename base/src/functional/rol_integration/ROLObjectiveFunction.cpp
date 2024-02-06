#include "ROLObjectiveFunction.hpp"

#include <ROL_StdVector.hpp>

#include "ROLHelpers.hpp"

namespace plato::functional::rol_integration
{
ROLObjectiveFunction::ROLObjectiveFunction(ROLPlatoFunction aROLPlatoFunction) : mFunction(std::move(aROLPlatoFunction))
{
}

double ROLObjectiveFunction::value(const ROL::Vector<double>& aControls, double&)
{
    return mFunction.f(to_dynamic_vector(aControls));
}

void ROLObjectiveFunction::gradient(ROL::Vector<double>& aGradient, const ROL::Vector<double>& aControls, double&)
{
    linear_algebra::DynamicVector<double> tLocalGradient = mFunction.df(to_dynamic_vector(aControls));
    assign_vector(aGradient, std::move(tLocalGradient).stdVector());
}

}  // namespace plato::functional::rol_integration
