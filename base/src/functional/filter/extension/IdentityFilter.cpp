#include "IdentityFilter.hpp"

#include "DynamicVector.hpp"
#include "Exception.hpp"
#include "FilterJacobian.hpp"
#include "FilterRegistration.hpp"
#include "InputEnumTypes.hpp"
#include "MeshProxy.hpp"
#include "ValidationRegistration.hpp"

namespace plato::functional::filter::extension
{
namespace
{
[[maybe_unused]] static auto kIdentityFilterRegistration =
    library::FilterRegistration{Plato::kFilterTypesTable.toString(Plato::FilterTypes::kIdentity).value(),
                                [](const Plato::density_topology&) { return make_identity_filter_function(); }};

[[maybe_unused]] static auto kIdentityFilterValidationRegistration =
    Plato::Functional::Validation::Registration<Plato::density_topology>{[](const Plato::density_topology& aInput)
                                                                         { return validate_identity_filter(aInput); }};
}  // namespace

Plato::Functional::MeshProxy IdentityFilter::filter(const Plato::Functional::MeshProxy& aMeshProxy) const
{
    return aMeshProxy;
}

linear_algebra::DynamicVector<double> IdentityFilter::jacobianTimesVector(
    const Plato::Functional::MeshProxy& aMeshProxy, const linear_algebra::DynamicVector<double>& aV) const
{
    const auto tVectorDimension = static_cast<std::size_t>(aV.size());
    const std::size_t tDensityDimension = aMeshProxy.mNodalDensities.size();
    if (tVectorDimension != tDensityDimension)
    {
        throw Plato::Functional::Exception{
            "IdentityFilter jacobian multiplication: Dimensions of vector and nodal density field don't match. Vector "
            "dimension: " +
            std::to_string(tVectorDimension) + ", density dimension: " + std::to_string(tDensityDimension)};
    }
    return aV;
}

auto make_identity_filter_function() -> Plato::Functional::
    Function<Plato::Functional::MeshProxy, library::FilterJacobian, const Plato::Functional::MeshProxy&>
{
    return Plato::Functional::make_function(
        [](const Plato::Functional::MeshProxy& aMeshProxy) { return IdentityFilter{}.filter(aMeshProxy); },
        [](const Plato::Functional::MeshProxy& aMeshProxy) {
            return library::FilterJacobian{std::make_unique<IdentityFilter>(), aMeshProxy};
        });
}

[[nodiscard]] std::optional<std::string> validate_identity_filter(const Plato::density_topology& aInput)
{
    if (aInput.filter_type && aInput.filter_type.value() == Plato::FilterTypes::kIdentity)
    {
        if (aInput.boundary_sticking_penalty.has_value() || aInput.filter_radius.has_value())
        {
            return Plato::block_name<Plato::density_topology>() +
                   R"( identity filter cannot have "filter_radius" or "boundary_sticking_penalty" defined.)";
        }
    }
    return std::nullopt;
}

}  // namespace plato::functional::filter::extension
