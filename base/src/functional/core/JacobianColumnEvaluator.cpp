#include "JacobianColumnEvaluator.hpp"

#include "JacobianMultiplier.hpp"

namespace Plato::Functional
{
ROL::StdVector<double> JacobianColumnEvaluator::column(const unsigned int aIndex) const
{
    return mColumnFunction(aIndex, mX);
}

ROL::StdVector<double> operator*(const ROL::StdVector<double>& aX, const JacobianColumnEvaluator& aA)
{
    auto tResult = ROL::StdVector<double>(aA.mColumns, 0.0);
    for (unsigned int i = 0; i < aA.mColumns; ++i)
    {
        tResult[i] = aX.dot(aA.column(i));
    }
    return tResult;
}

JacobianMultiplier to_jacobian_multiplier(JacobianColumnEvaluator aJacobianColumnEvaluator)
{
    return JacobianMultiplier{/*.mNumColumns=*/aJacobianColumnEvaluator.mColumns,
                              /*.mJacobianTimesVector=*/[tColumnEvaluator = std::move(aJacobianColumnEvaluator)](
                                                            const ROL::StdVector<double>& aX)
                              { return aX * tColumnEvaluator; }};
}
}  // namespace Plato::Functional
