#ifndef PLATO_FUNCTIONAL_TEST_PENALTY
#define PLATO_FUNCTIONAL_TEST_PENALTY

#include <cmath>

#include "TwoDTestTypes.hpp"

namespace Plato::Functional::Test
{

class Penalty
{
   public:
    Penalty(const double aXMin, const double aPower) : mXMin(aXMin), mPower(aPower) {}

    [[nodiscard]] TwoDVector f(const double aX, const double aY) const
    {
        return makeTwoDVector(mXMin + (1.0 - mXMin) * std::pow(aX, mPower),
                              mXMin + (1.0 - mXMin) * std::pow(aY, mPower));
    }

    [[nodiscard]] TwoDMatrix df(const double aX, const double aY) const
    {
        return makeTwoDMatrix(mPower * (1.0 - mXMin) * std::pow(aX, mPower - 1.0), 0.0, 0.0,
                              mPower * (1.0 - mXMin) * std::pow(aY, mPower - 1.0));
    }

   private:
    double mXMin = 1e-3;
    double mPower = 3.0;
};

}  // namespace Plato::Functional::Test

#endif
