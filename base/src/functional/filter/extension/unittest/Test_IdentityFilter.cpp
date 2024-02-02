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

namespace
{
constexpr std::string_view kMeshName = "the-mesh-is-a-lie.exo";
const auto kRho = std::vector{-1.0, 0.0, 1.0};
const auto kMeshArgument = Plato::Functional::MeshProxy{kMeshName, kRho};
const auto kV = Plato::Functional::Core::DynamicVector<double>{-2.0, -1.0, 42.0};
}  // namespace

TEST(FilterFactory, ValidIdentityFilter)
{
    auto tDensityTopology = Plato::density_topology{};
    tDensityTopology.filter_type = Plato::FilterTypes::kIdentity;
    EXPECT_NO_THROW(auto tFunction = Plato::Functional::FilterFactory::make_filter_function(tDensityTopology));
}

TEST(IdentityFilter, Filter)
{
    namespace pf = Plato::Functional;
    EXPECT_EQ(pf::IdentityFilter{}.filter(kMeshArgument).mNodalDensities, kRho);
}

TEST(IdentityFilter, JacobianTimesVector)
{
    namespace pf = Plato::Functional;

    const auto tV = pf::Core::DynamicVector<double>{-2.0, -1.0, 42.0};
    const pf::Core::DynamicVector<double> tResult = pf::IdentityFilter{}.jacobianTimesVector(kMeshArgument, tV);
    EXPECT_EQ(tResult.stdVector(), tV.stdVector());
}

TEST(IdentityFilter, JacobianMultiplication)
{
    namespace pf = Plato::Functional;

    const auto tFilterJacobian = pf::FilterJacobian{/*.mFilter=*/std::make_unique<pf::IdentityFilter>(),
                                                    /*.mMeshProxy=*/kMeshArgument};
    const pf::Core::DynamicVector<double> tResult = kV * tFilterJacobian;
    EXPECT_EQ(tResult.stdVector(), kV.stdVector());
}

TEST(IdentityFilter, JacobianBadDimensions)
{
    namespace pf = Plato::Functional;

    const auto tFilterJacobian = pf::FilterJacobian{/*.mFilter=*/std::make_unique<pf::IdentityFilter>(),
                                                    /*.mMeshProxy=*/kMeshArgument};

    const auto tVBad = pf::Core::DynamicVector<double>{-2.0, -1.0, 42.0, 84.0};
    EXPECT_THROW(auto tTemp = pf::IdentityFilter{}.jacobianTimesVector(kMeshArgument, tVBad), pf::Exception);
    EXPECT_THROW(auto tTemp2 = tVBad * tFilterJacobian, pf::Exception);
}

TEST(IdentityFilter, Function)
{
    namespace pf = Plato::Functional;
    const auto tFilterFunction = pf::make_identity_filter_function();
    EXPECT_EQ(tFilterFunction.f(kMeshArgument).mNodalDensities, kRho);

    const pf::Core::DynamicVector<double> tResult = kV * tFilterFunction.df(kMeshArgument);
    EXPECT_EQ(tResult.stdVector(), kV.stdVector());
}
