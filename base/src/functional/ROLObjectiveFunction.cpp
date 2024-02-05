#include "ROLObjectiveFunction.hpp"

#include <ROL_StdVector.hpp>

#include "ROLHelpers.hpp"

namespace Plato::Functional
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
    Core::DynamicVector<double> tLocalGradient = mFunction.df(to_dynamic_vector(aControls));
    assign_vector(aGradient, std::move(tLocalGradient).stdVector());
}

}  // namespace Plato::Functional
