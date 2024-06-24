#include <gtest/gtest.h>

#include <boost/mpi/communicator.hpp>
#include <optional>

#include "plato/filter/extension/LinearMaskFactory.hpp"
#include "plato/filter/extension/parallel_unittest/LinearMaskTestUtility.hpp"
#include "plato/utilities/CoordinateTestUtilities.hpp"
#include "plato/utilities/STKCommandGenerator.hpp"
#include "plato/utilities/STKSearchUtilities.hpp"  //element_bounding_box , FloatSphere, SearchResults
#include "plato/utilities/STKUtilities.hpp"        //generate_stk_mesh, element_vector
#include "plato/utilities/Vector3.hpp"

namespace plato::filter::extension::unittest
{

namespace
{

constexpr auto kNumRanks = int{4};
constexpr int kNumSpatialDimensions{3};

[[nodiscard]] Teuchos::RCP<const tpetra_integration::TpetraMap> create_contiguous_map(
    const Tpetra::global_size_t aSize, Teuchos::RCP<const Teuchos::Comm<int>> aCommunicator)
{
    return Teuchos::rcp(
        new tpetra_integration::TpetraMap(aSize, tpetra_integration::kIndexBase, std::move(aCommunicator)));
}

[[nodiscard]] tpetra_integration::TpetraVector create_projection_vector(
    const size_t aSize, const tpetra_integration::TpetraGlobalOrdinal aGlobalIndex)
{
    const auto tCommunicator = Tpetra::getDefaultComm();
    const auto tContiguousMap = create_contiguous_map(aSize, tCommunicator);
    constexpr bool tZeroOut = true;
    auto tVector = tpetra_integration::TpetraVector(tContiguousMap, tZeroOut);
    tVector.replaceGlobalValue(aGlobalIndex, 1);
    return tVector;
}

[[nodiscard]] tpetra_integration::TpetraScalar get_entry(const tpetra_integration::TpetraCRSMatrix& aMatrix,
                                                         const tpetra_integration::TpetraGlobalOrdinal aGlobalIndexI,
                                                         const tpetra_integration::TpetraGlobalOrdinal aGlobalIndexJ)
{
    const tpetra_integration::TpetraGlobalOrdinal tNRows = aMatrix.getGlobalNumRows();
    const tpetra_integration::TpetraGlobalOrdinal tMColumns = aMatrix.getGlobalNumCols();
    const auto tCommunicator = Tpetra::getDefaultComm();
    const auto tContiguousMap = create_contiguous_map(tNRows, tCommunicator);
    constexpr bool tZeroOut = true;
    auto tResult = tpetra_integration::TpetraVector(tContiguousMap, tZeroOut);

    const auto tProjectionI = create_projection_vector(tNRows, aGlobalIndexI);
    const auto tProjectionJ = create_projection_vector(tMColumns, aGlobalIndexJ);
    aMatrix.apply(tProjectionJ, tResult);
    return tProjectionI.dot(tResult);
}

}  // namespace

TEST(LinearMaskFactoryDetail, MPISize)
{
    const auto tComm = boost::mpi::communicator{};
    EXPECT_EQ(tComm.size(), kNumRanks);
}

TEST(LinearMaskFactoryDetail, LinearRamp)
{
    constexpr SearchRadius tSearchRadius{4};

    // closest point
    {
        constexpr Distance tDistance{0};
        constexpr double tGold = 1;
        const double tResult = detail::linear_ramp_weight(tDistance, tSearchRadius);
        EXPECT_EQ(tResult, tGold);
    }

    // outside radius
    {
        constexpr Distance tDistance{10};
        constexpr double tGold = 0;
        const double tResult = detail::linear_ramp_weight(tDistance, tSearchRadius);
        EXPECT_EQ(tResult, tGold);
    }
    // equal to radius
    {
        constexpr Distance tDistance{tSearchRadius.mValue};
        constexpr double tGold = 0;
        const double tResult = detail::linear_ramp_weight(tDistance, tSearchRadius);
        EXPECT_EQ(tResult, tGold);
    }
    // equal to half radius
    {
        constexpr Distance tDistance{tSearchRadius.mValue / 2.0};
        constexpr double tGold = 0.5;
        const double tResult = detail::linear_ramp_weight(tDistance, tSearchRadius);
        EXPECT_DOUBLE_EQ(tResult, tGold);
    }
}

TEST(LinearMaskFactoryDetail, ReturnNormalizedNonzeroWeights)
{
    const auto tCommunicator = Tpetra::getDefaultComm();
    const Tpetra::global_size_t tMapSize = 8;
    const int tEstimatedConnectivity = tMapSize / kNumRanks;
    const auto tContiguousMap = create_contiguous_map(tMapSize, tCommunicator);
    constexpr bool tZeroOut = true;
    tpetra_integration::TpetraVector tVector = tpetra_integration::TpetraVector(tContiguousMap, tZeroOut);
    constexpr double tRowSum = tMapSize * 1.0;
    {
        const auto [tGlobalNonZeroIndices, tLocalNonZeroWeights] = detail::normalize_nonzero_weights(
            tVector, detail::RowSum{tRowSum}, detail::EstimatedConnectivity{tEstimatedConnectivity});
        EXPECT_EQ(tGlobalNonZeroIndices.size(), tLocalNonZeroWeights.size());
        EXPECT_EQ(tGlobalNonZeroIndices.size(), 0u);
    }
    tVector.putScalar(1.0);
    {
        const auto [tGlobalNonZeroIndices, tLocalNonZeroWeights] = detail::normalize_nonzero_weights(
            tVector, detail::RowSum{tRowSum}, detail::EstimatedConnectivity{tEstimatedConnectivity});
        ASSERT_EQ(tGlobalNonZeroIndices.size(), tMapSize / kNumRanks);
        EXPECT_DOUBLE_EQ(tLocalNonZeroWeights[0], 1.0 / tRowSum);
        EXPECT_DOUBLE_EQ(tLocalNonZeroWeights[1], 1.0 / tRowSum);
    }
}

TEST(LinearMaskFactoryDetail, MakeSearchPointsWithIdentifiers)
{
    const auto tCommunicator = Tpetra::getDefaultComm();
    const auto tThisRank = tCommunicator->getRank();

    const std::vector<utilities::Coordinate> tNodalCoordinates{
        {1, 1, 1}, {2, 2, 2}, {3, 3, 3}, {4, 4, 4}};  // non-zero to ensure not default ctor
    const auto tContiguousMap = create_contiguous_map(tNodalCoordinates.size(), tCommunicator);

    constexpr bool tZeroOut = true;
    auto tMultiVector = tpetra_integration::TpetraMultiVector(tContiguousMap, kNumSpatialDimensions, tZeroOut);

    tpetra_integration::distribute_on_tpetra_multivector(tNodalCoordinates, tMultiVector);

    /// round robin assignment to ranks 0->3 of tNodalCoordinates above
    const utilities::Coordinate tGoldCoordinate{tThisRank + 1.0, tThisRank + 1.0, tThisRank + 1.0};
    const std::vector<utilities::SearchPointWithIdentifier> tGoldLocalSearchPointWithIdentifiers{
        utilities::SearchPointWithIdentifier{utilities::convert_coordinate(tGoldCoordinate),
                                             utilities::Identifier{0, tThisRank}}};

    const auto tSearchPoints = detail::stk_search_points(tMultiVector, tThisRank);

    ASSERT_EQ(tSearchPoints.size(), 1u);
    EXPECT_EQ(tGoldLocalSearchPointWithIdentifiers[0].first, tSearchPoints[0].first);
    EXPECT_EQ(tGoldLocalSearchPointWithIdentifiers[0].second, tSearchPoints[0].second);
}

TEST(LinearMaskFactory, GenerateDistanceMapNodal)
{
    const LinearMaskFactory tLinearMaskFactory = create_simple_linear_mask<LinearMaskFactory>(std::nullopt);

    const auto tDistanceMap = tLinearMaskFactory.returnMask();

    /* 2/3          1/3             0           0
       1/4         1/2            1/4        0
       0            1/4            1/2        1/4
       0            0               1/3         2/3*/

    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 0, 0), 2.0 / 3.0);
    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 0, 1), 1.0 / 3.0);
    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 0, 2), 0);
    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 0, 3), 0);

    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 1, 0), 1.0 / 4.0);
    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 1, 1), 1.0 / 2.0);
    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 1, 2), 1.0 / 4.0);
    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 1, 3), 0);

    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 2, 0), 0);
    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 2, 1), 1.0 / 4.0);
    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 2, 2), 1.0 / 2.0);
    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 2, 3), 1.0 / 4.0);

    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 3, 0), 0);
    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 3, 1), 0);
    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 3, 2), 1.0 / 3.0);
    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 3, 3), 2.0 / 3.0);
}

TEST(LinearMaskFactory, GenerateDistanceMapGivenCentroid)
{
    const std::vector<utilities::Coordinate> tRelativeToCoordinate{{1, 0, 0}};
    const LinearMaskFactory tLinearMaskFactory = create_simple_linear_mask<LinearMaskFactory>(tRelativeToCoordinate);

    const auto tDistanceMap = tLinearMaskFactory.returnMask();
    /*
           1/4         1/2            1/4        0
    */

    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 0, 0), 1.0 / 4.0);
    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 0, 1), 1.0 / 2.0);
    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 0, 2), 1.0 / 4.0);
    EXPECT_DOUBLE_EQ(get_entry(tDistanceMap, 0, 3), 0);
}

}  // namespace plato::filter::extension::unittest
