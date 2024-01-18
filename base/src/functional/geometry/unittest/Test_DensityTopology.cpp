#include <gtest/gtest.h>

#include <cmath>
#include <filesystem>
#include <numeric>
#include <stk_mesh/base/BulkData.hpp>
#include <stk_mesh/base/Comm.hpp>
#include <stk_util/parallel/Parallel.hpp>
#include <string_view>
#include <vector>

#include "DensityTopology.hpp"
#include "InputGeneration.hpp"
#include "JacobianColumnEvaluator.hpp"
#include "MeshProxy.hpp"
#include "Plato_InputBlocks.hpp"
#include "STKUtilities.hpp"

namespace
{
const auto kDensityInput = Plato::Functional::TestUtilities::create_valid_density_topology_geometry();

constexpr int kExpectedDensitySize = 8;  // Based on mesh generation command below (1x1x1)

void create_small_mesh(const std::string& aFileName)
{
    namespace pf = Plato::Functional;
    ASSERT_EQ(stk::parallel_machine_size(MPI_COMM_WORLD), 1);
    auto bulk = pf::create_mesh("generated:1x1x1|bbox:-1,-2,-1,2,1,2");
    pf::write_mesh(aFileName, bulk);
}
}  // namespace

TEST(DensityTopology, Jacobian)
{
    namespace pf = Plato::Functional;
    create_small_mesh(kDensityInput.mesh_name->mName);

    const pf::DensityTopology tDensityTopology(kDensityInput);

    const std::vector<double> tDesignVars = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
    const ROL::StdVector<double> tDesignVec(ROL::makePtr<std::vector<double>>(tDesignVars));
    const int tNumDesignParameters = tDesignVec.dimension();

    const pf::JacobianMultiplier tJacobian = tDensityTopology.jacobian(tDesignVec);

    std::vector<double> tRowVec(tNumDesignParameters, 0.0);
    std::iota(tRowVec.begin(), tRowVec.end(), 1.0);
    const ROL::StdVector<double> tRolVec(ROL::makePtr<std::vector<double>>(tRowVec));

    // Jacobian is identity matrix
    const std::vector<double> tGold = tRowVec;
    const ROL::StdVector<double> tRes = tRolVec * tJacobian;
    EXPECT_EQ(*(tRes.getVector()), tGold);

    EXPECT_TRUE(std::filesystem::remove(kDensityInput.mesh_name->mName));
}

TEST(DensityTopology, GenerateMesh)
{
    namespace pf = Plato::Functional;
    create_small_mesh(kDensityInput.mesh_name->mName);

    const pf::DensityTopology tDensityTopology(kDensityInput);

    const std::vector<double> tDesignVars = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
    const ROL::StdVector<double> tDesignVec(ROL::makePtr<std::vector<double>>(tDesignVars));

    const auto tMeshProxy = tDensityTopology.generateMesh(tDesignVec);
    EXPECT_EQ(tMeshProxy.mNodalDensities, tDesignVars);

    EXPECT_TRUE(std::filesystem::remove(kDensityInput.mesh_name->mName));
}

TEST(DensityTopology, InitialGuess)
{
    namespace pf = Plato::Functional;
    create_small_mesh(kDensityInput.mesh_name->mName);

    const std::unique_ptr<ROL::StdVector<double>> tInitialGuess =
        pf::DensityTopology::initialGuess(kDensityInput.mesh_name->mName);

    EXPECT_EQ(tInitialGuess->dimension(), kExpectedDensitySize);

    for (const double val : *tInitialGuess->getVector())
    {
        EXPECT_EQ(val, 0.5);
    }

    EXPECT_TRUE(std::filesystem::remove(kDensityInput.mesh_name->mName));
}

TEST(DensityTopology, Bounds)
{
    namespace pf = Plato::Functional;
    create_small_mesh(kDensityInput.mesh_name->mName);

    const auto [tLowerBounds, tUpperBounds] = pf::DensityTopology::bounds(kDensityInput.mesh_name->mName);

    EXPECT_EQ(tLowerBounds.size(), kExpectedDensitySize);
    EXPECT_EQ(tUpperBounds.size(), kExpectedDensitySize);

    EXPECT_TRUE(std::all_of(tLowerBounds.cbegin(), tLowerBounds.cend(), [](const double aVal) { return aVal == 0.0; }));
    EXPECT_TRUE(std::all_of(tUpperBounds.cbegin(), tUpperBounds.cend(), [](const double aVal) { return aVal == 1.0; }));

    EXPECT_TRUE(std::filesystem::remove(kDensityInput.mesh_name->mName));
}
