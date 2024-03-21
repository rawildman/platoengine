#ifndef PLATO_CORE_INPUTVARIANTUTILTIES
#define PLATO_CORE_INPUTVARIANTUTILTIES

#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

#include "plato/core/ValidatedInputTypeWrapper.hpp"
#include "plato/input_parser/InputBlocks.hpp"
#include "plato/utilities/Exception.hpp"

namespace plato::core
{
/// @brief Given a variant type @a InputVariant, this copies and returns each type type found in @a aInput in a vector.
/// @tparam InputVariant A `std::variant` with alternatives corresponding to input types in the ParsedInput struct.
template <typename InputVariant>
[[nodiscard]] std::vector<InputVariant> all_input_blocks_in_variant(const input_parser::ParsedInput& aInput);

/// @return An optional InputVariant variant, which is the first non-empty input block found in @a aInput that is a
/// member of the variant.
///  If no block was found, an empty optional is returned.
/// @tparam InputVariant A `std::variant` with alternatives corresponding to input types in the ParsedInput struct.
template <typename InputVariant>
[[nodiscard]] std::optional<InputVariant> first_input_block_in_variant(const input_parser::ParsedInput& aInput);

/// @return The name of the input held by the variant @a aInput
/// @tparam InputVariant A `std::variant` with alternatives corresponding to input types in the ParsedInput struct.
template <typename T>
[[nodiscard]] std::string block_name(const ValidatedInputTypeWrapper<T>& aInput);

namespace detail
{
/// @brief A type trait specifying if a type @a T is an alternative of variant @a VariantT
template <typename T, typename VariantT>
constexpr bool kIsVariantMember = false;

template <typename T, typename... Ts>
constexpr bool kIsVariantMember<T, std::variant<Ts...>> = std::disjunction_v<std::is_same<T, Ts>...>;

template <typename InputVariant, typename T, typename = void>
[[nodiscard]] std::optional<InputVariant> to_variant(const T&)
{
    return std::nullopt;
}

/// @return If @a aT is an alternative of variant InputVariant, and is non-empty, returns an optional-wrapped copy of
/// @a aT.
template <typename InputVariant, typename T, typename = std::enable_if_t<kIsVariantMember<T, InputVariant>>>
[[nodiscard]] std::optional<InputVariant> to_variant(const boost::optional<T>& aT)
{
    if (aT)
    {
        return std::make_optional(InputVariant{aT.value()});
    }
    return std::nullopt;
}

template <typename InputVariant>
void emplace_back_if_has_value(std::vector<InputVariant>& aInput, std::optional<InputVariant>&& aOptionalInput)
{
    if (aOptionalInput.has_value())
    {
        aInput.emplace_back(std::move(aOptionalInput).value());
    }
}

template <typename InputVariant, std::size_t... Is>
[[nodiscard]] std::vector<InputVariant> all_input_blocks_impl(const input_parser::ParsedInput& aInput,
                                                              std::integer_sequence<std::size_t, Is...>)
{
    std::vector<InputVariant> tInput;
    (emplace_back_if_has_value(tInput, to_variant<InputVariant>(boost::fusion::at_c<Is>(aInput))), ...);
    return tInput;
}

}  // namespace detail

template <typename InputVariant>
std::vector<InputVariant> all_input_blocks_in_variant(const input_parser::ParsedInput& aInput)
{
    constexpr auto tNumInputFields = boost::fusion::result_of::size<input_parser::ParsedInput>::value;
    return detail::all_input_blocks_impl<InputVariant>(aInput, std::make_index_sequence<tNumInputFields>{});
}

template <typename InputVariant>
std::optional<InputVariant> first_input_block_in_variant(const input_parser::ParsedInput& aInput)
{
    const std::vector<InputVariant> tInputBlocks = all_input_blocks_in_variant<InputVariant>(aInput);
    if (tInputBlocks.empty())
    {
        return std::nullopt;
    }
    else
    {
        return tInputBlocks.front();
    }
}

template <typename T>
std::string block_name(const ValidatedInputTypeWrapper<T>& aInput)
{
    return std::visit(
        [](const auto& aObj) -> std::string
        {
            using ValidatedInputType = std::decay_t<decltype(aObj)>;
            using RawInputType = typename ValidatedInputType::RawInputType;
            return input_parser::block_name<RawInputType>();
        },
        aInput.rawInput());
}
}  // namespace plato::core

#endif
