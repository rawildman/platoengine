#include "IdentityFilter.hpp"

#include <ROL_StdVector.hpp>

#include "Exception.hpp"
#include "FilterJacobian.hpp"
#include "FilterRegistration.hpp"
#include "MeshProxy.hpp"
#include "Plato_InputEnumTypes.hpp"
#include "ValidationRegistration.hpp"

namespace Plato::Functional
{
namespace
{
[[maybe_unused]] static auto kIdentityFilterRegistration = Plato::Functional::FilterFactory::FilterRegistration{
    Plato::kFilterTypesTable.toString(Plato::FilterTypes::kIdentity).value(),
    [](const Plato::density_topology&) { return make_identity_filter_function(); }};

[[maybe_unused]] static auto kIdentityFilterValidationRegistration = Validation::Registration<Plato::density_topology>{
    [](const Plato::density_topology& aInput) { return validate_identity_filter(aInput); }};
}  // namespace

MeshProxy IdentityFilter::filter(const MeshProxy& aMeshProxy) const { return aMeshProxy; }

ROL::StdVector<double> IdentityFilter::jacobianTimesVector(const MeshProxy& aMeshProxy,
                                                           const ROL::StdVector<double>& aV) const
{
    const auto tVectorDimension = static_cast<std::size_t>(aV.dimension());
    const std::size_t tDensityDimension = aMeshProxy.mNodalDensities.size();
    if (tVectorDimension != tDensityDimension)
    {
        throw Exception{
            "IdentityFilter jacobian multiplication: Dimensions of vector and nodal density field don't match. Vector "
            "dimension: " +
            std::to_string(tVectorDimension) + ", density dimension: " + std::to_string(tDensityDimension)};
    }
    return aV;
}

auto make_identity_filter_function() -> Function<MeshProxy, FilterJacobian, const MeshProxy&>
{
    return make_function([](const MeshProxy& aMeshProxy) { return IdentityFilter{}.filter(aMeshProxy); },
                         [](const MeshProxy& aMeshProxy) {
                             return FilterJacobian{std::make_unique<IdentityFilter>(), aMeshProxy};
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

}  // namespace Plato::Functional
