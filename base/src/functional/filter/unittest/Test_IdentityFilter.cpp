#include <gtest/gtest.h>

#include <ROL_StdVector.hpp>
#include <string_view>

#include "Exception.hpp"
#include "FilterJacobian.hpp"
#include "IdentityFilter.hpp"
#include "MeshProxy.hpp"

namespace
{
constexpr std::string_view kMeshName = "the-mesh-is-a-lie.exo";
const auto kRho = std::vector{-1.0, 0.0, 1.0};
const auto kMeshArgument = Plato::Functional::MeshProxy{kMeshName, kRho};
const auto kV = ROL::StdVector<double>{-2.0, -1.0, 42.0};
}  // namespace

TEST(IdentityFilter, Filter)
{
    namespace pf = Plato::Functional;
    EXPECT_EQ(pf::IdentityFilter{}.filter(kMeshArgument).mNodalDensities, kRho);
}

TEST(IdentityFilter, JacobianTimesVector)
{
    namespace pf = Plato::Functional;

    const auto tV = ROL::StdVector<double>{-2.0, -1.0, 42.0};
    const ROL::StdVector<double> tResult = pf::IdentityFilter{}.jacobianTimesVector(kMeshArgument, tV);
    EXPECT_EQ(*tResult.getVector(), *tV.getVector());
}

TEST(IdentityFilter, JacobianMultiplication)
{
    namespace pf = Plato::Functional;

    const auto tFilterJacobian = pf::FilterJacobian{/*.mFilter=*/std::make_unique<pf::IdentityFilter>(),
                                                    /*.mMeshProxy=*/kMeshArgument};
    const ROL::StdVector<double> tResult = kV * tFilterJacobian;
    EXPECT_EQ(*tResult.getVector(), *kV.getVector());
}

TEST(IdentityFilter, JacobianBadDimensions)
{
    namespace pf = Plato::Functional;

    const auto tFilterJacobian = pf::FilterJacobian{/*.mFilter=*/std::make_unique<pf::IdentityFilter>(),
                                                    /*.mMeshProxy=*/kMeshArgument};

    const auto tVBad = ROL::StdVector<double>{-2.0, -1.0, 42.0, 84.0};
    EXPECT_THROW(auto tTemp = pf::IdentityFilter{}.jacobianTimesVector(kMeshArgument, tVBad), pf::Exception);
    EXPECT_THROW(auto tTemp2 = tVBad * tFilterJacobian, pf::Exception);
}

TEST(IdentityFilter, Function)
{
    namespace pf = Plato::Functional;
    const auto tFilterFunction = pf::make_identity_filter_function();
    EXPECT_EQ(tFilterFunction.f(kMeshArgument).mNodalDensities, kRho);

    const ROL::StdVector<double> tResult = kV * tFilterFunction.df(kMeshArgument);
    EXPECT_EQ(*tResult.getVector(), *kV.getVector());
}
