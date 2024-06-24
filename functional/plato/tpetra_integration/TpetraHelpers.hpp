#ifndef PLATO_TPETRAINTEGRATION_TPETRAHELPERS
#define PLATO_TPETRAINTEGRATION_TPETRAHELPERS

#include <Kokkos_Core_fwd.hpp>
#include <Teuchos_OrdinalTraits.hpp>
#include <Tpetra_Core.hpp>
#include <Tpetra_CrsMatrix.hpp>
#include <Tpetra_KokkosCompat_ClassicNodeAPI_Wrapper.hpp>
#include <Tpetra_Vector.hpp>

#include "plato/utilities/Vector3.hpp"

namespace plato::tpetra_integration
{
constexpr int kNumberOfCartesianDimensions = 3;

using TeuchosCommOrdinal = int;

using TpetraNode = Tpetra::KokkosCompat::KokkosDeviceWrapperNode<Kokkos::Serial, Kokkos::HostSpace>;
using TpetraLocalOrdinal = int;   // Tpetra::Vector<>::local_ordinal_type;
using TpetraGlobalOrdinal = int;  // Tpetra::Vector<>::global_ordinal_type;
using TpetraScalar = double;      // Tpetra::Vector<>::scalar_type;

using TpetraMap = Tpetra::Map<TpetraLocalOrdinal, TpetraGlobalOrdinal, TpetraNode>;
using TpetraVector = Tpetra::Vector<TpetraScalar, TpetraLocalOrdinal, TpetraGlobalOrdinal, TpetraNode>;

/// @brief use C-style 0 indexing in all the Tpetra objects
constexpr TpetraVector::global_ordinal_type kIndexBase = 0;

using TpetraMultiVector = Tpetra::MultiVector<TpetraScalar, TpetraLocalOrdinal, TpetraGlobalOrdinal, TpetraNode>;
using TpetraCRSMatrix = Tpetra::CrsMatrix<TpetraScalar, TpetraLocalOrdinal, TpetraGlobalOrdinal, TpetraNode>;

/// @brief Helper function to retrieve the number of local elements in the map @a aTeptraMap
[[nodiscard]] TpetraLocalOrdinal number_of_local_elements(const Teuchos::RCP<const TpetraMap>& aTpetraMap);

[[nodiscard]] std::pair<TpetraVector, TpetraVector> create_zeroed_row_and_column_vectors_from_crs_map(
    const TpetraCRSMatrix& aCRSMatrix, const Teuchos::RCP<const Teuchos::Comm<int>>& aCommunicator);

/// @brief Helper function to take a vector of double @a aSerialVector and insert it onto the distributed
/// vector @a aVector
void distribute_on_tpetra_vector(const std::vector<double>& aSerialVector, TpetraVector& aVector);

/// @brief Helper function to take a distributed vector @a aVector and reduce it to a non-distributed std vector of
/// double @a aSerialVector
[[nodiscard]] std::vector<double> reduce_tpetra_vector(const TpetraVector& aVector,
                                                       const Teuchos::RCP<const Teuchos::Comm<int>>& aCommunicator);

/// @brief Helper function to take a vector of Coordinates @a aSerialVector and insert  it onto the distributed
/// multivector @a aMultiVector
void distribute_on_tpetra_multivector(const std::vector<utilities::Coordinate>& aSerialVector,
                                      TpetraMultiVector& aMultiVector);

/// @brief Helper function to retrieve a Coordinate stored in @a aMultiVector at row @a aRow
[[nodiscard]] utilities::Coordinate multivector_coordinate(const TpetraMultiVector& aMultiVector,
                                                           const TpetraLocalOrdinal aRow);

}  // namespace plato::tpetra_integration

#endif
