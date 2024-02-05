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
    Plato::Functional::MeshProxy mMeshProxy;
};

/// @pre `FilterJacobian::mFilter` must not be `nullptr`.
Plato::Functional::Core::DynamicVector<double> operator*(const Plato::Functional::Core::DynamicVector<double>& aV, const FilterJacobian& aJacobian);

}  // namespace plato::functional::filter::library

#endif
