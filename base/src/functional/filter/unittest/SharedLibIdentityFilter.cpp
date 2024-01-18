#include "SharedLibIdentityFilter.hpp"

#include "MeshProxy.hpp"

namespace Plato::Functional::Test
{
MeshProxy SharedLibIdentityFilter::filter(const MeshProxy& aMeshProxy) const { return aMeshProxy; }

Core::DynamicVector<double> SharedLibIdentityFilter::jacobianTimesVector(const MeshProxy&,
                                                                         const Core::DynamicVector<double>& aV) const
{
    return aV;
}

}  // namespace Plato::Functional::Test

namespace Plato::Functional
{
std::unique_ptr<FilterInterface> plato_create_filter(const FilterParameters&)
{
    return std::make_unique<Test::SharedLibIdentityFilter>();
}
}  // namespace Plato::Functional
