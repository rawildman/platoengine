#include <gtest/gtest.h>

#include <filesystem>

#include "plato/core/MeshProxy.hpp"
#include "plato/criteria/extension/SharedLibCriterion.hpp"
#include "plato/integration_tests/utilities/AppConfigurationTestUtilities.hpp"
#include "plato/utilities/Exception.hpp"
#include "plato/utilities/STKCommandGenerator.hpp"
#include "plato/utilities/STKUtilities.hpp"

namespace plato::integration_tests::serial
{
namespace
{
// These tests use the mass objective library but through the shared library interface so they are testing the shared
// library more generically
constexpr std::string_view kLibPath = "libPlatoTestMassObjective.so";

const auto kMeshGenerator = plato::utilities::STKCommandGenerator{
    {1, 1, 1}, {-1, -1, -1}, {1, 1, 1}, plato::utilities::STKCommandElementType::Hex};

void generate_bad_library_and_do_nothing()
{
    // This function should throw an exception
    const auto tTestConfiguration = utilities::test_app_configuration("badRobot.so");
    const auto tBad = criteria::extension::SharedLibCriterion{
        tTestConfiguration, tTestConfiguration.mConfiguration.mCriteria.front(), {}};
    std::cout << tBad.f(core::MeshProxy{"dne.exo", {}}) << std::endl;
}

criteria::extension::SharedLibCriterion test_shared_lib_criterion()
{
    const auto tTestConfiguration = utilities::test_app_configuration(kLibPath);
    return criteria::extension::SharedLibCriterion{
        tTestConfiguration, tTestConfiguration.mConfiguration.mCriteria.front(), {}};
}
}  // namespace

TEST(SharedLibObjective, BadLibraryPath)
{
    EXPECT_THROW(generate_bad_library_and_do_nothing(), plato::utilities::Exception);
}

TEST(SharedLibObjective, CallValue)
{
    namespace pfu = plato::utilities;
    const auto tSharedLib = test_shared_lib_criterion();

    constexpr std::string_view tMeshName = "massTest.exo";
    pfu::write_mesh(tMeshName, pfu::generate_stk_mesh(kMeshGenerator));
    const double tMass = tSharedLib.f(core::MeshProxy{tMeshName, {}});
    EXPECT_DOUBLE_EQ(tMass, 8.0);

    std::filesystem::remove(tMeshName);
}

TEST(SharedLibObjective, CallGradient)
{
    namespace pfu = plato::utilities;
    const auto tSharedLib = test_shared_lib_criterion();

    constexpr std::string_view tMeshName = "massTest.exo";
    pfu::write_mesh(tMeshName, pfu::generate_stk_mesh(kMeshGenerator));
    const auto tGrad = tSharedLib.df(core::MeshProxy{tMeshName, {}});

    const std::vector<double> tGold(24, 1.0);
    EXPECT_EQ(tGrad.stdVector(), tGold);

    std::filesystem::remove(tMeshName);
}

TEST(SharedLibObjective, ValueUsingFunction)
{
    namespace pfu = plato::utilities;
    const auto tFunction = criteria::extension::make_shared_lib_function(test_shared_lib_criterion());

    constexpr std::string_view tMeshName = "massTest.exo";
    pfu::write_mesh(tMeshName, pfu::generate_stk_mesh(kMeshGenerator));
    const double tMass = tFunction.f(core::MeshProxy{tMeshName, {}});
    EXPECT_DOUBLE_EQ(tMass, 8.0);

    std::filesystem::remove(tMeshName);
}
}  // namespace plato::integration_tests::serial
