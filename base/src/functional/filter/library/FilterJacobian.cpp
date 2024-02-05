#include "FilterJacobian.hpp"

#include <cassert>

#include "FilterInterface.hpp"

namespace plato::functional::filter::library
{
Plato::Functional::Core::DynamicVector<double> operator*(const Plato::Functional::Core::DynamicVector<double>& aV, const FilterJacobian& aJacobian)
{
    assert(aJacobian.mFilter);
    return aJacobian.mFilter->jacobianTimesVector(aJacobian.mMeshProxy, aV);
}

}  // namespace plato::functional::filter::library
