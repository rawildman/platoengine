#include <gtest/gtest.h>

#include "JacobianColumnEvaluator.hpp"
#include "JacobianMultiplier.hpp"

namespace Plato::Functional::Test
{
namespace pf = Plato::Functional;

TEST(JacobianColumnEvaluator, MultiplicationOperatorOverload)
{
    const pf::JacobianColumnEvaluator tJacobian = {/*.mColumns=*/1,
                                                   /*.mX=*/ROL::StdVector<double>{1.0, 2.0, 3.0, 4.0},
                                                   /*.mColumnFunction=*/[](unsigned int, ROL::StdVector<double>) {
                                                       return ROL::StdVector<double>{-1.0, -2.0, -3.0, -4.0};
                                                   }};

    const ROL::StdVector<double> tVec{1.0, 2.0, 3.0, 4.0};
    const std::vector<double> tGold{-30};
    const ROL::StdVector<double> tResult = tVec * tJacobian;
    EXPECT_EQ(*(tResult.getVector()), tGold);
}

TEST(JacobianColumnEvaluator, ToJacobianMultiplier)
{
    const pf::JacobianColumnEvaluator tJacobianColumn = {/*.mColumns=*/1,
                                                         /*.mX=*/ROL::StdVector<double>{1.0, 2.0, 3.0, 4.0},
                                                         /*.mColumnFunction=*/[](unsigned int, ROL::StdVector<double>) {
                                                             return ROL::StdVector<double>{-1.0, -2.0, -3.0, -4.0};
                                                         }};
    const pf::JacobianMultiplier tJacobian = pf::to_jacobian_multiplier(std::move(tJacobianColumn));
    const ROL::StdVector<double> tVec{1.0, 2.0, 3.0, 4.0};
    const std::vector<double> tGold{-30};
    const ROL::StdVector<double> tResult = tVec * tJacobian;
    EXPECT_EQ(*(tResult.getVector()), tGold);
}

}  // namespace Plato::Functional::Test
