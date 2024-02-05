#include "SharedLibIdentityFilter.hpp"

#include "MeshProxy.hpp"

namespace plato::functional::filter::testutilities
{
Plato::Functional::MeshProxy SharedLibIdentityFilter::filter(const Plato::Functional::MeshProxy& aMeshProxy) const { return aMeshProxy; }

Plato::Functional::Core::DynamicVector<double> SharedLibIdentityFilter::jacobianTimesVector(const Plato::Functional::MeshProxy&,
                                                                         const Plato::Functional::Core::DynamicVector<double>& aV) const
{
    return aV;
}

}  // plato::functional::filter::testutilities

namespace plato::functional::filter
{
std::unique_ptr<library::FilterInterface> plato_create_filter(const library::FilterParameters&)
{
    return std::make_unique<testutilities::SharedLibIdentityFilter>();
}
}  // namespace Plato::Functional
