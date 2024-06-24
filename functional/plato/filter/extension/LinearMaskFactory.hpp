#ifndef PLATO_FILTER_EXTENSION_LINEARMASKFACTORY
#define PLATO_FILTER_EXTENSION_LINEARMASKFACTORY

#include <boost/mpi/communicator.hpp>
#include <optional>

#include "plato/tpetra_integration/TpetraHelpers.hpp"
#include "plato/utilities/NamedType.hpp"
#include "plato/utilities/STKSearchUtilities.hpp"
#include "plato/utilities/Vector3.hpp"

namespace plato::filter::extension
{

using SearchRadius = utilities::NamedType<double, struct SearchRadiusTag>;
using Distance = utilities::NamedType<double, struct DistanceTag>;

using NodalVector = utilities::NamedType<std::vector<utilities::Coordinate>, struct NodalVectorTag>;
using CenterVector = utilities::NamedType<std::vector<utilities::Coordinate>, struct CenterVectorTag>;

/// @brief A mask generation class for the Kernel filter.
/// The intent is that this object gets created during some initialization phase and does not get updated thereafter.
class LinearMaskFactory
{
   public:
    /// @brief constructor for the distance mask.
    /// @a aNodalCoordinates is a vector of nodal coordinates, presumably from the mesh directly.
    /// @a aCenters is a vector of the element centroids if it is element centered or nodes if it is nodal.
    /// @a aSearchRadius is the search radius the distance map will be calculated over. It is used in a STK search as
    /// well as the linear function to determine the weight.
    /// @a aMaximumConnectivityEstimate is an estimate provided to the Tpetra CRS Matrix during allocation. It should be
    /// a maximum expected to avoid any additional allocation time.
    LinearMaskFactory(const NodalVector& aNodalCoordinates,
                      const CenterVector& aCenters,
                      const SearchRadius aSearchRadius,
                      const int aMaximumConnectivityEstimate,
                      const boost::mpi::communicator& aCommunicator);

    /// @brief Return a pointer to the distance mask.
    [[nodiscard]] auto returnMask() const -> const tpetra_integration::TpetraCRSMatrix&;

   private:
    /// @brief create nodal coordinate tpetra container of  @a aNodalCoordinates
    tpetra_integration::TpetraMultiVector createNodalCoordinates(
        const std::vector<utilities::Coordinate>& aNodalCoordinates);

    /// @brief Perform a parallel computation of the row calculation assuming some center @a aCenter.
    ///  Return a pair of vectors one with the global ordinals and the other the corresponding normalized weights.
    ///  An individual row should sum to 1.
    [[nodiscard]] auto generateRow(utilities::Coordinate aCenter)
        -> std::pair<std::vector<tpetra_integration::TpetraGlobalOrdinal>,
                     std::vector<tpetra_integration::TpetraScalar>>;

    /// @brief Perform the calculation of the full NxM distance map.
    void generateDistanceMap(const tpetra_integration::TpetraGlobalOrdinal aNumberOfNodalCoordinates);

    boost::mpi::communicator mCommunicator;

    double mSearchRadius = 1;
    int mMaximumConnectivityEstimate = 1;

    std::vector<utilities::Coordinate> mRowCenterCoordinates;
    tpetra_integration::TpetraMultiVector mNodalCoordinates;

    Teuchos::RCP<tpetra_integration::TpetraCRSMatrix> mLinearMask;

    std::vector<utilities::SearchPointWithIdentifier> mLocalSearchPointWithIdentifiers;
};

namespace detail
{

using RowSum = utilities::NamedType<double, struct RowSumTag>;
using EstimatedConnectivity = utilities::NamedType<int, struct EstimatedConnectivityTag>;

/// @brief Computes the linear tophat function based on a distance @a aDistance and a search radius @a aSearchRadius
[[nodiscard]] double linear_ramp_weight(const Distance aDistance, const SearchRadius aSearchRadius);

/// @brief Helper function that takes a tpetra vector @a RowVector and its sum of values @a aRowSum, and normalizes and
/// populates the non-zero weights into a pair of vectors storing the global indices and weights.
[[nodiscard]] auto normalize_nonzero_weights(const tpetra_integration::TpetraVector& aRowVector,
                                             const RowSum aRowSum,
                                             const EstimatedConnectivity aEstimatedConnectivity)
    -> std::pair<std::vector<tpetra_integration::TpetraGlobalOrdinal>, std::vector<tpetra_integration::TpetraScalar>>;

/// @brief Helper function to take a multivector @a aNodalCoordinates and a given rank @a aRank will generate the STK
/// formatted Search point equal to the node and assigned a local id and rank.
[[nodiscard]] std::vector<utilities::SearchPointWithIdentifier> stk_search_points(
    const tpetra_integration::TpetraMultiVector& aNodalCoordinates, const int aRank);

}  // namespace detail

}  // namespace plato::filter::extension

#endif
