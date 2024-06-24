#include <gtest/gtest.h>

#include <optional>
#include <string_view>

#include "plato/utilities/STKCommandGenerator.hpp"

namespace plato::utilities::unittest
{

TEST(STKCommandGenerator, Default)
{
    const STKCommandGenerator tSTKCommandGenerator;
    constexpr std::string_view tGold{"generated:1x1x1|bbox:0,0,0,1,1,1"};
    EXPECT_EQ(tSTKCommandGenerator.toString(), tGold);
    EXPECT_DOUBLE_EQ(tSTKCommandGenerator.volume(), 1.0);
    EXPECT_EQ(tSTKCommandGenerator.numberOfElements(), 1u);
    EXPECT_EQ(tSTKCommandGenerator.numberOfNodes(), 8u);
}

TEST(STKCommandGenerator, CustomHexBox)
{
    const STKCommandBounds tLowerBounds{-1, -2, -3};
    const STKCommandBounds tUpperBounds{1, 2, 3};
    const STKCommandNumberOfElements tElements{3, 4, 1};
    const auto tSTKCommandGenerator =
        STKCommandGenerator{tElements, tLowerBounds, tUpperBounds, STKCommandElementType::Hex};
    constexpr std::string_view tGold{"generated:3x4x1|bbox:-1,-2,-3,1,2,3"};
    EXPECT_EQ(tSTKCommandGenerator.toString(), tGold);
    EXPECT_DOUBLE_EQ(tSTKCommandGenerator.volume(), 2 * 4 * 6);
    EXPECT_EQ(tSTKCommandGenerator.numberOfElements(), 3u * 4u * 1u);
    EXPECT_EQ(tSTKCommandGenerator.numberOfNodes(), 40u);
}

TEST(STKCommandGenerator, CustomTetBox)
{
    const STKCommandBounds tLowerBounds{0, 0, 0};
    const STKCommandBounds tUpperBounds{4, 4, 4};
    const STKCommandNumberOfElements tElements{2, 2, 2};
    const auto tSTKCommandGenerator =
        STKCommandGenerator{tElements, tLowerBounds, tUpperBounds, STKCommandElementType::Tet};
    constexpr std::string_view tGold{"generated:2x2x2|bbox:0,0,0,4,4,4|tets"};
    EXPECT_EQ(tSTKCommandGenerator.toString(), tGold);
    EXPECT_DOUBLE_EQ(tSTKCommandGenerator.volume(), 4 * 4 * 4);
    EXPECT_EQ(tSTKCommandGenerator.numberOfElements(), 2u * 2u * 2u * 6u);
    EXPECT_EQ(tSTKCommandGenerator.numberOfNodes(), 27u);
}

TEST(STKCommandGenerator, CustomHexWithPrecision)
{
    const STKCommandBounds tLowerBounds{0.12345678901234567, 0.12345678901234567, 0.12345678901234567};
    const STKCommandBounds tUpperBounds{4.12345678901234567, 4.12345678901234567, 4.12345678901234567};
    const STKCommandNumberOfElements tElements{2, 2, 2};
    const auto tSTKCommandGenerator =
        STKCommandGenerator{tElements, tLowerBounds, tUpperBounds, STKCommandElementType::Tet, {}, {}, 16};
    constexpr std::string_view tGold{
        "generated:2x2x2|bbox:0.1234567890123457,0.1234567890123457,0.1234567890123457,4.123456789012345,4."
        "123456789012345,4.123456789012345|tets"};
    EXPECT_EQ(tSTKCommandGenerator.toString(), tGold);
    EXPECT_DOUBLE_EQ(tSTKCommandGenerator.volume(), 4 * 4 * 4);
    EXPECT_EQ(tSTKCommandGenerator.numberOfElements(), 2u * 2u * 2u * 6u);
    EXPECT_EQ(tSTKCommandGenerator.numberOfNodes(), 27u);
}

TEST(STKCommandGenerator, SideSets)
{
    const STKCommandBounds tLowerBounds{0, 0, 0};
    const STKCommandBounds tUpperBounds{1, 1, 1};
    const STKCommandNumberOfElements tElements{1, 1, 1};
    const auto tSTKCommandGenerator = STKCommandGenerator{tElements,
                                                          tLowerBounds,
                                                          tUpperBounds,
                                                          STKCommandElementType::Hex,
                                                          {false, false, false, false, false, false},
                                                          {true, true, true, true, true, true}};
    constexpr std::string_view tGold{"generated:1x1x1|bbox:0,0,0,1,1,1|sideset:xXyYzZ"};
    EXPECT_EQ(tSTKCommandGenerator.toString(), tGold);
}

TEST(STKCommandGenerator, NodeSets)
{
    const STKCommandBounds tLowerBounds{0, 0, 0};
    const STKCommandBounds tUpperBounds{1, 1, 1};
    const STKCommandNumberOfElements tElements{1, 1, 1};
    const auto tSTKCommandGenerator = STKCommandGenerator{tElements,
                                                          tLowerBounds,
                                                          tUpperBounds,
                                                          STKCommandElementType::Hex,
                                                          {true, true, true, true, true, true},
                                                          {false, false, false, false, false, false}};

    constexpr std::string_view tGold{"generated:1x1x1|bbox:0,0,0,1,1,1|nodeset:xXyYzZ"};
    EXPECT_EQ(tSTKCommandGenerator.toString(), tGold);
}

TEST(STKCommandGeneratorDetail, xXyYzZ)
{
    {
        STKNodeSetSideSetIdentifiers tSTKNodeSetSideSetIdentifiers;
        constexpr std::string_view tGold{""};
        EXPECT_EQ(detail::xyz_boundary_string(tSTKNodeSetSideSetIdentifiers), tGold);
    }
    {
        STKNodeSetSideSetIdentifiers tSTKNodeSetSideSetIdentifiers;
        tSTKNodeSetSideSetIdentifiers.mLowerX = true;

        constexpr std::string_view tGold{"x"};
        EXPECT_EQ(detail::xyz_boundary_string(tSTKNodeSetSideSetIdentifiers), tGold);
    }
    {
        STKNodeSetSideSetIdentifiers tSTKNodeSetSideSetIdentifiers;
        tSTKNodeSetSideSetIdentifiers.mLowerX = true;
        tSTKNodeSetSideSetIdentifiers.mUpperX = true;

        constexpr std::string_view tGold{"xX"};
        EXPECT_EQ(detail::xyz_boundary_string(tSTKNodeSetSideSetIdentifiers), tGold);
    }
    {
        STKNodeSetSideSetIdentifiers tSTKNodeSetSideSetIdentifiers{false, false, true, false, false, true};
        constexpr std::string_view tGold{"yZ"};
        EXPECT_EQ(detail::xyz_boundary_string(tSTKNodeSetSideSetIdentifiers), tGold);
    }
    {
        STKNodeSetSideSetIdentifiers tSTKNodeSetSideSetIdentifiers{true, true, true, true, true, true};
        constexpr std::string_view tGold{"xXyYzZ"};
        EXPECT_EQ(detail::xyz_boundary_string(tSTKNodeSetSideSetIdentifiers), tGold);
    }
    {
        STKNodeSetSideSetIdentifiers tSTKNodeSetSideSetIdentifiers{true, false, true, false, true, false};
        constexpr std::string_view tGold{"xyz"};
        EXPECT_EQ(detail::xyz_boundary_string(tSTKNodeSetSideSetIdentifiers), tGold);
    }
}

}  // namespace plato::utilities::unittest
