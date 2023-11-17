#ifndef PLATO_FUNCTIONAL_COREUTILITIES
#define PLATO_FUNCTIONAL_COREUTILITIES

#include "Function.hpp"
#include "Penalty.hpp"
#include "Rosenbrock.hpp"
#include "TwoDTestTypes.hpp"

namespace Plato::Functional::Test
{

[[nodiscard]] inline auto make_rosenbrock_function(const Rosenbrock& aRosenbrock)
{
    return make_function([aRosenbrock](const TwoDVector& aX) { return aRosenbrock.f(aX(0), aX(1)); },
                         [aRosenbrock](const TwoDVector& aX) { return aRosenbrock.df(aX(0), aX(1)); });
}

[[nodiscard]] inline auto make_penalty_function(const Penalty& aPenalty)
{
    return make_function([aPenalty](const TwoDVector& aX) { return aPenalty.f(aX(0), aX(1)); },
                         [aPenalty](const TwoDVector& aX) { return aPenalty.df(aX(0), aX(1)); });
}

}  // namespace Plato::Functional::Test

#endif