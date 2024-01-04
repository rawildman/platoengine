#include "ValidatedInput.hpp"

#include <string>
#include <string_view>

#include "ConstraintValidation.hpp"
#include "Exception.hpp"
#include "GeometryValidation.hpp"
#include "InputParser.hpp"
#include "ObjectiveValidation.hpp"
#include "OptimizerValidation.hpp"
#include "detail/GeometryRegistrationUtilities.hpp"

namespace Plato::Functional::Validation
{
ValidatedInput::ValidatedInput(Plato::PlatoInput aInput, Key) : mInput{std::move(aInput)} {}

ValidatedInput make_validated_input(Plato::PlatoInput aInput)
{
    std::vector<std::string> tMessages;
    tMessages = Geometry::validate_geometry(aInput, std::move(tMessages));
    tMessages = Criteria::validate_objectives(aInput.mObjectives, std::move(tMessages));
    tMessages = Criteria::validate_constraints(aInput.mConstraints, std::move(tMessages));
    tMessages = Optimizer::validate_optimization_parameters(aInput.mOptimizationParameters, std::move(tMessages));
    if (!tMessages.empty())
    {
        throw Exception("Error: Could not validate input, the following errors were found: \n" + all_messages(tMessages));
    }
    return ValidatedInput{aInput, Key{}};
}

ValidatedInput::Geometry ValidatedInput::geometry() const
{
    return Core::ValidatedInputTypeWrapper{GeometryFactory::Detail::first_geometry_input(mInput)};
}

ValidatedInput::Objectives ValidatedInput::objectives() const
{
    return Core::ValidatedInputTypeWrapper{validatedVector(mInput.mObjectives)};
}

ValidatedInput::Constraints ValidatedInput::constraints() const
{
    return Core::ValidatedInputTypeWrapper{validatedVector(mInput.mConstraints)};
}

ValidatedInput::OptimizationParameters ValidatedInput::optimizationParameters() const
{
    return Core::ValidatedInputTypeWrapper{mInput.mOptimizationParameters};
}

template <typename T>
std::vector<Core::ValidatedInputTypeWrapper<T>> ValidatedInput::validatedVector(const std::vector<T>& aInputs)
{
    std::vector<Core::ValidatedInputTypeWrapper<T>> tValidatedInputs;
    std::transform(aInputs.cbegin(), aInputs.cend(), std::back_inserter(tValidatedInputs),
                   [](T aT) { return Core::ValidatedInputTypeWrapper<T>{std::move(aT)}; });
    return tValidatedInputs;
}

Validation::ValidatedInput parse_and_validate_from_file(const std::filesystem::path& aFileName)
{
    return Validation::make_validated_input(parse_input_from_file(aFileName));
}

Validation::ValidatedInput parse_and_validate(const std::string_view aInput)
{
    return Validation::make_validated_input(parse_input(aInput));
}

}  // namespace Plato::Functional::Validation
