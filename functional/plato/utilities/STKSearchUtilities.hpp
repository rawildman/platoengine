#ifndef PLATO_UTILITIES_STKSEARCHUTILITIES
#define PLATO_UTILITIES_STKSEARCHUTILITIES

#include <boost/mpi/communicator.hpp>
#include <filesystem>
#include <stk_mesh/base/BulkData.hpp>
#include <stk_search/Box.hpp>
#include <stk_search/CoarseSearch.hpp>
#include <stk_search/Point.hpp>
#include <vector>

#include "plato/utilities/NamedType.hpp"
#include "plato/utilities/STKVolumeUtilities.hpp"
#include "plato/utilities/Vector3.hpp"

namespace plato::utilities
{

using Identifier = stk::search::IdentProc<int, int>;
using SearchSphere = stk::search::Sphere<double>;
using SearchSphereWithIdentifier = std::pair<SearchSphere, Identifier>;
using SearchPoint = stk::search::Point<double>;
using SearchPointWithIdentifier = std::pair<SearchPoint, Identifier>;
using SearchResults = std::vector<std::pair<Identifier, Identifier>>;

using STKRadius = NamedType<double, struct STKRadiusTag>;

/// @brief Convert the plato functional coordinate object @a aCoordinate to a STK search point for use in stk searches
[[nodiscard]] SearchPoint convert_coordinate(const Coordinate& aCoordinate);

/// @brief Convert the stk search point @a aSearchPoint into a plato functional coordinate object
[[nodiscard]] Coordinate convert_search_point(const SearchPoint& aSearchPoint);

/// @brief Helper function that calls the STK search procedures on a specialized case of a single sphere with center @a
/// aCenter and radius @a aSearchRadius, on a specific rank @a aRank using the already populated vector of search points
/// with identifiers @a aLocalSearchPointsWithIdentifiers
[[nodiscard]] SearchResults find_points_in_sphere(
    const Coordinate aCenter,
    const double aSearchRadius,
    const std::vector<SearchPointWithIdentifier>& aLocalSearchPointWithIdentifiers,
    const boost::mpi::communicator& aCommunicator);

/// @brief Take a a center coordinate @a aCenter, and a strongly typed double @a aRadius to create a stk search sphere
/// object
[[nodiscard]] SearchSphere create_sphere(const Coordinate& aCenter, const STKRadius aRadius);

/// @brief Take a vector of coordinates @a aCoordinates and assign them simple identifiers assuming no additional ranks,
/// return a STK searchable object
[[nodiscard]] std::vector<SearchPointWithIdentifier> search_points_with_identifiers(
    const std::vector<Coordinate>& aCoordinates);

/// @brief Determine whether a specific identifier @a aIdentifier is within the search results @a aSearchResults
[[nodiscard]] bool is_in_search_results(const Identifier aIdentifier, const utilities::SearchResults& aSearchResults);

}  // namespace plato::utilities

#endif
