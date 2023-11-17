#ifndef PLATO_FUNCTIONAL_OBJECTIVEFACTORY
#define PLATO_FUNCTIONAL_OBJECTIVEFACTORY

#include <ROL_StdVector.hpp>

#include "Aggregate.hpp"
#include "Function.hpp"
#include "Plato_InputBlocks.hpp"

namespace Plato::Functional
{
struct MeshProxy;
}

namespace Plato::Functional::ObjectiveFactory
{
using ObjectiveFunction = Function<double, ROL::StdVector<double>, const MeshProxy&>;
using AggregateObjective = Aggregate<double, ROL::StdVector<double>, const MeshProxy&>;

namespace detail
{
[[nodiscard]] AggregateObjective make_aggregate(const std::vector<Plato::objective>& aInput);
}

/// @pre @a aObjectiveInput is valid according to affirm_valid_input.
[[nodiscard]] ObjectiveFunction make_objective_function(const Plato::objective& aObjectiveInput);

/// @pre @a aInput is valid according to affirm_valid_input.
[[nodiscard]] ObjectiveFunction make_aggregate_objective_function(const std::vector<Plato::objective>& aInput);

/// @throw Exception If @a aInput does not contain valid input such as an `app` field.
void affirm_valid_input(const std::vector<Plato::objective>& aInput);
}  // namespace Plato::Functional::ObjectiveFactory

#endif
