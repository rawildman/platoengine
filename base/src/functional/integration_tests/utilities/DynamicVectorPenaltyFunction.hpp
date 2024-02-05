#ifndef PLATO_FUNCTIONAL_TEST_DYNAMICVECTORPENALTYFUNCTION
#define PLATO_FUNCTIONAL_TEST_DYNAMICVECTORPENALTYFUNCTION

#include "DynamicVector.hpp"
#include "Function.hpp"
#include "Penalty.hpp"

namespace plato::functional::integration_tests::utilities
{
struct DynamicVectorJacobian
{
    linear_algebra::DynamicVector<double> column(const int aIndex) const;
    Plato::Functional::Test::TwoDMatrix mJacobian;
};

[[nodiscard]] linear_algebra::DynamicVector<double> operator*(
    const linear_algebra::DynamicVector<double>& aX, const DynamicVectorJacobian& aJacobian);

[[nodiscard]] auto make_penalty_dynamic_vector_function(const Plato::Functional::Test::Penalty& aPenalty)
    -> Plato::Functional::Function<linear_algebra::DynamicVector<double>,
                                   DynamicVectorJacobian,
                                   const linear_algebra::DynamicVector<double>&>;

}  // namespace plato::functional::integration_tests::utilities

#endif
