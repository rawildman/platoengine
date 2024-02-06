#include "ValidatedInput.hpp"

#include <string>
#include <string_view>

#include "ConstraintValidation.hpp"
#include "Exception.hpp"
#include "GeometryRegistration.hpp"
#include "GeometryValidation.hpp"
#include "InputParser.hpp"
#include "ObjectiveValidation.hpp"
#include "OptimizerValidation.hpp"

namespace plato::functional::main::library
{
ValidatedInput::ValidatedInput(Plato::PlatoInput aInput, Key) : mInput{std::move(aInput)} {}

ValidatedInput::Geometry ValidatedInput::geometry() const
{
    plato::functional::geometry::library::GeometryInput tGeometryInput =
        plato::functional::geometry::library::first_geometry_input(mInput);
    using ValidatedGeometryVariant =
        typename plato::functional::geometry::library::ValidatedGeometryInput::RawInputType;
    // Use visit with a return value in c++20
    std::optional<ValidatedGeometryVariant> tValidatedGeometry;
    std::visit([&tValidatedGeometry](auto&& tGeometry)
               { tValidatedGeometry = Plato::Functional::Core::ValidatedInputTypeWrapper{std::move(tGeometry)}; },
               std::move(tGeometryInput));
    assert(tValidatedGeometry);
    return Plato::Functional::Core::ValidatedInputTypeWrapper{*tValidatedGeometry};
}

ValidatedInput::Objectives ValidatedInput::objectives() const
{
    return Plato::Functional::Core::ValidatedInputTypeWrapper{validatedVector(mInput.mObjectives)};
}

ValidatedInput::Constraints ValidatedInput::constraints() const
{
    return Plato::Functional::Core::ValidatedInputTypeWrapper{validatedVector(mInput.mConstraints)};
}

ValidatedInput::OptimizationParameters ValidatedInput::optimizationParameters() const
{
    return Plato::Functional::Core::ValidatedInputTypeWrapper{mInput.mOptimizationParameters};
}

template <typename T>
std::vector<Plato::Functional::Core::ValidatedInputTypeWrapper<T>> ValidatedInput::validatedVector(
    const std::vector<T>& aInputs)
{
    std::vector<Plato::Functional::Core::ValidatedInputTypeWrapper<T>> tValidatedInputs;
    std::transform(aInputs.cbegin(), aInputs.cend(), std::back_inserter(tValidatedInputs),
                   [](T aT) { return Plato::Functional::Core::ValidatedInputTypeWrapper{std::move(aT)}; });
    return tValidatedInputs;
}

ValidatedInput make_validated_input(Plato::PlatoInput aInput)
{
    std::vector<std::string> tMessages;
    tMessages = plato::functional::geometry::library::validate_geometry(aInput, std::move(tMessages));
    tMessages = plato::functional::criteria::library::validate_objectives(aInput.mObjectives, std::move(tMessages));
    tMessages = plato::functional::criteria::library::validate_constraints(aInput.mConstraints, std::move(tMessages));
    tMessages = plato::functional::optimizer::validate_optimization_parameters(aInput.mOptimizationParameters,
                                                                               std::move(tMessages));
    if (!tMessages.empty())
    {
        throw plato::functional::utilities::Exception(
            "Error: Could not validate input, the following errors were found: \n" +
            Plato::Functional::Validation::all_messages(tMessages));
    }
    return ValidatedInput{std::move(aInput), Key{}};
}

ValidatedInput parse_and_validate_from_file(const std::filesystem::path& aFileName)
{
    return make_validated_input(Plato::Functional::parse_input_from_file(aFileName));
}

ValidatedInput parse_and_validate(const std::string_view aInput)
{
    return make_validated_input(Plato::Functional::parse_input(aInput));
}

}  // namespace plato::functional::main::library
