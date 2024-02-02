#include "DynamicVectorPenaltyFunction.hpp"

#include <cassert>

#include "DynamicVectorRosenbrockFunction.hpp"

namespace Plato::Functional::Test
{
Core::DynamicVector<double> DynamicVectorJacobian::column(const int aIndex) const
{
    return Core::DynamicVector<double>{mJacobian(0, aIndex), mJacobian(1, aIndex)};
}

Core::DynamicVector<double> operator*(const Core::DynamicVector<double>& aX, const DynamicVectorJacobian& aJacobian)
{
    assert(aX.size() == 2);
    return Core::DynamicVector<double>{aX.dot(aJacobian.column(0)), aX.dot(aJacobian.column(1))};
}

auto make_penalty_dynamic_vector_function(const Penalty& aPenalty)
    -> Plato::Functional::Function<Core::DynamicVector<double>, DynamicVectorJacobian, const Core::DynamicVector<double>&>
{
    return Plato::Functional::make_function(
        [p = aPenalty](const Core::DynamicVector<double>& x) { return to_dynamic_vector(p.f(x[0], x[1])); },
        [p = aPenalty](const Core::DynamicVector<double>& x) { return DynamicVectorJacobian{p.df(x[0], x[1])}; });
}

}  // namespace Plato::Functional::Test