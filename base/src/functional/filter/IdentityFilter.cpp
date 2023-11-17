#include "IdentityFilter.hpp"

#include <ROL_StdVector.hpp>

#include "Exception.hpp"
#include "FilterJacobian.hpp"
#include "MeshProxy.hpp"

namespace Plato::Functional
{
MeshProxy IdentityFilter::filter(const MeshProxy& aMeshProxy) const { return aMeshProxy; }

ROL::StdVector<double> IdentityFilter::jacobianTimesVector(const MeshProxy& aMeshProxy,
                                                           const ROL::StdVector<double>& aV) const
{
    const auto tVectorDimension = static_cast<std::size_t>(aV.dimension());
    const std::size_t tDensityDimension = aMeshProxy.mNodalDensities.size();
    if (tVectorDimension != tDensityDimension)
    {
        throw Exception{
            "IdentityFilter jacobian multiplication: Dimensions of vector and nodal density field don't match. Vector "
            "dimension: " +
            std::to_string(tVectorDimension) + ", density dimension: " + std::to_string(tDensityDimension)};
    }
    return aV;
}

auto make_identity_filter_function() -> Function<MeshProxy, FilterJacobian, const MeshProxy&>
{
    return make_function([](const MeshProxy& aMeshProxy) { return IdentityFilter{}.filter(aMeshProxy); },
                         [](const MeshProxy& aMeshProxy) {
                             return FilterJacobian{std::make_unique<IdentityFilter>(), aMeshProxy};
                         });
}
}  // namespace Plato::Functional
