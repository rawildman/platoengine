#include "ROLPenaltyFunction.hpp"

#include <cassert>

#include "ROLTestUtilities.hpp"

namespace Plato::Functional::Test
{
ROL::StdVector<double> ROLVectorJacobian::column(const int aIndex) const
{
    return ROL::StdVector<double>{mJacobian(0, aIndex), mJacobian(1, aIndex)};
}

ROL::StdVector<double> operator*(const ROL::StdVector<double>& aX, const ROLVectorJacobian& aJacobian)
{
    assert(aX.dimension() == 2);
    auto tResult = ROL::StdVector<double>(2, 0.0);
    tResult[0] = aX.dot(aJacobian.column(0));
    tResult[1] = aX.dot(aJacobian.column(1));
    return tResult;
}

auto make_penalty_rol_vector_function(const Penalty& aPenalty)
    -> Plato::Functional::Function<ROL::StdVector<double>, ROLVectorJacobian, const ROL::StdVector<double>&>
{
    return Plato::Functional::make_function(
        [p = aPenalty](const ROL::StdVector<double>& x) { return toROLVector(p.f(x[0], x[1])); },
        [p = aPenalty](const ROL::StdVector<double>& x) { return ROLVectorJacobian{p.df(x[0], x[1])}; });
}

}  // namespace Plato::Functional::Test