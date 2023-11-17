#include <gtest/gtest.h>

#include "JacobianMultiplier.hpp"

namespace Plato::Functional::Test
{
namespace pf = Plato::Functional;

TEST(JacobianMultiplier, Identity)
{
    const pf::JacobianMultiplier tIdentityJacobian = {/*.mNumColumns=*/2,
                                                      /*.mJacobianTimesVectorFunction=*/
                                                      [](const ROL::StdVector<double>& aV) { return aV; }};

    const auto tVec = ROL::StdVector<double>{1.0, 2.0};
    const ROL::StdVector<double> tRes = tVec * tIdentityJacobian;
    EXPECT_EQ(*tRes.getVector(), *tVec.getVector());
}

}  // namespace Plato::Functional::Test
