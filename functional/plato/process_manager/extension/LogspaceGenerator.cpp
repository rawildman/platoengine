#include "plato/process_manager/extension/LogspaceGenerator.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>

namespace plato::process_manager::extension
{
std::vector<double> LogspaceGenerator::steps() const
{
    if (mNumberOfSteps == 0)
    {
        return {};
    }
    std::vector<double> tLogspace(mNumberOfSteps, mStartingMagnitude);
    std::generate_n(tLogspace.begin() + 1, tLogspace.size() - 1,
                    [n = mStartingMagnitude, r = mStepRatio]() mutable { return n = n * r; });
    return tLogspace;
}

}  // namespace plato::process_manager::extension
