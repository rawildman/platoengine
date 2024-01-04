#include <gtest/gtest.h>

#include "detail/GeometryRegistrationUtilities.hpp"

TEST(GeometryRegistrationUtilities, IsVariant)
{
    namespace pfgd = Plato::Functional::GeometryFactory::Detail;

    using TestVariant = std::variant<double, char, bool>;
    constexpr bool tDoubleInVariant = pfgd::kIsVariantMember<double, TestVariant>;
    EXPECT_TRUE(tDoubleInVariant);

    constexpr bool tIntInVariant = pfgd::kIsVariantMember<int, TestVariant>;
    EXPECT_FALSE(tIntInVariant);
}

TEST(GeometryRegistrationUtilities, MakeVariantWrongType)
{
    namespace pfg = Plato::Functional::GeometryFactory;
    namespace pfgd = Plato::Functional::GeometryFactory::Detail;
    const std::optional<pfg::GeometryInput> tResult = pfgd::make_variant_if_geometry(42);
    EXPECT_FALSE(tResult.has_value());
}

TEST(GeometryRegistrationUtilities, MakeVariantRightTypeEmpty)
{
    namespace pfg = Plato::Functional::GeometryFactory;
    namespace pfgd = Plato::Functional::GeometryFactory::Detail;
    using ValidOptional = boost::optional<Plato::density_topology>;
    const std::optional<pfg::GeometryInput> tResult = pfgd::make_variant_if_geometry(ValidOptional{boost::none});
    EXPECT_FALSE(tResult.has_value());
}

TEST(GeometryRegistrationUtilities, MakeVariantRightTypeNotEmpty)
{
    namespace pfg = Plato::Functional::GeometryFactory;
    namespace pfgd = Plato::Functional::GeometryFactory::Detail;
    using ValidOptional = boost::optional<Plato::density_topology>;
    const std::optional<pfg::GeometryInput> tResult =
        pfgd::make_variant_if_geometry(ValidOptional{Plato::density_topology{}});
    EXPECT_TRUE(tResult.has_value());
}

TEST(GeometryRegistrationUtilities, GeometryBlockAllEmpty)
{
    namespace pfg = Plato::Functional::GeometryFactory;
    namespace pfgd = Plato::Functional::GeometryFactory::Detail;
    const std::optional<pfg::GeometryInput> tGeometryInput = pfgd::first_geometry_block(Plato::PlatoInput{});
    EXPECT_FALSE(tGeometryInput.has_value());
}

TEST(GeometryRegistrationUtilities, GeometryBlockHasDensityTopology)
{
    namespace pfg = Plato::Functional::GeometryFactory;
    namespace pfgd = Plato::Functional::GeometryFactory::Detail;
    auto tInput = Plato::PlatoInput{};
    tInput.mDensityTopology.emplace();
    const std::optional<pfg::GeometryInput> tGeometryInput = pfgd::first_geometry_block(tInput);
    EXPECT_TRUE(tGeometryInput.has_value());
}

TEST(GeometryRegistrationUtilities, GeometryInputAllEmpty)
{
    namespace pfg = Plato::Functional::GeometryFactory;
    namespace pfgd = Plato::Functional::GeometryFactory::Detail;
    EXPECT_THROW(auto tGeometryInput = pfgd::first_geometry_input(Plato::PlatoInput{}), Plato::Functional::Exception);
}

TEST(GeometryRegistrationUtilities, GeometryInputHasDensityTopology)
{
    namespace pfg = Plato::Functional::GeometryFactory;
    namespace pfgd = Plato::Functional::GeometryFactory::Detail;
    auto tInput = Plato::PlatoInput{};
    tInput.mDensityTopology.emplace();
    EXPECT_NO_THROW(auto tGeometryInput = pfgd::first_geometry_block(tInput));
}

TEST(GeometryRegistrationUtilities, BlockName)
{
    namespace pf = Plato::Functional;
    namespace pfg = pf::GeometryFactory;
    {
        const auto tGeometryInput = pfg::GeometryInput{Plato::density_topology{}};
        EXPECT_EQ(pfg::Detail::block_name(tGeometryInput), "density_topology");
    }  // namespace Plato::Functional::GeometryFactory;
    {
        const auto tGeometryInput = pfg::GeometryInput{Plato::brick_shape_geometry{}};
        EXPECT_EQ(pfg::Detail::block_name(tGeometryInput), "brick_shape_geometry");
    }
}

TEST(GeometryRegistrationUtilities, GeometryBlocksVector)
{
    namespace pfg = Plato::Functional::GeometryFactory;

    {
        Plato::PlatoInput tInput;
        EXPECT_EQ(pfg::Detail::geometry_blocks(tInput).size(), 0u);
    }
    {
        Plato::PlatoInput tInput;
        tInput.mDensityTopology = Plato::density_topology{};
        EXPECT_EQ(pfg::Detail::geometry_blocks(tInput).size(), 1u);
    }
    {
        Plato::PlatoInput tInput;
        tInput.mDensityTopology = Plato::density_topology{};
        tInput.mBrickShapeGeometry = Plato::brick_shape_geometry{};
        EXPECT_EQ(pfg::Detail::geometry_blocks(tInput).size(), 2u);
    }
}
