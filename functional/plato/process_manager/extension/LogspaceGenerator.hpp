#ifndef PLATO_PROCESSMANAGER_EXTENSION_LINSPACEGENERATOR
#define PLATO_PROCESSMANAGER_EXTENSION_LINSPACEGENERATOR

#include <vector>

namespace plato::process_manager::extension
{
/// @brief The purpose of this is to generate a set of logarithmically spaced values.
///
/// The main use is in the gradient checker for generating finite difference step sizes.
struct LogspaceGenerator
{
    double mStartingMagnitude = 1;
    double mStepRatio = 0.1;
    unsigned int mNumberOfSteps = 10;

    [[nodiscard]] std::vector<double> steps() const;
};
}  // namespace plato::process_manager::extension

#endif
