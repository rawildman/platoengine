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
constexpr bool kIsVariantMember = false;

template <typename T, typename... Ts>
constexpr bool kIsVariantMember<T, std::variant<Ts...>> = std::disjunction_v<std::is_same<T, Ts>...>;

template <typename T>
[[nodiscard]] std::optional<GeometryFactory::GeometryInput> make_variant_if_geometry(const T&)
{
    return std::nullopt;
}

/// @return If @a aT is an alternative of variant GeometryInput, and is non-empty, returns an optional-wrapped copy of
/// @a aT.
template <typename T>
[[nodiscard]] std::optional<GeometryFactory::GeometryInput> make_variant_if_geometry(const boost::optional<T>& aT)
{
    if (kIsVariantMember<T, GeometryFactory::GeometryInput> && aT)
    {
        return std::make_optional(GeometryFactory::GeometryInput{aT.value()});
    }
    return std::nullopt;
}

void emplace_back_if_has_value(std::vector<GeometryFactory::GeometryInput>& aGeometryInput,
                               std::optional<GeometryFactory::GeometryInput>&& aOptionalGeometry);

template <std::size_t... Is>
[[nodiscard]] std::vector<GeometryFactory::GeometryInput> geometry_blocks_impl(
    const Plato::PlatoInput& aInput, std::integer_sequence<std::size_t, Is...>)
{
    std::vector<GeometryFactory::GeometryInput> tGeometryInput;
    (emplace_back_if_has_value(tGeometryInput, make_variant_if_geometry(boost::fusion::at_c<Is>(aInput))), ...);
    return tGeometryInput;
}
[[nodiscard]] std::vector<GeometryFactory::GeometryInput> geometry_blocks(const Plato::PlatoInput& aInput);

/// @return An optional GeometryInput variant, which is the first non-empty geometry input block found in @a aInput.
///  If no geometry block was found, an empty optional is returned.
[[nodiscard]] std::optional<GeometryFactory::GeometryInput> first_geometry_block(const Plato::PlatoInput& aInput);

/// @return A GeometryInput variant, which is the first non-empty geometry input block found in @a aInput.
/// @throw Exception If no geometry block was defined in @a aInput.
[[nodiscard]] GeometryFactory::GeometryInput first_geometry_input(const Plato::PlatoInput& aInput);

/// @return The name of the geometry input held by the variant @a aInput
[[nodiscard]] std::string block_name(const GeometryFactory::GeometryInput& aInput);

}  // namespace Plato::Functional::GeometryFactory::Detail

#endif
