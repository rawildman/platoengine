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

std::unique_ptr<ROL::StdVector<double>> make_dual_vector()
{
    return std::make_unique<ROL::StdVector<double>>(1, 1.0);
}

namespace detail
{
Constraint<const MeshProxy&> make_constraint(const Plato::constraint& aConstraintInput)
{
    const double tValue = aConstraintInput.equal_to.value();
    const bool tIsLinear = aConstraintInput.is_linear.value_or(false);
    return Constraint<const MeshProxy&>{aConstraintInput.name.value_or("Unnamed Constraint"),
                                        CriterionFactory::make_criterion_function(aConstraintInput), tValue, tIsLinear};
}

}  // namespace detail
}  // namespace Plato::Functional::ConstraintFactory
