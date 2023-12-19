#include "ValidatedInput.hpp"

#include <string>
#include <string_view>

#include "ConstraintValidation.hpp"
#include "Exception.hpp"
#include "GeometryValidation.hpp"
#include "InputParser.hpp"
#include "ObjectiveValidation.hpp"

namespace Plato::Functional::Core
{
ValidatedInput::ValidatedInput(Plato::PlatoInput aInput, Key) : mInput{std::move(aInput)}
{
}

Plato::PlatoInput ValidatedInput::value() const
{
    return mInput;
}

ValidatedInput make_validated_input(Plato::PlatoInput aInput)
{
    std::vector<std::string> tMessages;
    tMessages = Geometry::validate_geometry(aInput, std::move(tMessages));
    tMessages = Criteria::validate_objectives(aInput.mObjectives, std::move(tMessages));
    tMessages = Criteria::validate_constraints(aInput.mConstraints, std::move(tMessages));
    if (tMessages.size() != 0)
    {
        Validation::print_messages(tMessages);
        throw Exception("Could not validate input.");
    }
    return ValidatedInput{aInput, Key{}};
}

}  // namespace Plato::Functional::Core