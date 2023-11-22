#ifndef PLATO_FUNCTIONAL_JACOBIANMULTIPLIER
#define PLATO_FUNCTIONAL_JACOBIANMULTIPLIER

#include <ROL_StdVector.hpp>

namespace Plato::Functional
{
/// @brief An object representing the multiplication of a row vector and a Jacobian matrix.
struct JacobianMultiplier
{
    using JacobianTimesVectorFunction = std::function<ROL::StdVector<double>(const ROL::StdVector<double>&)>;

    unsigned int mNumColumns = 0;
    JacobianTimesVectorFunction mJacobianTimesVectorFunction;
};

/// @brief Implementation of multiplication of a row vector @a aX
template <typename Arg>
[[nodiscard]] ROL::StdVector<double> operator*(const Arg& aX, const JacobianMultiplier& aA)
{
    return aA.mJacobianTimesVectorFunction(aX);
}

}  // namespace Plato::Functional

#endif
