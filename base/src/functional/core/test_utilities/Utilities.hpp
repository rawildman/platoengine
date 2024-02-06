#ifndef PLATO_FUNCTIONAL_COREUTILITIES
#define PLATO_FUNCTIONAL_COREUTILITIES

#include "Function.hpp"
#include "Penalty.hpp"
#include "Rosenbrock.hpp"
#include "TwoDTestTypes.hpp"

namespace plato::functional::core::test_utilities
{
[[nodiscard]] inline auto make_rosenbrock_function(const plato::functional::test_utilities::Rosenbrock& aRosenbrock)
{
    namespace pftu = plato::functional::test_utilities;
    return make_function([aRosenbrock](const pftu::TwoDVector& aX) { return aRosenbrock.f(aX(0), aX(1)); },
                         [aRosenbrock](const pftu::TwoDVector& aX) { return aRosenbrock.df(aX(0), aX(1)); });
}

[[nodiscard]] inline auto make_penalty_function(const plato::functional::test_utilities::Penalty& aPenalty)
{
    namespace pftu = plato::functional::test_utilities;
    return make_function([aPenalty](const pftu::TwoDVector& aX) { return aPenalty.f(aX(0), aX(1)); },
                         [aPenalty](const pftu::TwoDVector& aX) { return aPenalty.df(aX(0), aX(1)); });
}

}  // namespace plato::functional::core::test_utilities

#endif
