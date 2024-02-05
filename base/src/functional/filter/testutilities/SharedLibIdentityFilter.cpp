#include "SharedLibIdentityFilter.hpp"

#include "MeshProxy.hpp"

namespace plato::functional::filter::testutilities
{
Plato::Functional::MeshProxy SharedLibIdentityFilter::filter(const Plato::Functional::MeshProxy& aMeshProxy) const
{
    return aMeshProxy;
}

Plato::Functional::Core::DynamicVector<double> SharedLibIdentityFilter::jacobianTimesVector(
    const Plato::Functional::MeshProxy&, const Plato::Functional::Core::DynamicVector<double>& aV) const
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