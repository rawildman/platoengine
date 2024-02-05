#ifndef PLATO_FUNCTIONAL_NODALSUMOBJECTIVE
#define PLATO_FUNCTIONAL_NODALSUMOBJECTIVE

#include "DynamicVector.hpp"
#include "Function.hpp"
#include "MeshProxy.hpp"

namespace plato::functional::criteria::extension
{
/// @brief Computes the sum of all nodal coordinates. The main purpose of this
///  class is for implementing a geometry sensitivity check.
struct NodalSumObjective
{
    [[nodiscard]] double f(const Plato::Functional::MeshProxy& aMeshProxy) const;
    [[nodiscard]] Plato::Functional::Core::DynamicVector<double> df(
        const Plato::Functional::MeshProxy& aMeshProxy) const;
};

/// @brief Creates a Function object from a NodalSumObjective
[[nodiscard]] auto make_nodal_sum_function() -> Plato::Functional::
    Function<double, Plato::Functional::Core::DynamicVector<double>, const Plato::Functional::MeshProxy&>;

}  // namespace plato::functional::criteria::extension

#endif
