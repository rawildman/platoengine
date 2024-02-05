#include "ConstraintFactory.hpp"

#include <string>
#include <unordered_map>

#include "CriterionFactory.hpp"
#include "Exception.hpp"
#include "MeshProxy.hpp"

namespace plato::functional::criteria::library
{
std::vector<Constraint<const Plato::Functional::MeshProxy&>> make_constraints(const ValidatedConstraints& aInput)
{
    std::vector<Constraint<const Plato::Functional::MeshProxy&>> tConstraints;
    std::transform(aInput.rawInput().cbegin(), aInput.rawInput().cend(), std::back_inserter(tConstraints),
                   [](const Plato::Functional::Core::ValidatedInputTypeWrapper<Plato::constraint>& aValidatedInput)
                   { return detail::make_constraint(aValidatedInput); });
    return tConstraints;
}

std::unique_ptr<ROL::StdVector<double>> make_dual_vector() { return std::make_unique<ROL::StdVector<double>>(1, 1.0); }

namespace detail
{
Constraint<const Plato::Functional::MeshProxy&> make_constraint(
    const Plato::Functional::Core::ValidatedInputTypeWrapper<Plato::constraint>& aConstraintInput)
{
    const Plato::constraint& tRawInput = aConstraintInput.rawInput();
    const double tValue = tRawInput.equal_to.value();
    const bool tIsLinear = tRawInput.is_linear.value_or(false);
    return Constraint<const Plato::Functional::MeshProxy&>{
        tRawInput.name.value_or("Unnamed Constraint"), make_criterion_function(aConstraintInput), tValue, tIsLinear};
}

}  // namespace detail
}  // namespace plato::functional::criteria::library
