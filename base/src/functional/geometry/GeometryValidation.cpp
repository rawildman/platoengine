#include "GeometryValidation.hpp"

#include "detail/GeometryRegistrationUtilities.hpp"

namespace Plato::Functional::Geometry
{

[[maybe_unused]] static auto kGeometryValidationRegistration =
    Plato::Functional::Validation::Registration<Plato::PlatoInput>{
        [](const Plato::PlatoInput& aInput) { return detail::validate_only_one_geometry(aInput); }};

namespace detail
{

std::optional<std::string> validate_only_one_geometry(const Plato::PlatoInput& aInput)
{
    if (const unsigned int tTally = Plato::Functional::GeometryFactory::Detail::geometry_blocks(aInput).size();
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

std::vector<std::string> validate_geometry(const Plato::PlatoInput& aInput,
                                           std::vector<std::string>&& aCurrentMessageList)
{
    const std::vector<GeometryFactory::GeometryInput> tGeometryBlocks =
        GeometryFactory::Detail::geometry_blocks(aInput);
    for (const GeometryFactory::GeometryInput& iBlockEntry : tGeometryBlocks)
    {
        aCurrentMessageList = std::visit(
            [tList = std::move(aCurrentMessageList)](const auto& aGeometryInput) mutable -> std::vector<std::string>
            {
                return Plato::Functional::Validation::validate(aGeometryInput, std::move(tList));
            },
            iBlockEntry);
    }

    return Plato::Functional::Validation::validate(aInput, std::move(aCurrentMessageList));
}

}  // namespace Plato::Functional::Geometry
