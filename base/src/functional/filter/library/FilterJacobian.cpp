#include "filter/library/FilterJacobian.hpp"

#include <cassert>

#include "filter/library/FilterInterface.hpp"

namespace plato::functional::filter::library
{
linear_algebra::DynamicVector<double> operator*(const linear_algebra::DynamicVector<double>& aV,
                                                         const FilterJacobian& aJacobian)
{
    assert(aJacobian.mFilter);
    return aJacobian.mFilter->jacobianTimesVector(aJacobian.mMeshProxy, aV);
}
}  // namespace plato::functional::filter::library
