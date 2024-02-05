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
    [[nodiscard]] Plato::Functional::MeshProxy filter(const Plato::Functional::MeshProxy& aMeshProxy) const override;

    [[nodiscard]] linear_algebra::DynamicVector<double> jacobianTimesVector(
        const Plato::Functional::MeshProxy& aMeshProxy,
        const linear_algebra::DynamicVector<double>& aV) const override;
};

[[nodiscard]] auto make_identity_filter_function() -> Plato::Functional::
    Function<Plato::Functional::MeshProxy, library::FilterJacobian, const Plato::Functional::MeshProxy&>;

[[nodiscard]] std::optional<std::string> validate_identity_filter(const Plato::density_topology& aInput);

}  // namespace plato::functional::filter::extension

#endif
