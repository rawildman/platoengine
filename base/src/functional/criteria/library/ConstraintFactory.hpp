#ifndef PLATO_FUNCTIONAL_CONSTRAINTFACTORY
#define PLATO_FUNCTIONAL_CONSTRAINTFACTORY

#include <ROL_StdVector.hpp>
#include <string>

#include "DynamicVector.hpp"
#include "Function.hpp"
#include "InputBlocks.hpp"
#include "ValidatedInputTypeWrapper.hpp"

namespace Plato::Functional
{
struct MeshProxy;
}

namespace plato::functional::criteria::library
{
using ValidatedConstraints = Plato::Functional::Core::ValidatedInputTypeWrapper<
    std::vector<Plato::Functional::Core::ValidatedInputTypeWrapper<Plato::constraint>>>;

/// @brief Holds members for defining a Constraint
/// @tparam FunctionArg The argument of the function used to define the constraint.
///   Typically, this is either MeshProxy or a vector type such as DynamicVector.
template <typename FunctionArg>
struct Constraint
{
    using ConstraintFunction =
        Plato::Functional::Function<double, Plato::Functional::Core::DynamicVector<double>, FunctionArg>;

    std::string mName;
    ConstraintFunction mConstraintFunction;
    double mConstraintTarget = 0;
    bool mLinear = false;
};

/// @brief Factory to create Constraint objects from input data.
/// @post The return vector will have the same size as @a aInput.
[[nodiscard]] std::vector<Constraint<const Plato::Functional::MeshProxy&>> make_constraints(const ValidatedConstraints& aInput);

/// @brief Helper for providing ROL a dual vector for constraints.
/// @note Currently, constraints are scalar, and so the dual vector always has dimension 1.
[[nodiscard]] std::unique_ptr<ROL::StdVector<double>> make_dual_vector();

namespace detail
{
[[nodiscard]] Constraint<const Plato::Functional::MeshProxy&> make_constraint(
    const Plato::Functional::Core::ValidatedInputTypeWrapper<Plato::constraint>& aConstraintInput);

}  // namespace detail
}  // namespace plato::functional::criteria::library

#endif
