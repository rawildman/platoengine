#include <gtest/gtest.h>

#include <string_view>

#include "DynamicVector.hpp"
#include "Exception.hpp"
#include "FilterFactory.hpp"
#include "FilterJacobian.hpp"
#include "IdentityFilter.hpp"
#include "InputBlocks.hpp"
#include "InputEnumTypes.hpp"
#include "MeshProxy.hpp"

namespace plato::functional::filter::extension::unittest
{

namespace
{
constexpr std::string_view kMeshName = "the-mesh-is-a-lie.exo";
const auto kRho = std::vector{-1.0, 0.0, 1.0};
const auto kMeshArgument = Plato::Functional::MeshProxy{kMeshName, kRho};
const auto kV = linear_algebra::DynamicVector<double>{-2.0, -1.0, 42.0};
}  // namespace

TEST(FilterFactory, ValidIdentityFilter)
{
    auto tDensityTopology = Plato::density_topology{};
    tDensityTopology.filter_type = Plato::FilterTypes::kIdentity;
    EXPECT_NO_THROW(auto tFunction = library::make_filter_function(tDensityTopology));
}

TEST(IdentityFilter, Filter) { EXPECT_EQ(IdentityFilter{}.filter(kMeshArgument).mNodalDensities, kRho); }

TEST(IdentityFilter, JacobianTimesVector)
{
    const auto tV = linear_algebra::DynamicVector<double>{-2.0, -1.0, 42.0};
    const linear_algebra::DynamicVector<double> tResult = IdentityFilter{}.jacobianTimesVector(kMeshArgument, tV);
    EXPECT_EQ(tResult.stdVector(), tV.stdVector());
}

TEST(IdentityFilter, JacobianMultiplication)
{
    const auto tFilterJacobian = library::FilterJacobian{/*.mFilter=*/std::make_unique<IdentityFilter>(),
                                                         /*.mMeshProxy=*/kMeshArgument};
    const linear_algebra::DynamicVector<double> tResult = kV * tFilterJacobian;
    EXPECT_EQ(tResult.stdVector(), kV.stdVector());
}

TEST(IdentityFilter, JacobianBadDimensions)
{
    namespace pfu = plato::functional::utilities;

    const auto tFilterJacobian = library::FilterJacobian{/*.mFilter=*/std::make_unique<IdentityFilter>(),
                                                         /*.mMeshProxy=*/kMeshArgument};

    const auto tVBad = linear_algebra::DynamicVector<double>{-2.0, -1.0, 42.0, 84.0};
    EXPECT_THROW(auto tTemp = IdentityFilter{}.jacobianTimesVector(kMeshArgument, tVBad), pfu::Exception);
    EXPECT_THROW(auto tTemp2 = tVBad * tFilterJacobian, pfu::Exception);
}

TEST(IdentityFilter, Function)
{
    const auto tFilterFunction = make_identity_filter_function();
    EXPECT_EQ(tFilterFunction.f(kMeshArgument).mNodalDensities, kRho);

    const linear_algebra::DynamicVector<double> tResult = kV * tFilterFunction.df(kMeshArgument);
    EXPECT_EQ(tResult.stdVector(), kV.stdVector());
}
}  // namespace plato::functional::filter::extension::unittest
