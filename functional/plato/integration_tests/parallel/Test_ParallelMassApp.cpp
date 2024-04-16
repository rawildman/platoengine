#include <gtest/gtest.h>

#include <boost/mpi/communicator.hpp>
#include <string_view>

#include "plato/core/MeshProxy.hpp"
#include "plato/criteria/extension/SharedLibCriterion.hpp"
#include "plato/utilities/STKUtilities.hpp"

namespace plato::integration_tests::parallel
{
namespace
{
constexpr auto kLibPath = std::string_view{"libPlatoTestMassObjective.so"};
constexpr auto kMeshName = std::string_view{"massTest.exo"};
constexpr auto kMeshCommand = std::string_view{"generated:1x1x1|bbox:-1,-1,-1,1,1,1"};
}  // namespace

TEST(ParallelMassObjective, CallValue)
{
    auto tComm = boost::mpi::communicator{};
    EXPECT_GT(tComm.size(), 1u);

    const auto tSharedLib = criteria::extension::SharedLibCriterion{std::string{kLibPath}, {}, tComm};

    utilities::write_mesh(kMeshName, utilities::create_mesh(kMeshCommand));
    const double tMass = tSharedLib.f(core::MeshProxy{kMeshName, {}});
    EXPECT_DOUBLE_EQ(tMass, 8.0);
}

TEST(ParallelMassObjective, CallGradient)
{
    auto tComm = boost::mpi::communicator{};
    EXPECT_GT(tComm.size(), 1u);

    const auto tSharedLib = criteria::extension::SharedLibCriterion{std::string{kLibPath}, {}, tComm};

    utilities::write_mesh(kMeshName, utilities::create_mesh(kMeshCommand));
    const auto tGrad = tSharedLib.df(core::MeshProxy{kMeshName, {}});

    const std::vector<double> tGold(24, 0.0);
    EXPECT_EQ(tGrad.stdVector(), tGold);
}
}  // namespace plato::integration_tests::parallel