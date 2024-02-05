#include "DynamicVectorPenaltyFunction.hpp"

#include <cassert>

#include "DynamicVectorRosenbrockFunction.hpp"

namespace plato::functional::integration_tests::utilities
{
Plato::Functional::Core::DynamicVector<double> DynamicVectorJacobian::column(const int aIndex) const
{
    return Plato::Functional::Core::DynamicVector<double>{mJacobian(0, aIndex), mJacobian(1, aIndex)};
}

Plato::Functional::Core::DynamicVector<double> operator*(const Plato::Functional::Core::DynamicVector<double>& aX,
                                                         const DynamicVectorJacobian& aJacobian)
{
    assert(aX.size() == 2);
    return Plato::Functional::Core::DynamicVector<double>{aX.dot(aJacobian.column(0)), aX.dot(aJacobian.column(1))};
}

auto make_penalty_dynamic_vector_function(const Plato::Functional::Test::Penalty& aPenalty)
    -> Plato::Functional::Function<Plato::Functional::Core::DynamicVector<double>,
                                   DynamicVectorJacobian,
                                   const Plato::Functional::Core::DynamicVector<double>&>
{
    return Plato::Functional::make_function([p = aPenalty](const Plato::Functional::Core::DynamicVector<double>& x)
                                            { return to_dynamic_vector(p.f(x[0], x[1])); },
                                            [p = aPenalty](const Plato::Functional::Core::DynamicVector<double>& x)
                                            { return DynamicVectorJacobian{p.df(x[0], x[1])}; });
}

}  // namespace plato::functional::integration_tests::utilities