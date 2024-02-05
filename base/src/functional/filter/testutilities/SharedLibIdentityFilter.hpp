#ifndef PLATO_FUNCTIONAL_TEST_SHAREDLIBIDENTITYFILTER
#define PLATO_FUNCTIONAL_TEST_SHAREDLIBIDENTITYFILTER

#include "DynamicVector.hpp"
#include "FilterInterface.hpp"

namespace Plato::Functional
{
struct MeshProxy;
}

namespace plato::functional::filter::testutilities
{
/// @brief The purpose of this filter is to test the shared library interface.
///
/// It is an identity filter, meaning that the density field will be unchanged on
/// calls to `filter` and its Jacobian is the identity matrix.
class SharedLibIdentityFilter : public library::FilterInterface
{
   public:
    [[nodiscard]] Plato::Functional::MeshProxy filter(const Plato::Functional::MeshProxy& aMeshProxy) const override;

    [[nodiscard]] linear_algebra::DynamicVector<double> jacobianTimesVector(const Plato::Functional::MeshProxy& aMeshProxy,
                                                                  const linear_algebra::DynamicVector<double>& aV) const override;
};
}  // namespace plato::functional::filter::testutilities

#endif
