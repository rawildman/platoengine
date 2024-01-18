#ifndef PLATO_FUNCTIONAL_CONSTRAINTFACTORY
#define PLATO_FUNCTIONAL_CONSTRAINTFACTORY

#include <ROL_StdVector.hpp>
#include <string>

#include "Function.hpp"
#include "Plato_InputBlocks.hpp"
#include "ValidatedInputTypeWrapper.hpp"
#include "DynamicVector.hpp"

namespace Plato::Functional
{
struct MeshProxy;
}

namespace Plato::Functional::ConstraintFactory
{
using ValidatedConstraints =
    Core::ValidatedInputTypeWrapper<std::vector<Core::ValidatedInputTypeWrapper<Plato::constraint>>>;

/// @brief Holds members for defining a Constraint
/// @tparam FunctionArg The argument of the function used to define the constraint.
///   Typically, this is either MeshProxy or a vector type such as DynamicVector.
template <typename FunctionArg>
struct Constraint
{
    using ConstraintFunction = Function<double, Core::DynamicVector<double>, FunctionArg>;

    std::string mName;
    ConstraintFunction mConstraintFunction;
    double mConstraintTarget = 0;
    bool mLinear = false;
};

/// @brief Factory to create Constraint objects from input data.
/// @post The return vector will have the same size as @a aInput.
[[nodiscard]] std::vector<Constraint<const MeshProxy&>> make_constraints(const ValidatedConstraints& aInput);

/// @brief Helper for providing ROL a dual vector for constraints.
/// @note Currently, constraints are scalar, and so the dual vector always has dimension 1.
[[nodiscard]] std::unique_ptr<ROL::StdVector<double>> make_dual_vector();

namespace detail
{
[[nodiscard]] Constraint<const MeshProxy&> make_constraint(
    const Core::ValidatedInputTypeWrapper<Plato::constraint>& aConstraintInput);

}  // namespace detail
}  // namespace Plato::Functional::ConstraintFactory

#endif
