#ifndef PLATO_FUNCTIONAL_TEST_DYNAMICVECTORTESTUTILITIES
#define PLATO_FUNCTIONAL_TEST_DYNAMICVECTORTESTUTILITIES

#include "DynamicVector.hpp"
#include "Function.hpp"
#include "Rosenbrock.hpp"
#include "TwoDTestTypes.hpp"

namespace plato::functional::integration_tests::utilities
{
[[nodiscard]] inline auto to_dynamic_vector(const Plato::Functional::Test::TwoDVector& aX)
    -> Plato::Functional::Core::DynamicVector<double>
{
    return Plato::Functional::Core::DynamicVector<double>{std::vector{aX(0), aX(1)}};
}

[[nodiscard]] inline auto make_rosenbrock_dynamic_vector_function(
    const Plato::Functional::Test::Rosenbrock& aRosenbrock)
{
    return Plato::Functional::make_function(
        [rosenbrock = aRosenbrock](const Plato::Functional::Core::DynamicVector<double>& x)
        { return rosenbrock.f(x[0], x[1]); },
        [rosenbrock = aRosenbrock](const Plato::Functional::Core::DynamicVector<double>& x)
        { return to_dynamic_vector(rosenbrock.df(x[0], x[1])); });
}

}  // namespace plato::functional::integration_tests::utilities

#endif
