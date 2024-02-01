#ifndef PLATO_FUNCTIONAL_FILTERJACOBIAN
#define PLATO_FUNCTIONAL_FILTERJACOBIAN

#include <memory>

#include "DynamicVector.hpp"
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
Core::DynamicVector<double> operator*(const Core::DynamicVector<double>& aV, const FilterJacobian& aJacobian);

}  // namespace Plato::Functional

#endif
