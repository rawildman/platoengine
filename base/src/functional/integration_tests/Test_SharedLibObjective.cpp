#include <gtest/gtest.h>

#include "BrickShapeGeometry.hpp"
#include "Exception.hpp"
#include "MeshProxy.hpp"
#include "STKUtilities.hpp"
#include "SharedLibCriterion.hpp"

namespace
{
// These tests use the mass objective library but through the shared library interface so they are testing the shared
// library more generically
constexpr std::string_view kLibPath = "libPlatoTestMassObjective.so";

void generate_bad_library_and_do_nothing()
{
    // This function should throw an exception
    namespace pf = Plato::Functional;
    const auto tBad = pf::SharedLibCriterion{std::string{"badRobot.so"}, {}};
    std::cout << tBad.f(pf::MeshProxy{"dne.exo", {}}) << std::endl;
}
}  // namespace

TEST(SharedLibObjective, BadLibraryPath)
{
    namespace pf = Plato::Functional;
    EXPECT_THROW(generate_bad_library_and_do_nothing(), Plato::Functional::Exception);
}

TEST(SharedLibObjective, CallValue)
{
    namespace pf = Plato::Functional;
    const auto tSharedLib = pf::SharedLibCriterion{std::string{kLibPath}, {}};

    constexpr std::string_view tMeshName = "massTest.exo";
    pf::write_mesh(tMeshName, pf::create_mesh("generated:1x1x1|bbox:-1,-1,-1,1,1,1"));
    const double tMass = tSharedLib.f(pf::MeshProxy{tMeshName, {}});
    EXPECT_DOUBLE_EQ(tMass, 8.0);
}

TEST(SharedLibObjective, CallGradient)
{
    namespace pf = Plato::Functional;
    const auto tSharedLib = pf::SharedLibCriterion{std::string{kLibPath}, {}};

    constexpr std::string_view tMeshName = "massTest.exo";
    pf::write_mesh(tMeshName, pf::create_mesh("generated:1x1x1|bbox:-1,-1,-1,1,1,1"));
    const auto tGrad = tSharedLib.df(pf::MeshProxy{tMeshName, {}});

    const std::vector<double> tGold(24, 0);
    EXPECT_EQ(tGrad.stdVector(), tGold);
}

TEST(SharedLibObjective, ValueUsingFunction)
{
    namespace pf = Plato::Functional;
    const auto tFunction = pf::make_shared_lib_function(pf::SharedLibCriterion{std::string{kLibPath}, {}});

    constexpr std::string_view tMeshName = "massTest.exo";
    pf::write_mesh(tMeshName, pf::create_mesh("generated:1x1x1|bbox:-1,-1,-1,1,1,1"));
    const double tMass = tFunction.f(pf::MeshProxy{tMeshName, {}});
    EXPECT_DOUBLE_EQ(tMass, 8.0);
}
