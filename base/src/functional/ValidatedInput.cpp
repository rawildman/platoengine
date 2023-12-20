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
ValidatedInput::ValidatedInput(Plato::PlatoInput aInput, Key) : mInput{std::move(aInput)}
{
}

ValidatedInput make_validated_input(Plato::PlatoInput aInput)
{
    std::vector<std::string> tMessages;
    tMessages = Geometry::validate_geometry(aInput, std::move(tMessages));
    tMessages = Criteria::validate_objectives(aInput.mObjectives, std::move(tMessages));
    tMessages = Criteria::validate_constraints(aInput.mConstraints, std::move(tMessages));
    tMessages = Optimizer::validate_optimization_parameters(aInput.mOptimizationParameters, std::move(tMessages));
    if (tMessages.size() != 0)
    {
        Validation::print_messages(tMessages);
        throw Exception("Error: Could not validate input. See issue messages above.");
    }
    return ValidatedInput{aInput, Key{}};
}

Core::ValidatedInputTypeWrapper<GeometryFactory::GeometryInput> ValidatedInput::geometry() const
{
    return Core::ValidatedInputTypeWrapper<GeometryFactory::GeometryInput>{GeometryFactory::Detail::first_geometry_input(mInput)};
}
Core::ValidatedInputTypeWrapper<std::vector<Plato::objective>> ValidatedInput::objectives() const
{
    return Core::ValidatedInputTypeWrapper<std::vector<Plato::objective>>{mInput.mObjectives};
}
Core::ValidatedInputTypeWrapper<std::vector<Plato::constraint>> ValidatedInput::constraints() const
{
    return Core::ValidatedInputTypeWrapper<std::vector<Plato::constraint>>{mInput.mConstraints};
}
Core::ValidatedInputTypeWrapper<Plato::optimization_parameters> ValidatedInput::optimizationParameters() const
{
    return Core::ValidatedInputTypeWrapper<Plato::optimization_parameters>{mInput.mOptimizationParameters};
}

}  // namespace Plato::Functional::Validation