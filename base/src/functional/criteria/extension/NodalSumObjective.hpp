#ifndef PLATO_FUNCTIONAL_NODALSUMOBJECTIVE
#define PLATO_FUNCTIONAL_NODALSUMOBJECTIVE

#include "DynamicVector.hpp"
#include "Function.hpp"
#include "MeshProxy.hpp"

namespace Plato::Functional
{
/// @brief Computes the sum of all nodal coordinates. The main purpose of this
///  class is for implementing a geometry sensitivity check.
struct NodalSumObjective
{
    [[nodiscard]] double f(const MeshProxy& aMeshProxy) const;
    [[nodiscard]] Core::DynamicVector<double> df(const MeshProxy& aMeshProxy) const;
};

/// @brief Creates a Function object from a NodalSumObjective
[[nodiscard]] auto make_nodal_sum_function() -> Function<double, Core::DynamicVector<double>, const MeshProxy&>;

}  // namespace Plato::Functional

#endif
