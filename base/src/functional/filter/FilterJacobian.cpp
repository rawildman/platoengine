#include "FilterJacobian.hpp"

#include "FilterInterface.hpp"

namespace Plato::Functional
{
ROL::StdVector<double> operator*(const ROL::StdVector<double>& aV, const FilterJacobian& aJacobian)
{
    assert(aJacobian.mFilter);
    return aJacobian.mFilter->jacobianTimesVector(aJacobian.mMeshProxy, aV);
}

}  // namespace Plato::Functional
