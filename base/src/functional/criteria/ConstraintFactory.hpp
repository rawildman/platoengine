#ifndef PLATO_FUNCTIONAL_CONSTRAINTFACTORY
#define PLATO_FUNCTIONAL_CONSTRAINTFACTORY

#include <ROL_StdVector.hpp>
#include <string>

#include "Function.hpp"
#include "Plato_InputBlocks.hpp"

namespace Plato::Functional
{
struct MeshProxy;
}

namespace Plato::Functional::ConstraintFactory
{
/// @brief Holds members for defining a Constraint
/// @tparam FunctionArg The argument of the function used to define the constraint.
///   Typically, this is either MeshProxy or a vector type such as `ROL::StdVector`.
template <typename FunctionArg>
struct Constraint
{
    using ConstraintFunction = Function<double, ROL::StdVector<double>, FunctionArg>;

    std::string mName;
    ConstraintFunction mConstraintFunction;
    double mConstraintTarget = 0;
    bool mLinear = false;
};

/// @brief Factory to create Constraint objects from input data.
/// @post The return vector will have the same size as @a aInput.
[[nodiscard]] std::vector<Constraint<const MeshProxy&>> make_constraints(const std::vector<Plato::constraint>& aInput);

/// @brief Helper for providing ROL a dual vector for constraints.
/// @note Currently, constraints are scalar, and so the dual vector always has dimension 1.
[[nodiscard]] std::unique_ptr<ROL::StdVector<double>> make_dual_vector();

namespace detail
{

[[nodiscard]] Constraint<const MeshProxy&> make_constraint(const Plato::constraint& aConstraintInput);

}  // namespace detail
}  // namespace Plato::Functional::ConstraintFactory

#endif
