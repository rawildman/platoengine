#include "plato/geometry/library/GeometryValidation.hpp"

#include "plato/core/InputVariantUtilities.hpp"
#include "plato/core/ValidationRegistration.hpp"
#include "plato/geometry/library/GeometryRegistration.hpp"

namespace plato::geometry::library
{
[[maybe_unused]] static auto kGeometryValidationRegistration = core::ValidationRegistration<input_parser::ParsedInput>{
    [](const input_parser::ParsedInput& aInput) { return detail::validate_only_one_geometry(aInput); }};

namespace detail
{
std::optional<std::string> validate_only_one_geometry(const input_parser::ParsedInput& aInput)
{
    if (const unsigned int tTally = core::all_input_blocks_in_variant<library::GeometryInput>(aInput).size();
        tTally != 1)
    {
        return "Only define exactly one geometry block. There were " + std::to_string(tTally) + " found.";
    }
    else
    {
        return std::nullopt;
    }
}

}  // namespace detail

std::vector<std::string> validate_geometry(const input_parser::ParsedInput& aInput,
                                           std::vector<std::string>&& aCurrentMessageList)
{
    const std::vector<library::GeometryInput> tGeometryBlocks =
        core::all_input_blocks_in_variant<library::GeometryInput>(aInput);
    for (const library::GeometryInput& iBlockEntry : tGeometryBlocks)
    {
        aCurrentMessageList = std::visit(
            [tList = std::move(aCurrentMessageList)](const auto& aGeometryInput) mutable -> std::vector<std::string>
            { return core::validate(aGeometryInput, std::move(tList)); },
            iBlockEntry);
    }

    return core::validate(aInput, std::move(aCurrentMessageList));
}

}  // namespace plato::geometry::library
