#ifndef PLATO_FUNCTIONAL_TEST_ROLTESTUTILITIES
#define PLATO_FUNCTIONAL_TEST_ROLTESTUTILITIES

#include <ROL_StdVector.hpp>

#include "Rosenbrock.hpp"
#include "TwoDTestTypes.hpp"

namespace Plato::Functional::Test
{
[[nodiscard]] inline ROL::StdVector<double> toROLVector(const TwoDVector& aX)
{
    return ROL::StdVector<double>{aX(0), aX(1)};
}

[[nodiscard]] inline auto make_rosenbrock_rol_vector_function(const Rosenbrock& aRosenbrock)
{
    return make_function(
        [rosenbrock = aRosenbrock](const ROL::StdVector<double>& x) { return rosenbrock.f(x[0], x[1]); },
        [rosenbrock = aRosenbrock](const ROL::StdVector<double>& x) { return toROLVector(rosenbrock.df(x[0], x[1])); });
}

}  // namespace Plato::Functional::Test

#endif
