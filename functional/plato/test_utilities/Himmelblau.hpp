#ifndef PLATO_TEST_UTILITIES_HIMMELBLAU
#define PLATO_TEST_UTILITIES_HIMMELBLAU

#include <type_traits>

#include "plato/test_utilities/TwoDTestTypes.hpp"

namespace plato::test_utilities
{
namespace detail
{
constexpr double kA = 11.0;
constexpr double kB = 7.0;
}  // namespace detail

struct Himmelblau
{
    [[nodiscard]] constexpr double f(const double aX, const double aY) const
    {
        const double tParensOne = aX * aX + aY - detail::kA;
        const double tParensTwo = aX + aY * aY - detail::kB;
        return tParensOne * tParensOne + tParensTwo * tParensTwo;
    }

    [[nodiscard]] TwoDVector df(const double aX, const double aY) const
    {
        const double tParensOne = aX * aX + aY - detail::kA;
        const double tParensTwo = aX + aY * aY - detail::kB;

        const double tXterm = 4 * aX * tParensOne + 2 * tParensTwo;
        const double tYterm = 2 * tParensOne + 4 * aY * tParensTwo;

        return makeTwoDVector(tXterm, tYterm);
    }
};

}  // namespace plato::test_utilities

#endif
