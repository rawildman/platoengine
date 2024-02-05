#include <gtest/gtest.h>

#include "JacobianColumnEvaluator.hpp"
#include "JacobianMultiplier.hpp"

namespace Plato::Functional::Test
{
namespace pf = Plato::Functional;

TEST(JacobianColumnEvaluator, MultiplicationOperatorOverload)
{
    const pf::JacobianColumnEvaluator tJacobian = {/*.mColumns=*/1,
                                                   /*.mX=*/Core::DynamicVector<double>{1.0, 2.0, 3.0, 4.0},
                                                   /*.mColumnFunction=*/[](unsigned int, Core::DynamicVector<double>) {
                                                       return Core::DynamicVector<double>{-1.0, -2.0, -3.0, -4.0};
                                                   }};

    const Core::DynamicVector<double> tVec{1.0, 2.0, 3.0, 4.0};
    const std::vector<double> tGold{-30};
    const Core::DynamicVector<double> tResult = tVec * tJacobian;
    EXPECT_EQ(tResult.stdVector(), tGold);
}

TEST(JacobianColumnEvaluator, ToJacobianMultiplier)
{
    const pf::JacobianColumnEvaluator tJacobianColumn = {
        /*.mColumns=*/1,
        /*.mX=*/Core::DynamicVector<double>{1.0, 2.0, 3.0, 4.0},
        /*.mColumnFunction=*/[](unsigned int, Core::DynamicVector<double>) {
            return Core::DynamicVector<double>{-1.0, -2.0, -3.0, -4.0};
        }};
    const pf::JacobianMultiplier tJacobian = pf::to_jacobian_multiplier(std::move(tJacobianColumn));
    const Core::DynamicVector<double> tVec{1.0, 2.0, 3.0, 4.0};
    const std::vector<double> tGold{-30};
    const Core::DynamicVector<double> tResult = tVec * tJacobian;
    EXPECT_EQ(tResult.stdVector(), tGold);
}

}  // namespace Plato::Functional::Test
