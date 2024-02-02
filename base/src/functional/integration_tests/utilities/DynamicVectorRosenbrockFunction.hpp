#ifndef PLATO_FUNCTIONAL_TEST_DYNAMICVECTORTESTUTILITIES
#define PLATO_FUNCTIONAL_TEST_DYNAMICVECTORTESTUTILITIES

#include "DynamicVector.hpp"
#include "Function.hpp"
#include "Rosenbrock.hpp"
#include "TwoDTestTypes.hpp"

namespace Plato::Functional::Test
{
[[nodiscard]] inline auto to_dynamic_vector(const TwoDVector& aX) -> Plato::Functional::Core::DynamicVector<double>
{
    return Plato::Functional::Core::DynamicVector<double>{std::vector{aX(0), aX(1)}};
}

[[nodiscard]] inline auto make_rosenbrock_dynamic_vector_function(const Rosenbrock& aRosenbrock)
{
    return make_function([rosenbrock = aRosenbrock](const Core::DynamicVector<double>& x)
                         { return rosenbrock.f(x[0], x[1]); },
                         [rosenbrock = aRosenbrock](const Core::DynamicVector<double>& x)
                         { return to_dynamic_vector(rosenbrock.df(x[0], x[1])); });
}

}  // namespace Plato::Functional::Test

#endif
