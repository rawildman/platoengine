#include <gtest/gtest.h>

#include "BrickShapeGeometry.hpp"
#include "Exception.hpp"
#include "MeshProxy.hpp"
#include "STKUtilities.hpp"
#include "SharedLibCriterion.hpp"

namespace plato::functional::integration_tests::serial
{
namespace
{
// These tests use the mass objective library but through the shared library interface so they are testing the shared
// library more generically
constexpr std::string_view kLibPath = "libPlatoTestMassObjective.so";

void generate_bad_library_and_do_nothing()
{
    // This function should throw an exception
    namespace pfu = plato::functional::utilities;
    const auto tBad = criteria::extension::SharedLibCriterion{std::string{"badRobot.so"}, {}};
    std::cout << tBad.f(Plato::Functional::MeshProxy{"dne.exo", {}}) << std::endl;
}
}  // namespace

TEST(SharedLibObjective, BadLibraryPath)
{
    EXPECT_THROW(generate_bad_library_and_do_nothing(), plato::functional::utilities::Exception);
}

TEST(SharedLibObjective, CallValue)
{
    namespace pfu = plato::functional::utilities;
    const auto tSharedLib = criteria::extension::SharedLibCriterion{std::string{kLibPath}, {}};

    constexpr std::string_view tMeshName = "massTest.exo";
    pfu::write_mesh(tMeshName, pfu::create_mesh("generated:1x1x1|bbox:-1,-1,-1,1,1,1"));
    const double tMass = tSharedLib.f(Plato::Functional::MeshProxy{tMeshName, {}});
    EXPECT_DOUBLE_EQ(tMass, 8.0);
}

TEST(SharedLibObjective, CallGradient)
{
    namespace pfu = plato::functional::utilities;
    const auto tSharedLib = criteria::extension::SharedLibCriterion{std::string{kLibPath}, {}};

    constexpr std::string_view tMeshName = "massTest.exo";
    pfu::write_mesh(tMeshName, pfu::create_mesh("generated:1x1x1|bbox:-1,-1,-1,1,1,1"));
    const auto tGrad = tSharedLib.df(Plato::Functional::MeshProxy{tMeshName, {}});

    const std::vector<double> tGold(24, 0);
    EXPECT_EQ(tGrad.stdVector(), tGold);
}

TEST(SharedLibObjective, ValueUsingFunction)
{
    namespace pfu = plato::functional::utilities;
    const auto tFunction = criteria::extension::make_shared_lib_function(
        criteria::extension::SharedLibCriterion{std::string{kLibPath}, {}});

    constexpr std::string_view tMeshName = "massTest.exo";
    pfu::write_mesh(tMeshName, pfu::create_mesh("generated:1x1x1|bbox:-1,-1,-1,1,1,1"));
    const double tMass = tFunction.f(Plato::Functional::MeshProxy{tMeshName, {}});
    EXPECT_DOUBLE_EQ(tMass, 8.0);
}
}  // namespace plato::functional::integration_tests::serial
