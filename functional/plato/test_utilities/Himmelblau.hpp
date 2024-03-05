#ifndef PLATO_TEST_UTILITIES_HIMMELBLAU
#define PLATO_TEST_UTILITIES_HIMMELBLAU

#include <cmath>
#include <type_traits>

#include "plato/test_utilities/TwoDTestTypes.hpp"

namespace plato::test_utilities
{
class Himmelblau
{
   public:
    Himmelblau() = default;

    [[nodiscard]] double f(const double aX, const double aY) const
    {
        return std::pow(aX * aX + aY - mA, 2.0) + std::pow(aX + aY * aY - mB, 2.0);
    }

    [[nodiscard]] TwoDVector df(const double aX, const double aY) const
    {
        const double tParensOne = aX * aX + aY - mA;
        const double tParensTwo = aX + aY * aY - mB;

        const double tXterm = 4 * aX * tParensOne + 2 * tParensTwo;
        const double tYterm = 2 * tParensOne + 4 * aY * tParensTwo;

        return makeTwoDVector(tXterm, tYterm);
    }

   private:
    double mA = 11.0;
    double mB = 7.0;
};

}  // namespace plato::test_utilities

#endif
