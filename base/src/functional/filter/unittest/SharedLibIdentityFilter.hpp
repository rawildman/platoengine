#ifndef PLATO_FUNCTIONAL_TEST_SHAREDLIBIDENTITYFILTER
#define PLATO_FUNCTIONAL_TEST_SHAREDLIBIDENTITYFILTER

#include <ROL_StdVector.hpp>

#include "FilterInterface.hpp"

namespace Plato::Functional
{
struct MeshProxy;
}

namespace Plato::Functional::Test
{
/// @brief The purpose of this filter is to test the shared library interface.
///
/// It is an identity filter, meaning that the density field will be unchanged on
/// calls to `filter` and its Jacobian is the identity matrix.
class SharedLibIdentityFilter : public FilterInterface
{
   public:
    [[nodiscard]] MeshProxy filter(const MeshProxy& aMeshProxy) const override;

    [[nodiscard]] ROL::StdVector<double> jacobianTimesVector(const MeshProxy& aMeshProxy,
                                                             const ROL::StdVector<double>& aV) const override;
};
}  // namespace Plato::Functional::Test

#endif
