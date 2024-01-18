#include <gtest/gtest.h>

#include "GeometryValidation.hpp"
#include "InputGeneration.hpp"
#include "ValidationRegistration.hpp"

namespace
{
std::optional<std::string> bogus_error(const Plato::density_topology& aInput)
{
    if (aInput.mesh_name.value().mName == "trigger_bogus_test")
    {
        return "Bogus error for test_geometry_block";
    }
    else
    {
        return std::nullopt;
    }
}

[[maybe_unused]] static auto kDensityTopologyValidationRegistration =
    Plato::Functional::Validation::Registration<Plato::density_topology>{[](const Plato::density_topology& aInput)
                                                                         { return bogus_error(aInput); }};

// create some registered tests on geometry to make sure the right test is called for the right variant
[[maybe_unused]] static auto kBrickValidationRegistration =
    Plato::Functional::Validation::Registration<Plato::brick_shape_geometry>{[](const Plato::brick_shape_geometry&)
                                                                             { return std::nullopt; }};
}  // namespace

TEST(GeometryValidation, InValidPlatoInputNoGeometry)
{
    const auto tInput = Plato::PlatoInput{};
    EXPECT_TRUE(Plato::Functional::Geometry::detail::validate_only_one_geometry(tInput).has_value());
}

TEST(GeometryValidation, ValidPlatoInputOneGeometry)
{
    auto tInput = Plato::PlatoInput{};
    tInput.mBrickShapeGeometry = Plato::brick_shape_geometry{};
    EXPECT_FALSE(Plato::Functional::Geometry::detail::validate_only_one_geometry(tInput).has_value());
}

TEST(GeometryValidation, InValidPlatoInputTwoGeometry)
{
    auto tInput = Plato::PlatoInput{};
    tInput.mBrickShapeGeometry = Plato::brick_shape_geometry{};
    tInput.mDensityTopology = Plato::density_topology{};
    EXPECT_TRUE(Plato::Functional::Geometry::detail::validate_only_one_geometry(tInput).has_value());
}

TEST(GeometryValidation, MeshName)
{
    namespace pf = Plato::Functional;
    auto tInput = Plato::PlatoInput{};
    tInput.mBrickShapeGeometry = pf::TestUtilities::create_valid_brick_shape_geometry();

    std::vector<std::string> tMessages;
    tMessages = pf::Geometry::validate_geometry(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 0u);
    tInput.mBrickShapeGeometry = Plato::brick_shape_geometry{};
    tMessages = pf::Geometry::validate_geometry(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 1u);
}

TEST(GeometryValidation, ValidInputCallsRightVariantTest)
{
    namespace pf = Plato::Functional;
    auto tInput = Plato::PlatoInput{};
    auto tDensityTopology = pf::TestUtilities::create_valid_density_topology_geometry();
    tDensityTopology.mesh_name = Plato::FileName{"trigger_bogus_test"};
    auto tBrickShapeGeometry = pf::TestUtilities::create_valid_brick_shape_geometry();
    std::vector<std::string> tMessages;

    tInput.mBrickShapeGeometry = tBrickShapeGeometry;
    tMessages = pf::Geometry::validate_geometry(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 0u);

    tInput.mBrickShapeGeometry = boost::none;
    tInput.mDensityTopology = tDensityTopology;
    tMessages = pf::Geometry::validate_geometry(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 1u);  // from bogus test geometry registration above

    tMessages.resize(0);
    tInput.mBrickShapeGeometry = tBrickShapeGeometry;  // now there are two geometries
    tMessages = pf::Geometry::validate_geometry(tInput, std::move(tMessages));
    EXPECT_EQ(tMessages.size(), 2u);  // from bogus test geometry registration above and multiple geometries
}
