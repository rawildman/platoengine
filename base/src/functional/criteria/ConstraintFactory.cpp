#include "ConstraintFactory.hpp"

#include <string>
#include <unordered_map>

#include "AffirmUtilities.hpp"
#include "CriterionFactory.hpp"
#include "Exception.hpp"
#include "MeshProxy.hpp"
#include "NodalSumObjective.hpp"
#include "SharedLibCriterion.hpp"

namespace Plato::Functional::ConstraintFactory
{
std::vector<Constraint<const MeshProxy&>> make_constraints(const std::vector<Plato::constraint>& aInput)
{
    detail::affirm_valid_input(aInput);
    std::vector<Constraint<const MeshProxy&>> tConstraints;
    for (const auto& tConInp : aInput)
    {
        tConstraints.push_back(detail::make_constraint(tConInp));
    }
    return tConstraints;
}

[[nodiscard]] std::unique_ptr<ROL::StdVector<double>> make_dual_vector()
{
    return std::make_unique<ROL::StdVector<double>>(1, 1.0);
}

namespace detail
{
Constraint<const MeshProxy&> make_constraint(const Plato::constraint& aConstraintInput)
{
    double tValue = 0;
    if (aConstraintInput.equal_to.has_value())
    {
        tValue = aConstraintInput.equal_to.value();
    }
    else if (aConstraintInput.greater_than.has_value())
    {
        tValue = aConstraintInput.greater_than.value();
    }
    else
    {
        tValue = aConstraintInput.less_than.value();
    }

    bool tIsLinear = false;
    if (aConstraintInput.is_linear.has_value())
    {
        tIsLinear = aConstraintInput.is_linear.value();
    }

    return Constraint<const MeshProxy&>{aConstraintInput.name.value_or("Unnamed Constraint"),
                                        CriterionFactory::make_criterion_function(aConstraintInput), tValue, tIsLinear};
}

void affirm_only_one_type(const Plato::constraint& aConstraintInput)
{
    int tTally = (aConstraintInput.equal_to.has_value() ? 1 : 0);
    tTally += (aConstraintInput.less_than.has_value() ? 1 : 0);
    tTally += (aConstraintInput.greater_than.has_value() ? 1 : 0);
    if (tTally != 1)
    {
        throw Plato::Functional::Exception(
            R"(Constraint must have only one "equal_to", "less_than", or "greater_than" type specification.)");
    }
}

void affirm_valid_input(const std::vector<Plato::constraint>& aInput)
{
    for (const auto& tConstraint : aInput)
    {
        detail::affirm_only_one_type(tConstraint);
        Plato::Functional::Affirmations::error_message_for_empty_parameter("Constraint", tConstraint.app, "app");
        if (tConstraint.app.value() == Plato::CodeOptions::kCustomApp)
        {
            Plato::Functional::Affirmations::error_message_for_empty_parameter(
                "Constraint", tConstraint.shared_library_path, "shared_library_path");
        }
    }
}
}  // namespace detail
}  // namespace Plato::Functional::ConstraintFactory
