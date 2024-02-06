#ifndef PLATO_FUNCTIONAL_FILTERJACOBIAN
#define PLATO_FUNCTIONAL_FILTERJACOBIAN

#include <memory>

#include "DynamicVector.hpp"
#include "MeshProxy.hpp"

namespace plato::functional::filter::library
{
class FilterInterface;

/// @brief A wrapper for a filter an a filter argument, used for implementing operator*.
struct FilterJacobian
{
    std::shared_ptr<FilterInterface> mFilter;
    core::MeshProxy mMeshProxy;
};

/// @pre `FilterJacobian::mFilter` must not be `nullptr`.
linear_algebra::DynamicVector<double> operator*(const linear_algebra::DynamicVector<double>& aV,
                                                const FilterJacobian& aJacobian);

}  // namespace plato::functional::filter::library
#endif
