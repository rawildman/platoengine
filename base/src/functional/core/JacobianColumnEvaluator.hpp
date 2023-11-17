#ifndef PLATO_JACOBIAN_COLUMN_EVALUATOR
#define PLATO_JACOBIAN_COLUMN_EVALUATOR

#include <ROL_StdVector.hpp>
#include <functional>

namespace Plato::Functional
{
struct JacobianMultiplier;

/// @brief An implementation of a Jacobian that generates one column at a time.
///
/// @note This is mainly for testing, prefer JacobianMultiplier.
/// @sa JacobianMultiplier
struct JacobianColumnEvaluator
{
    /// @return Column with index @a aIndex.
    [[nodiscard]] ROL::StdVector<double> column(const unsigned int aIndex) const;

    using ColumnFunction = std::function<ROL::StdVector<double>(unsigned int, const ROL::StdVector<double>&)>;

    unsigned int mColumns = 0;
    ROL::StdVector<double> mX;
    ColumnFunction mColumnFunction;
};

/// Implements multiplication of a row vector @a aX with the Jacobian matrix represented by @a aA
[[nodiscard]] ROL::StdVector<double> operator*(const ROL::StdVector<double>& aX, const JacobianColumnEvaluator& aA);

/// @brief Creates a JacobianMultiplier from @a aJacobianColumnEvaluator
JacobianMultiplier to_jacobian_multiplier(JacobianColumnEvaluator aJacobianColumnEvaluator);

}  // namespace Plato::Functional

#endif
