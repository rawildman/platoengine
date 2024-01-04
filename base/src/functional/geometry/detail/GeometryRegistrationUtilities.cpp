#include "GeometryRegistrationUtilities.hpp"

namespace Plato::Functional::GeometryFactory::Detail
{

void emplace_back_if_has_value(std::vector<GeometryFactory::GeometryInput>& aGeometryInput,
                               std::optional<GeometryFactory::GeometryInput>&& aOptionalGeometry)
{
    if (aOptionalGeometry.has_value())
    {
        aGeometryInput.emplace_back(std::move(aOptionalGeometry).value());
    }
}

std::vector<GeometryFactory::GeometryInput> geometry_blocks(const Plato::PlatoInput& aInput)
{
    constexpr auto tNumInputFields = boost::fusion::result_of::size<Plato::PlatoInput>::value;
    return geometry_blocks_impl(aInput, std::make_index_sequence<tNumInputFields>{});
}

std::optional<GeometryFactory::GeometryInput> first_geometry_block(const Plato::PlatoInput& aInput)
{
    const std::vector<GeometryFactory::GeometryInput> tGeometryBlocks = geometry_blocks(aInput);
    if (tGeometryBlocks.empty())
    {
        return std::nullopt;
    }
    else
    {
        return tGeometryBlocks.front();
    }
}

GeometryFactory::GeometryInput first_geometry_input(const Plato::PlatoInput& aInput)
{
    const std::optional<GeometryFactory::GeometryInput> tGeometryInput = first_geometry_block(aInput);
    if (!tGeometryInput)
    {
        throw Exception("No geometry block was defined.");
    }
    return tGeometryInput.value();
}

std::string block_name(const GeometryFactory::GeometryInput& aInput)
{
    return std::visit(
        [](const auto& aObj) -> std::string
        {
            using InputType = std::decay_t<decltype(aObj)>;
            return Plato::block_name<InputType>();
        },
        aInput);
}

std::string block_name(const GeometryFactory::ValidatedGeometryInput& aInput)
{
    return std::visit(
        [](const auto& aObj) -> std::string
        {
            using ValidatedInputType = std::decay_t<decltype(aObj)>;
            using RawInputType = typename ValidatedInputType::RawInputType;
            return Plato::block_name<RawInputType>();
        },
        aInput.value());
}

}  // namespace Plato::Functional::GeometryFactory::Detail
