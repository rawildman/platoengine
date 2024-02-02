#include "JacobianColumnEvaluator.hpp"

#include "JacobianMultiplier.hpp"

namespace Plato::Functional
{
Core::DynamicVector<double> JacobianColumnEvaluator::column(const unsigned int aIndex) const
{
    return mColumnFunction(aIndex, mX);
}

Core::DynamicVector<double> operator*(const Core::DynamicVector<double>& aX, const JacobianColumnEvaluator& aA)
{
    auto tResult = std::vector<double>(aA.mColumns);
    for (unsigned int i = 0; i < aA.mColumns; ++i)
    {
        tResult[i] = aX.dot(aA.column(i));
    }
    return Core::DynamicVector<double>(std::move(tResult));
}

JacobianMultiplier to_jacobian_multiplier(JacobianColumnEvaluator aJacobianColumnEvaluator)
{
    return JacobianMultiplier{/*.mNumColumns=*/aJacobianColumnEvaluator.mColumns,
                              /*.mJacobianTimesVector=*/[tColumnEvaluator = std::move(aJacobianColumnEvaluator)](
                                                            const Core::DynamicVector<double>& aX)
                              { return aX * tColumnEvaluator; }};
}
}  // namespace Plato::Functional
