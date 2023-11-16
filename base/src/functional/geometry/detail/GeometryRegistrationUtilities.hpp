#ifndef PLATO_FUNCTIONAL_DETAIL_GEOMETRYREGISTRATIONUTILITIES
#define PLATO_FUNCTIONAL_DETAIL_GEOMETRYREGISTRATIONUTILITIES

#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

#include "Exception.hpp"
#include "GeometryRegistration.hpp"
#include "Plato_InputBlocks.hpp"

namespace Plato::Functional::GeometryFactory::Detail
{
/// @brief A type trait specifying if a type @a T is an alternative of variant @a VariantT
template <typename T, typename VariantT>
struct IsVariantMember;

template <typename T, typename... Ts>
struct IsVariantMember<T, std::variant<Ts...>> : public std::disjunction<std::is_same<T, Ts>...>
{
};

template <typename T>
[[nodiscard]] std::optional<GeometryFactory::GeometryInput> make_variant_if_geometry(const T&)
{
    return std::nullopt;
}

/// @return If @a aT is an alternative of variant GeometryInput, and is non-empty, returns a wrapped copy of @a aT.
template <typename T>
[[nodiscard]] std::optional<GeometryFactory::GeometryInput> make_variant_if_geometry(const boost::optional<T>& aT)
{
    if (IsVariantMember<T, GeometryFactory::GeometryInput>::value && aT)
    {
        return std::make_optional(GeometryFactory::GeometryInput{aT.value()});
    }
    return std::nullopt;
}

template <std::size_t... Is>
[[nodiscard]] std::optional<GeometryFactory::GeometryInput> geometry_block_impl(
    const Plato::PlatoInput& aInput, std::integer_sequence<std::size_t, Is...>)
{
    std::optional<GeometryFactory::GeometryInput> tGeometryInput;
    ((tGeometryInput = make_variant_if_geometry(boost::fusion::at_c<Is>(aInput))) || ...);
    return tGeometryInput;
}

/// @return An optional GeometryInput variant, which is the first non-empty geometry input block found in @a aInput.
///  If no geometry block was found, an empty optional is returned.
[[nodiscard]] std::optional<GeometryFactory::GeometryInput> geometry_block(const Plato::PlatoInput& aInput)
{
    constexpr auto tNumInputFields = boost::fusion::result_of::size<Plato::PlatoInput>::value;
    return geometry_block_impl(aInput, std::make_index_sequence<tNumInputFields>{});
}

/// @return A GeometryInput variant, which is the first non-empty geometry input block found in @a aInput.
/// @throw Exception If no geometry block was defined in @a aInput.
[[nodiscard]] GeometryFactory::GeometryInput geometry_input(const Plato::PlatoInput& aInput)
{
    std::optional<GeometryFactory::GeometryInput> tGeometryInput = geometry_block(aInput);
    if (!tGeometryInput)
    {
        throw Exception("No geometry block was defined.");
    }
    return tGeometryInput.value();
}

/// @return The name of the geometry input held by the variant @a aInput
[[nodiscard]] std::string block_name(const GeometryFactory::GeometryInput& aInput)
{
    return std::visit(
        [](const auto& aObj) -> std::string
        {
            using InputType = std::decay_t<decltype(aObj)>;
            return Plato::block_name<InputType>();
        },
        aInput);
}
}  // namespace Plato::Functional::GeometryFactory::Detail

#endif
