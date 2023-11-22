#ifndef PLATO_FUNCTIONAL_TEST_ROLPENALTYFUNCTION
#define PLATO_FUNCTIONAL_TEST_ROLPENALTYFUNCTION

#include <ROL_StdVector.hpp>

#include "Function.hpp"
#include "Penalty.hpp"

namespace Plato::Functional::Test
{
struct ROLVectorJacobian
{
    ROL::StdVector<double> column(const int aIndex) const;
    TwoDMatrix mJacobian;
};

ROL::StdVector<double> operator*(const ROL::StdVector<double>& aX, const ROLVectorJacobian& aJacobian);

[[nodiscard]] auto make_penalty_rol_vector_function(const Penalty& aPenalty)
    -> Plato::Functional::Function<ROL::StdVector<double>, ROLVectorJacobian, const ROL::StdVector<double>&>;

}  // namespace Plato::Functional::Test

#endif
