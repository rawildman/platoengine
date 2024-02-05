#include "FilterJacobian.hpp"

#include <cassert>

#include "FilterInterface.hpp"

namespace Plato::Functional
{
Core::DynamicVector<double> operator*(const Core::DynamicVector<double>& aV, const FilterJacobian& aJacobian)
{
    assert(aJacobian.mFilter);
    return aJacobian.mFilter->jacobianTimesVector(aJacobian.mMeshProxy, aV);
}

}  // namespace Plato::Functional
