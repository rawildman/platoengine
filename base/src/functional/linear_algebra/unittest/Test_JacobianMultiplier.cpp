#include <gtest/gtest.h>

#include "JacobianMultiplier.hpp"

namespace Plato::Functional::Test
{
namespace pf = Plato::Functional;

TEST(JacobianMultiplier, Identity)
{
    const pf::JacobianMultiplier tIdentityJacobian = {/*.mNumColumns=*/2,
                                                      /*.mJacobianTimesVectorFunction=*/
                                                      [](const Core::DynamicVector<double>& aV) { return aV; }};

    const auto tVec = Core::DynamicVector<double>{1.0, 2.0};
    const Core::DynamicVector<double> tRes = tVec * tIdentityJacobian;
    EXPECT_EQ(tRes.stdVector(), tVec.stdVector());
}

}  // namespace Plato::Functional::Test
