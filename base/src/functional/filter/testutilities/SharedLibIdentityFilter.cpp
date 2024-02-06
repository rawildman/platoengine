#include "SharedLibIdentityFilter.hpp"

#include "MeshProxy.hpp"

namespace plato::functional::filter::testutilities
{
core::MeshProxy SharedLibIdentityFilter::filter(const core::MeshProxy& aMeshProxy) const
{
    return aMeshProxy;
}

linear_algebra::DynamicVector<double> SharedLibIdentityFilter::jacobianTimesVector(
    const core::MeshProxy&, const linear_algebra::DynamicVector<double>& aV) const
{
    return aV;
}

}  // namespace plato::functional::filter::testutilities

namespace plato::functional
{
std::unique_ptr<filter::library::FilterInterface> plato_create_filter(const filter::library::FilterParameters&)
{
    return std::make_unique<filter::testutilities::SharedLibIdentityFilter>();
}
}  // namespace plato::functional