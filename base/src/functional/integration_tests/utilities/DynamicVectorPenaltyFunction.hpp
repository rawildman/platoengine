#ifndef PLATO_FUNCTIONAL_TEST_DYNAMICVECTORPENALTYFUNCTION
#define PLATO_FUNCTIONAL_TEST_DYNAMICVECTORPENALTYFUNCTION

#include "DynamicVector.hpp"
#include "Function.hpp"
#include "Penalty.hpp"

namespace Plato::Functional::Test
{
struct DynamicVectorJacobian
{
    Core::DynamicVector<double> column(const int aIndex) const;
    TwoDMatrix mJacobian;
};

Core::DynamicVector<double> operator*(const Core::DynamicVector<double>& aX, const DynamicVectorJacobian& aJacobian);

[[nodiscard]] auto make_penalty_dynamic_vector_function(const Penalty& aPenalty) -> Plato::Functional::
    Function<Core::DynamicVector<double>, DynamicVectorJacobian, const Core::DynamicVector<double>&>;

}  // namespace Plato::Functional::Test

#endif
