#ifndef PLATO_FUNCTIONAL_JACOBIANMULTIPLIER
#define PLATO_FUNCTIONAL_JACOBIANMULTIPLIER

#include "DynamicVector.hpp"

namespace Plato::Functional
{
/// @brief An object representing the multiplication of a row vector and a Jacobian matrix.
struct JacobianMultiplier
{
    using JacobianTimesVectorFunction = std::function<Core::DynamicVector<double>(const Core::DynamicVector<double>&)>;

    unsigned int mNumColumns = 0;
    JacobianTimesVectorFunction mJacobianTimesVectorFunction;
};

/// @brief Implementation of multiplication of a row vector @a aX
template <typename Arg>
[[nodiscard]] Core::DynamicVector<double> operator*(const Arg& aX, const JacobianMultiplier& aA)
{
    return aA.mJacobianTimesVectorFunction(aX);
}

}  // namespace Plato::Functional

#endif
