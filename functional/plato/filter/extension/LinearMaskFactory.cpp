#include "plato/filter/extension/LinearMaskFactory.hpp"

#include <Teuchos_ArrayViewDecl.hpp>
#include <Teuchos_EReductionType.hpp>
#include <boost/mpi/collectives.hpp>
#include <set>

#include "plato/utilities/MultidimensionalRange.hpp"

namespace plato::filter::extension
{

namespace
{

constexpr bool kZeroOut = true;
}  // namespace

LinearMaskFactory::LinearMaskFactory(const NodalVector& aNodalCoordinates,
                                     const CenterVector& aCenters,
                                     const SearchRadius aSearchRadius,
                                     const int aMaximumConnectivityEstimate,
                                     const boost::mpi::communicator& aCommunicator)
    : mCommunicator(aCommunicator),
      mSearchRadius(aSearchRadius.mValue),
      mMaximumConnectivityEstimate(aMaximumConnectivityEstimate),
      mRowCenterCoordinates(aCenters.mValue),
      mNodalCoordinates(createNodalCoordinates(aNodalCoordinates.mValue)),
      mLocalSearchPointWithIdentifiers(detail::stk_search_points(mNodalCoordinates, mCommunicator.rank()))
{
    generateDistanceMap(static_cast<tpetra_integration::TpetraGlobalOrdinal>(aNodalCoordinates.mValue.size()));
}

tpetra_integration::TpetraMultiVector LinearMaskFactory::createNodalCoordinates(
    const std::vector<utilities::Coordinate>& aNodalCoordinates)
{
    const auto tCommunicator(Teuchos::rcp(new Teuchos::MpiComm<int>(mCommunicator)));
    auto tNodalCoordinates = tpetra_integration::TpetraMultiVector(
        Teuchos::rcp(
            new tpetra_integration::TpetraMap(aNodalCoordinates.size(), tpetra_integration::kIndexBase, tCommunicator)),
        tpetra_integration::kNumberOfCartesianDimensions, kZeroOut);
    tpetra_integration::distribute_on_tpetra_multivector(aNodalCoordinates, tNodalCoordinates);
    return tNodalCoordinates;
}

auto LinearMaskFactory::returnMask() const -> const tpetra_integration::TpetraCRSMatrix& { return *mLinearMask; }

auto LinearMaskFactory::generateRow(utilities::Coordinate aCenter)
    -> std::pair<std::vector<tpetra_integration::TpetraGlobalOrdinal>, std::vector<tpetra_integration::TpetraScalar>>
{
    const auto tSearchResults =
        utilities::find_points_in_sphere(aCenter, mSearchRadius, mLocalSearchPointWithIdentifiers, mCommunicator);

    constexpr bool tZeroOut = true;
    auto tRow = tpetra_integration::TpetraVector(mNodalCoordinates.getMap(), tZeroOut);

    double tSum = 0;
    for (const auto [tLocalIndex] :
         utilities::MultidimensionalRange{tpetra_integration::number_of_local_elements(mNodalCoordinates.getMap())})
    {
        const utilities::Identifier tLocalIdentifier{tLocalIndex, mCommunicator.rank()};
        if (utilities::is_in_search_results(tLocalIdentifier, tSearchResults))
        {
            const utilities::Coordinate tLocalCoordinate =
                tpetra_integration::multivector_coordinate(mNodalCoordinates, tLocalIndex);

            const double tDistance = utilities::magnitude(aCenter - tLocalCoordinate);
            const double tWeight{detail::linear_ramp_weight(Distance{tDistance}, SearchRadius{mSearchRadius})};

            const auto tGlobalID = mNodalCoordinates.getMap()->getGlobalElement(tLocalIndex);
            tRow.replaceGlobalValue(tGlobalID, tWeight);

            tSum += tWeight;
        }
    }
    double tGlobalSumOfWeights;
    const auto tCommunicator(Teuchos::rcp(new Teuchos::MpiComm<int>(mCommunicator)));
    Teuchos::reduceAll<int, double>(*tCommunicator, Teuchos::REDUCE_SUM, tSum, Teuchos::outArg(tGlobalSumOfWeights));

    return detail::normalize_nonzero_weights(tRow, detail::RowSum{tGlobalSumOfWeights},
                                             detail::EstimatedConnectivity{mMaximumConnectivityEstimate});
}

void LinearMaskFactory::generateDistanceMap(const tpetra_integration::TpetraGlobalOrdinal aNumberOfRows)
{
    const auto tCommunicator(Teuchos::rcp(new Teuchos::MpiComm<int>(mCommunicator)));
    auto tCrsRowMap = Teuchos::rcp(
        new tpetra_integration::TpetraMap(mRowCenterCoordinates.size(), tpetra_integration::kIndexBase, tCommunicator));
    auto tCrsDomainMap =
        Teuchos::rcp(new tpetra_integration::TpetraMap(aNumberOfRows, tpetra_integration::kIndexBase, tCommunicator));

    mLinearMask = Teuchos::RCP<tpetra_integration::TpetraCRSMatrix>{
        new tpetra_integration::TpetraCRSMatrix(tCrsRowMap, mMaximumConnectivityEstimate)};

    for (tpetra_integration::TpetraGlobalOrdinal tGlobalIndex = 0;
         tGlobalIndex < static_cast<tpetra_integration::TpetraGlobalOrdinal>(mRowCenterCoordinates.size());
         ++tGlobalIndex)
    {
        const utilities::Coordinate tGlobalCoordinate = mRowCenterCoordinates[tGlobalIndex];
        auto [tGlobalNonZeroIndices, tGlobalNonZeroWeights] = generateRow(tGlobalCoordinate);

        mLinearMask->insertGlobalValues(
            tGlobalIndex, Teuchos::ArrayView<tpetra_integration::TpetraGlobalOrdinal>(tGlobalNonZeroIndices),
            Teuchos::ArrayView<tpetra_integration::TpetraScalar>(tGlobalNonZeroWeights));
    }
    mLinearMask->fillComplete(tCrsDomainMap, tCrsRowMap);
}

namespace detail
{

double linear_ramp_weight(const Distance aDistance, const SearchRadius aSearchRadius)
{
    return std::max(0.0, 1.0 - aDistance.mValue / aSearchRadius.mValue);
}

auto normalize_nonzero_weights(const tpetra_integration::TpetraVector& aRowVector,
                               const RowSum aRowSum,
                               const EstimatedConnectivity aEstimatedConnectivity)
    -> std::pair<std::vector<tpetra_integration::TpetraGlobalOrdinal>, std::vector<tpetra_integration::TpetraScalar>>
{
    std::vector<tpetra_integration::TpetraScalar> tNonZeroWeightVector;
    std::vector<tpetra_integration::TpetraGlobalOrdinal> tNonZeroGlobalIndices;
    tNonZeroWeightVector.reserve(aEstimatedConnectivity.mValue);
    tNonZeroGlobalIndices.reserve(aEstimatedConnectivity.mValue);

    for (const auto [tLocalIndex] :
         utilities::MultidimensionalRange{tpetra_integration::number_of_local_elements(aRowVector.getMap())})
    {
        const tpetra_integration::TpetraScalar tReplacementWeight = aRowVector.getData()[tLocalIndex] / aRowSum.mValue;
        if (tReplacementWeight > 0)
        {
            const auto tGlobalID = aRowVector.getMap()->getGlobalElement(tLocalIndex);
            tNonZeroGlobalIndices.push_back(tGlobalID);
            tNonZeroWeightVector.push_back(tReplacementWeight);
        }
    }
    return {tNonZeroGlobalIndices, tNonZeroWeightVector};
}

std::vector<utilities::SearchPointWithIdentifier> stk_search_points(
    const tpetra_integration::TpetraMultiVector& aNodalCoordinates, const int aRank)
{
    std::vector<utilities::SearchPointWithIdentifier> tLocalSearchPointWithIdentifiers(
        tpetra_integration::number_of_local_elements(aNodalCoordinates.getMap()));

    for (const auto [tLocalIndex] :
         utilities::MultidimensionalRange{tpetra_integration::number_of_local_elements(aNodalCoordinates.getMap())})
    {
        const utilities::Identifier tIdentifier{tLocalIndex, aRank};
        const utilities::Coordinate tCoordinate =
            tpetra_integration::multivector_coordinate(aNodalCoordinates, tLocalIndex);
        tLocalSearchPointWithIdentifiers[tLocalIndex] =
            utilities::SearchPointWithIdentifier({utilities::convert_coordinate(tCoordinate), tIdentifier});
    }
    return tLocalSearchPointWithIdentifiers;
}

}  // namespace detail

}  // namespace plato::filter::extension
