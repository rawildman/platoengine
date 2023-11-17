#include "ROLObjectiveFunction.hpp"

namespace Plato::Functional
{

ROLObjectiveFunction::ROLObjectiveFunction(ROLPlatoFunction aROLPlatoFunction) : mFunction(std::move(aROLPlatoFunction))
{
}

double ROLObjectiveFunction::value(const ROL::Vector<double>& aControls, double&)
{
    auto tControlsAsStdVector = dynamic_cast<const ROL::StdVector<double>&>(aControls);
    return mFunction.f(tControlsAsStdVector);
}

void ROLObjectiveFunction::gradient(ROL::Vector<double>& aGradient, const ROL::Vector<double>& aControls, double&)
{
    const auto& tControlsAsStdVector = dynamic_cast<const ROL::StdVector<double>&>(aControls);
    auto& tGradientAsStdVector = dynamic_cast<ROL::StdVector<double>&>(aGradient);
    tGradientAsStdVector.set(mFunction.df(tControlsAsStdVector));
}

}  // namespace Plato::Functional
