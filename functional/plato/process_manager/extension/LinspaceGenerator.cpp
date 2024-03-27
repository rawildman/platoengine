#include "plato/process_manager/extension/LinspaceGenerator.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
namespace plato::process_manager::extension
{

std::vector<double> LinspaceGenerator::steps() const
{
    std::vector<double> tLinspace(mNumberOfSteps, mStartingMagnitude);
    std::generate_n(tLinspace.begin() + 1, tLinspace.size() - 1,
                    [n = mStartingMagnitude, r = mStepRatio]() mutable { return n = n * r; });
    return tLinspace;
}

}  // namespace plato::process_manager::extension
