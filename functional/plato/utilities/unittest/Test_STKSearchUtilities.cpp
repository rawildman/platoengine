#include <gtest/gtest.h>

#include <boost/mpi/communicator.hpp>
#include <optional>

#include "plato/test_utilities/TestDataFilePath.hpp"
#include "plato/utilities/CoordinateTestUtilities.hpp"
#include "plato/utilities/STKCommandGenerator.hpp"
#include "plato/utilities/STKSearchUtilities.hpp"
#include "plato/utilities/STKUtilities.hpp"
#include "plato/utilities/Vector3.hpp"

namespace plato::utilities::unittest
{

TEST(STKSearchUtilities, NodalIdentifiers)
{
    const std::vector<Coordinate> tNodalCoordinates{{0, 0, 0}, {1, 0, 0}, {2, 0, 0}, {3, 0, 0}, {4, 0, 0}};

    std::vector<SearchPointWithIdentifier> tNodalCoordsWithIdentifiers =
        search_points_with_identifiers(tNodalCoordinates);

    ASSERT_EQ(tNodalCoordinates.size(), tNodalCoordsWithIdentifiers.size());
    for (unsigned int tIndex = 0; tIndex < tNodalCoordinates.size(); ++tIndex)
    {
        const Coordinate tResult = convert_search_point(tNodalCoordsWithIdentifiers[tIndex].first);
        test_double_equality_of_components(tResult, tNodalCoordinates[tIndex],
                                           TEST_CONTEXT("Checking search point is node"));
        EXPECT_EQ(tNodalCoordsWithIdentifiers[tIndex].second.id(), tIndex);
    }
}

TEST(STKSearchUtilities, IsInSearchResults)
{
    const std::vector<Coordinate> tNodalCoordinates{{0, 0, 0}, {1, 0, 0}, {2, 0, 0}, {3, 0, 0}, {4, 0, 0}};
    std::vector<SearchPointWithIdentifier> tNodalCoordsWithIdentifiers =
        search_points_with_identifiers(tNodalCoordinates);

    constexpr Coordinate tCenter{0, 0, 0};
    constexpr double tSearchRadius{2};
    const auto tSearchResults =
        find_points_in_sphere(tCenter, tSearchRadius, tNodalCoordsWithIdentifiers, boost::mpi::communicator{});

    EXPECT_TRUE(is_in_search_results(tNodalCoordsWithIdentifiers[0].second, tSearchResults));
    EXPECT_TRUE(is_in_search_results(tNodalCoordsWithIdentifiers[1].second, tSearchResults));
    EXPECT_TRUE(is_in_search_results(tNodalCoordsWithIdentifiers[2].second, tSearchResults));
    EXPECT_FALSE(is_in_search_results(tNodalCoordsWithIdentifiers[3].second, tSearchResults));
    EXPECT_FALSE(is_in_search_results(tNodalCoordsWithIdentifiers[4].second, tSearchResults));
}

TEST(STKSearchUtilities, FindPointsInSphere)
{
    const boost::mpi::communicator tCommunicator{};
    constexpr Coordinate tCenter{1, 0, 0};
    constexpr double tSearchRadius{2.1};
    const auto tThisRank = tCommunicator.rank();

    const std::vector<SearchPointWithIdentifier> tLocalSearchPointWithIdentifiers{
        SearchPointWithIdentifier{{1, 0, 0}, Identifier{0, tThisRank}},
        SearchPointWithIdentifier{{2, 0, 0}, Identifier{1, tThisRank}},
        SearchPointWithIdentifier{{2, 2, 2}, Identifier{2, tThisRank}},
        SearchPointWithIdentifier{{0, 0, 0}, Identifier{3, tThisRank}}};

    const auto tSearchResults =
        find_points_in_sphere(tCenter, tSearchRadius, tLocalSearchPointWithIdentifiers, tCommunicator);

    EXPECT_EQ(tSearchResults.size(), 3u);
}

}  // namespace plato::utilities::unittest
