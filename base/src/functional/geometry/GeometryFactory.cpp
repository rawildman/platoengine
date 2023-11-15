#include "GeometryFactory.hpp"

#include <optional>

#include "BrickShapeGeometry.hpp"
#include "DensityTopology.hpp"
#include "Exception.hpp"
#include "GeometryRegistration.hpp"
#include "Plato_InputBlocks.hpp"

namespace Plato::Functional::GeometryFactory
{

namespace
{

template <typename T, typename VARIANT_T>
struct isVariantMember;
template <typename T, typename... ALL_T>
struct isVariantMember<T, std::variant<ALL_T...>> : public std::disjunction<std::is_same<T, ALL_T>...>
{
};

template <typename T>
[[nodiscard]] std::optional<GeometryFactory::GeometryInput> make_variant(const T&)
{
    return std::nullopt;
}

template <typename T>
[[nodiscard]] std::optional<GeometryFactory::GeometryInput> make_variant(const boost::optional<T>& aObj)
{
    if (isVariantMember<T, GeometryFactory::GeometryInput>::value && aObj)
    {
        return std::make_optional(GeometryFactory::GeometryInput{aObj.value()});
    }
    return std::nullopt;
}

template <std::size_t... Is>
[[nodiscard]] std::optional<GeometryFactory::GeometryInput> geometry_input_impl(
    const Plato::PlatoInput& aInput, std::integer_sequence<std::size_t, Is...>)
{
    std::optional<GeometryFactory::GeometryInput> tGeometryInput;
    ((tGeometryInput = make_variant(boost::fusion::at_c<Is>(aInput))) || ...);
    return tGeometryInput;
}

[[nodiscard]] std::optional<GeometryFactory::GeometryInput> geometry_block(const Plato::PlatoInput& aInput)
{
    constexpr auto tNumInputFields = boost::fusion::result_of::size<Plato::PlatoInput>::value;
    return geometry_input_impl(aInput, std::make_index_sequence<tNumInputFields>{});
}

[[nodiscard]] GeometryFactory::GeometryInput geometry_input(const Plato::PlatoInput& aInput)
{
    std::optional<GeometryFactory::GeometryInput> tGeometryInput = geometry_block(aInput);
    if (!tGeometryInput)
    {
        throw Exception("No geometry block was defined.");
    }
    return tGeometryInput.value();
}

[[nodiscard]] std::string block_name(const GeometryFactory::GeometryInput& aInput)
{
    return std::visit(
        [](const auto& aObj) -> std::string
        {
            using InputType = std::decay_t<decltype(aObj)>;
            return Plato::block_name<InputType>();
        },
        aInput);
    throw Exception("No geometry block was defined.");
}

}  // namespace

GeometryFunction make_geometry_function(const Plato::PlatoInput& aInput)
{
    const GeometryInput tGeometryInput = geometry_input(aInput);

    if (const auto tIter =
            detail::registered_functions<GeometryFunction, GeometryInput>().find(block_name(tGeometryInput));
        tIter != detail::registered_functions<GeometryFunction, GeometryInput>().end())
    {
        return tIter->second(tGeometryInput);
    }
    else
    {
        throw Plato::Functional::Exception{"Unknown geometry"};
    }
}

std::unique_ptr<ROL::StdVector<double>> make_initial_guess(const Plato::PlatoInput& aInput)
{
    if (aInput.mBrickShapeGeometry)
    {
        return BrickShapeGeometry::initialGuess();
    }
    else if (aInput.mDensityTopology)
    {
        return DensityTopology::initialGuess(aInput.mDensityTopology->mesh_name->mName);
    }
    throw Exception("No geometry block was defined.");
}

ROL::StdBoundConstraint<double> make_bound_constraint(const Plato::PlatoInput& aInput)
{
    if (aInput.mBrickShapeGeometry)
    {
        auto [lowerBound, upperBound] = BrickShapeGeometry::bounds();
        return ROL::StdBoundConstraint<double>{lowerBound, upperBound};
    }
    else if (aInput.mDensityTopology)
    {
        auto [lowerBound, upperBound] = DensityTopology::bounds(aInput.mDensityTopology->mesh_name->mName);
        return ROL::StdBoundConstraint<double>{lowerBound, upperBound};
    }
    throw Exception("No geometry block was defined.");
}

std::function<void(const ROL::StdVector<double>&)> make_output_function(const Plato::PlatoInput& aInput)
{
    if (aInput.mBrickShapeGeometry)
    {
        return [](const ROL::StdVector<double>& x) { BrickShapeGeometry::output(x); };
    }
    else if (aInput.mDensityTopology)
    {
        if (!aInput.mDensityTopology->output_name)
        {
            throw Exception("density_topology requires an output_name.");
        }
        return [tInputMeshName = aInput.mDensityTopology->mesh_name->mName,
                tOutputMeshName = aInput.mDensityTopology->output_name->mName](const ROL::StdVector<double>& x)
        { DensityTopology::output(tInputMeshName, x, tOutputMeshName); };
    }
    throw Exception("No geometry block was defined.");
}
}  // namespace Plato::Functional::GeometryFactory
