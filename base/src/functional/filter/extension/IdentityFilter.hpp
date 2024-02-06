#ifndef PLATO_FUNCTIONAL_IDENTITYFILTER
#define PLATO_FUNCTIONAL_IDENTITYFILTER

#include "FilterInterface.hpp"
#include "Function.hpp"

namespace Plato
{
struct density_topology;
}
namespace plato::functional::core
{
struct MeshProxy;
}

namespace plato::functional::filter::library
{
struct FilterJacobian;
}

namespace plato::functional::filter::extension
{
/// @brief A Filter that does not alter the density field, mostly used for testing.
///
/// Calls to the filter member function return the same density field,
/// and the Jacobian is the identity matrix.
class IdentityFilter : public library::FilterInterface
{
   public:
    [[nodiscard]] core::MeshProxy filter(const core::MeshProxy& aMeshProxy) const override;

    [[nodiscard]] linear_algebra::DynamicVector<double> jacobianTimesVector(
        const core::MeshProxy& aMeshProxy, const linear_algebra::DynamicVector<double>& aV) const override;
};

[[nodiscard]] auto make_identity_filter_function()
    -> core::Function<core::MeshProxy, library::FilterJacobian, const core::MeshProxy&>;

[[nodiscard]] std::optional<std::string> validate_identity_filter(const Plato::density_topology& aInput);

}  // namespace plato::functional::filter::extension

#endif
