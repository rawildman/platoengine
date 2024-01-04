#ifndef PLATO_FUNCTIONAL_OBJECTIVEFACTORY
#define PLATO_FUNCTIONAL_OBJECTIVEFACTORY

#include <ROL_StdVector.hpp>

#include "Aggregate.hpp"
#include "Function.hpp"
#include "Plato_InputBlocks.hpp"
#include "ValidatedInputTypeWrapper.hpp"

namespace Plato::Functional
{
struct MeshProxy;
}

namespace Plato::Functional::ObjectiveFactory
{
using ValidatedObjectives =
    Core::ValidatedInputTypeWrapper<std::vector<Core::ValidatedInputTypeWrapper<Plato::objective>>>;

using ObjectiveFunction = Function<double, ROL::StdVector<double>, const MeshProxy&>;
using AggregateObjective = Aggregate<double, ROL::StdVector<double>, const MeshProxy&>;

namespace detail
{
[[nodiscard]] AggregateObjective make_aggregate(const ValidatedObjectives& aInput);
}

[[nodiscard]] ObjectiveFunction make_aggregate_objective_function(const ValidatedObjectives& aInput);

}  // namespace Plato::Functional::ObjectiveFactory

#endif
