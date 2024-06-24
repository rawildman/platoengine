#include "plato/utilities/STKSearchUtilities.hpp"

#include <memory>
#include <numeric>
#include <optional>
#include <stk_mesh/base/MetaData.hpp>

#include "plato/utilities/STKUtilities.hpp"

namespace plato::utilities
{
namespace
{
const auto kDefaultSTKSearch = stk::search::KDTREE;

}  // namespace

SearchPoint convert_coordinate(const Coordinate& aCoordinate) { return {aCoordinate.x, aCoordinate.y, aCoordinate.z}; }

Coordinate convert_search_point(const SearchPoint& aSearchPoint)
{
    return {aSearchPoint.get_x_min(), aSearchPoint.get_y_min(), aSearchPoint.get_z_min()};
}

SearchResults find_points_in_sphere(const Coordinate aCenter,
                                    const double aSearchRadius,
                                    const std::vector<SearchPointWithIdentifier>& aLocalSearchPointWithIdentifiers,
                                    const boost::mpi::communicator& aCommunicator)
{
    const SearchSphere tSearchSphere = create_sphere(aCenter, STKRadius{aSearchRadius});
    const Identifier tSphereIdentifier{0, aCommunicator.rank()};
    const std::vector<SearchSphereWithIdentifier> tSearchDomain = {{tSearchSphere, tSphereIdentifier}};

    SearchResults tSearchResults;
    constexpr bool tEnforceSearchSymmetry = false;
    stk::search::coarse_search(tSearchDomain, aLocalSearchPointWithIdentifiers, kDefaultSTKSearch, aCommunicator,
                               tSearchResults, tEnforceSearchSymmetry);
    return tSearchResults;
}

SearchSphere create_sphere(const Coordinate& aCenter, const STKRadius aRadius)
{
    return SearchSphere{{aCenter.x, aCenter.y, aCenter.z}, aRadius.mValue};
}

std::vector<SearchPointWithIdentifier> search_points_with_identifiers(const std::vector<Coordinate>& aCoordinates)
{
    std::vector<SearchPointWithIdentifier> tSearchPointWithIdentifiers;
    int tIndex = -1;
    std::transform(
        aCoordinates.begin(), aCoordinates.end(), std::back_inserter(tSearchPointWithIdentifiers),
        [tIndex](const auto& iNode) mutable {
            return SearchPointWithIdentifier{SearchPoint{iNode.x, iNode.y, iNode.z}, Identifier{++tIndex, 0}};
        });
    return tSearchPointWithIdentifiers;
}

bool is_in_search_results(const Identifier aIdentifier, const utilities::SearchResults& aSearchResults)
{
    return std::any_of(aSearchResults.begin(), aSearchResults.end(),
                       [aIdentifier](auto tSearchResult) { return tSearchResult.second == aIdentifier; });
}

}  // namespace plato::utilities
