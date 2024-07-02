#include "plato/filter/extension/KernelFilter.hpp"

#include <boost/math/constants/constants.hpp>
#include <boost/mpi.hpp>
#include <boost/serialization/vector.hpp>
#include <memory>
#include <optional>

#include "plato/core/MeshProxy.hpp"
#include "plato/linear_algebra/DynamicVector.hpp"
#include "plato/third_party_integration/stk_io/VolumeUtilities.hpp"

namespace plato::filter::extension
{

KernelFilter::KernelFilter(const std::filesystem::path& aMeshFileName,
                           const FilterRadius aFilterRadius,
                           const FilterCentering aFilterCentering,
                           const boost::mpi::communicator& aCommunicator)
    : mLinearMask(detail::create_linear_mask(aMeshFileName, aFilterRadius, aFilterCentering, aCommunicator)),
      mCommunicator(aCommunicator)

{
}

core::MeshProxy KernelFilter::filter(const core::MeshProxy& aMeshProxy) const
{
    core::MeshProxy tMeshProxy{aMeshProxy};
    tMeshProxy.mNodalDensities = mLinearMask.matrixMultiply(aMeshProxy.mNodalDensities);
    return tMeshProxy;
}

linear_algebra::DynamicVector<double> KernelFilter::jacobianTimesVector(
    const core::MeshProxy& /*aMeshProxy*/, const linear_algebra::DynamicVector<double>& aV) const
{
    return linear_algebra::DynamicVector<double>{mLinearMask.transposeMatrixMultiply(aV.stdVector())};
}

namespace detail
{
double filter_volume(const FilterRadius aFilterRadius)
{
    return 4.0 / 3.0 * boost::math::constants::pi<double>() * aFilterRadius.mValue * aFilterRadius.mValue *
           aFilterRadius.mValue;
}

int determine_maximum_connectivity_estimate(const std::filesystem::path& aMeshFileName,
                                            const FilterRadius aFilterRadius)
{
    const auto tBulk = third_party_integration::stk_io::read_mesh_bulk_data(aMeshFileName);
    auto tNodalCoordinates = third_party_integration::stk_io::nodal_coordinates(*tBulk);
    const double tAverageNodalDensity = third_party_integration::stk_io::average_nodal_density(*tBulk);
    const double tSearchVolume = detail::filter_volume(aFilterRadius);
    return static_cast<int>(tSearchVolume * tAverageNodalDensity *
                            kMaxMultiplier);  // for Tpetra sparse matrix allocation
}

LinearMask create_linear_mask(const std::filesystem::path& aMeshFileName,
                              const FilterRadius aFilterRadius,
                              const FilterCentering aFilterCentering,
                              const boost::mpi::communicator& aCommunicator)
{
    const auto tBulk = third_party_integration::stk_io::read_mesh_bulk_data(aMeshFileName);
    auto tNodalCoordinates = third_party_integration::stk_io::nodal_coordinates(*tBulk);
    const int tMaximumConnectivityEstimate =
        detail::determine_maximum_connectivity_estimate(aMeshFileName, aFilterRadius);

    if (aFilterCentering == FilterCentering::ElementCentered)
    {
        auto tElementCentroids = third_party_integration::stk_io::element_centroids(*tBulk);
        return LinearMask(NodalVector{tNodalCoordinates}, CenterVector{tElementCentroids},
                          SearchRadius{aFilterRadius.mValue}, tMaximumConnectivityEstimate, aCommunicator);
    }
    else
    {
        return LinearMask(NodalVector{tNodalCoordinates}, SearchRadius{aFilterRadius.mValue},
                          tMaximumConnectivityEstimate, aCommunicator);
    }
}

}  // namespace detail

}  // namespace plato::filter::extension
