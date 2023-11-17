#ifndef PLATO_FUNCTIONAL_FILTERJACOBIAN
#define PLATO_FUNCTIONAL_FILTERJACOBIAN

#include <ROL_StdVector.hpp>
#include <memory>

#include "MeshProxy.hpp"

namespace Plato::Functional
{
class FilterInterface;

/// @brief A wrapper for a filter an a filter argument, used for implementing operator*.
struct FilterJacobian
{
    std::shared_ptr<FilterInterface> mFilter;
    MeshProxy mMeshProxy;
};

/// @pre `FilterJacobian::mFilter` must not be `nullptr`.
ROL::StdVector<double> operator*(const ROL::StdVector<double>& aV, const FilterJacobian& aJacobian);

}  // namespace Plato::Functional

#endif
