#include <gtest/gtest.h>
#include <variant>

#include "plato/core/InputVariantUtilities.hpp"

namespace plato::core::unittest
{
TEST(InputVariantUtilities, IsVariant)
{
    using TestVariant = std::variant<double, char, bool>;
    constexpr bool tDoubleInVariant = detail::kIsVariantMember<double, TestVariant>;
    EXPECT_TRUE(tDoubleInVariant);

    constexpr bool tIntInVariant = detail::kIsVariantMember<int, TestVariant>;
    EXPECT_FALSE(tIntInVariant);
}

TEST(InputVariantUtilities, MakeVariantWrongType)
{
    using TestVariant = std::variant<double, float, bool>;
    const std::optional<TestVariant> tResult = detail::to_variant<TestVariant>(42);
    EXPECT_FALSE(tResult.has_value());
}

TEST(InputVariantUtilities, MakeVariantRightTypeEmpty)
{
    using TestVariant = std::variant<double, float, bool>;
    using ValidOptional = boost::optional<double>;
    const std::optional<TestVariant> tResult = detail::to_variant<TestVariant>(ValidOptional{boost::none});
    EXPECT_FALSE(tResult.has_value());
}

TEST(InputVariantUtilities, MakeVariantRightTypeNotEmpty)
{
    using TestVariant = std::variant<double, float, bool>;
    using ValidOptional = boost::optional<double>;
    const std::optional<TestVariant> tResult = detail::to_variant<TestVariant>(ValidOptional{42.0});
    EXPECT_TRUE(tResult.has_value());
}

TEST(InputVariantUtilities, GeometryBlockAllEmpty)
{
    using TestVariant = std::variant<input_parser::density_topology, input_parser::brick_shape_geometry>;
    const std::optional<TestVariant> tGeometryInput =
        first_input_block_in_variant<TestVariant>(input_parser::ParsedInput{});
    EXPECT_FALSE(tGeometryInput.has_value());
}

TEST(InputVariantUtilities, GeometryBlockHasDensityTopology)
{
    using TestVariant = std::variant<input_parser::density_topology, input_parser::brick_shape_geometry>;
    auto tInput = input_parser::ParsedInput{};
    tInput.mDensityTopology.emplace();
    const std::optional<TestVariant> tGeometryInput = first_input_block_in_variant<TestVariant>(tInput);
    EXPECT_TRUE(tGeometryInput.has_value());
}

TEST(InputVariantUtilities, GeometryBlocksVector)
{
    using TestVariant = std::variant<input_parser::density_topology, input_parser::brick_shape_geometry>;
    {
        input_parser::ParsedInput tInput;
        EXPECT_EQ(all_input_blocks_in_variant<TestVariant>(tInput).size(), 0u);
    }
    {
        input_parser::ParsedInput tInput;
        tInput.mDensityTopology = input_parser::density_topology{};
        EXPECT_EQ(all_input_blocks_in_variant<TestVariant>(tInput).size(), 1u);
    }
    {
        input_parser::ParsedInput tInput;
        tInput.mDensityTopology = input_parser::density_topology{};
        tInput.mBrickShapeGeometry = input_parser::brick_shape_geometry{};
        EXPECT_EQ(all_input_blocks_in_variant<TestVariant>(tInput).size(), 2u);
    }
}

TEST(InputVariantUtilities, AllBlockNamesInVariant)
{
    using TestVariant = std::variant<input_parser::density_topology, input_parser::brick_shape_geometry>;
    const std::vector<std::string> tAllVariantBlockNames = all_variant_block_names<TestVariant>();
    ASSERT_EQ(tAllVariantBlockNames.size(), std::variant_size_v<TestVariant>);
    EXPECT_EQ(tAllVariantBlockNames.front(), input_parser::block_name<input_parser::density_topology>());
    EXPECT_EQ(tAllVariantBlockNames.back(), input_parser::block_name<input_parser::brick_shape_geometry>());
}

}  // namespace plato::core::unittest