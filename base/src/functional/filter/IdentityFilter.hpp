#ifndef PLATO_FUNCTIONAL_IDENTITYFILTER
#define PLATO_FUNCTIONAL_IDENTITYFILTER

#include "FilterInterface.hpp"
#include "Function.hpp"

namespace Plato
{
struct density_topology;
}
namespace Plato::Functional
{
class FilterJacobian;
struct MeshProxy;

/// @brief A Filter that does not alter the density field, mostly used for testing.
///
/// Calls to the filter member function return the same density field,
/// and the Jacobian is the identity matrix.
class IdentityFilter : public FilterInterface
{
   public:
    [[nodiscard]] MeshProxy filter(const MeshProxy& aMeshProxy) const override;

    [[nodiscard]] ROL::StdVector<double> jacobianTimesVector(const MeshProxy& aMeshProxy,
                                                             const ROL::StdVector<double>& aV) const override;
};

[[nodiscard]] auto make_identity_filter_function()
    -> Function<MeshProxy, FilterJacobian, const MeshProxy&>;

}  // namespace Plato::Functional

#endif
