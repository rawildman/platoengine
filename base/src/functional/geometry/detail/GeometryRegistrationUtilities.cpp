#include "GeometryRegistrationUtilities.hpp"

namespace Plato::Functional::GeometryFactory::Detail
{

[[nodiscard]] unsigned int geometry_block_count(const Plato::PlatoInput& aInput)
{
    constexpr auto tNumInputFields = boost::fusion::result_of::size<Plato::PlatoInput>::value;
    return geometry_block_count_impl(aInput, std::make_index_sequence<tNumInputFields>{});
}

[[nodiscard]] std::optional<GeometryFactory::GeometryInput> geometry_block(const Plato::PlatoInput& aInput)
{
    constexpr auto tNumInputFields = boost::fusion::result_of::size<Plato::PlatoInput>::value;
    return geometry_block_impl(aInput, std::make_index_sequence<tNumInputFields>{});
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
}

}  // namespace Plato::Functional::GeometryFactory::Detail