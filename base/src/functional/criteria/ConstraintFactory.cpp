#include "ConstraintFactory.hpp"

#include <string>
#include <unordered_map>

#include "CriterionFactory.hpp"
#include "Exception.hpp"
#include "MeshProxy.hpp"
#include "NodalSumObjective.hpp"
#include "SharedLibCriterion.hpp"

namespace Plato::Functional::ConstraintFactory
{
std::vector<Constraint<const MeshProxy&>> make_constraints(const ValidatedConstraints& aInput)
{
    std::vector<Constraint<const MeshProxy&>> tConstraints;
    for (const auto& tConInp : aInput.value())
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

}  // namespace detail
}  // namespace Plato::Functional::ConstraintFactory
