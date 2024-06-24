#ifndef PLATO_FILTER_EXTENSION_LINEARMASK
#define PLATO_FILTER_EXTENSION_LINEARMASK

#include <boost/mpi/communicator.hpp>
#include <optional>

#include "plato/filter/extension/LinearMaskFactory.hpp"
#include "plato/tpetra_integration/TpetraHelpers.hpp"
#include "plato/utilities/NamedType.hpp"
#include "plato/utilities/STKSearchUtilities.hpp"
#include "plato/utilities/Vector3.hpp"

namespace plato::filter::extension
{

/// @brief A mask generation class for the Kernel filter.
/// The intent is that this object gets created during some initialization phase and does not get updated thereafter.
class LinearMask
{
   public:
    /// @brief Constructor for the distance mask using element centroids.
    /// @a aNodalCoordinates is a vector of nodal coordinates
    /// @a aCentroids is a vector of the element centroids.
    /// @a aSearchRadius is the search radius the distance map will be calculated over. It is used in a STK search as
    /// well as the linear function to determine the weight.
    /// @a aMaximumConnectivityEstimate is an estimate provided to the Tpetra CRS Matrix during allocation. It should be
    /// a maximum expected to avoid any additional allocation time.
    LinearMask(const NodalVector& aNodalCoordinates,
               const CenterVector& aCentroids,
               const SearchRadius aSearchRadius,
               const int aMaximumConnectivityEstimate,
               const boost::mpi::communicator& aCommunicator);

    /// @brief Node centered constructor for the distance mask.
    /// @a aNodalCoordinates is a vector of nodal coordinates, presumably from the mesh directly.
    /// @a aSearchRadius is the search radius the distance map will be calculated over. It is used in a STK search as
    /// well as the linear function to determine the weight.
    /// @a aMaximumConnectivityEstimate is an estimate provided to the Tpetra CRS Matrix during allocation. It should be
    /// a maximum expected to avoid any additional allocation time.
    LinearMask(const NodalVector& aNodalCoordinates,
               const SearchRadius aSearchRadius,
               const int aMaximumConnectivityEstimate,
               const boost::mpi::communicator& aCommunicator);

    /// @brief Return the size of the distance mask.
    [[nodiscard]] auto size() const
        -> std::pair<tpetra_integration::TpetraGlobalOrdinal, tpetra_integration::TpetraGlobalOrdinal>;

    [[nodiscard]] std::vector<double> matrixMultiply(const std::vector<double>& aValues) const;
    [[nodiscard]] std::vector<double> transposeMatrixMultiply(const std::vector<double>& aValues) const;

   private:
    boost::mpi::communicator mCommunicator;
    tpetra_integration::TpetraCRSMatrix mLinearMask;
};

}  // namespace plato::filter::extension

#endif
