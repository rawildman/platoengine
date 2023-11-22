#ifndef PLATO_FUNCTIONAL_ROLHELPERS
#define PLATO_FUNCTIONAL_ROLHELPERS

#include <ROL_StdVector.hpp>

namespace Plato::Functional
{
/// @brief Scalar multiplication operator overload so that ROL vector types can be used with Aggregate
[[nodiscard]] ROL::StdVector<double> operator*(double aAlpha, ROL::StdVector<double> aVec);

/// @brief Addition operator overload so that ROL vector types can be used with Aggregate
[[nodiscard]] ROL::StdVector<double> operator+(const ROL::StdVector<double>& aVec1, ROL::StdVector<double> aVec2);

/// @brief Deep copies a `ROL::StdVector`
[[nodiscard]] ROL::Ptr<ROL::StdVector<double>> copy_vector(const ROL::StdVector<double>& aVector);

}  // namespace Plato::Functional

#endif
