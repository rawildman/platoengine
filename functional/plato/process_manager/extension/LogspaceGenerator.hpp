#ifndef PLATO_PROCESSMANAGER_EXTENSION_LINSPACEGENERATOR
#define PLATO_PROCESSMANAGER_EXTENSION_LINSPACEGENERATOR

#include <vector>

namespace plato::process_manager::extension
{
struct LogspaceGenerator
{
    double mStartingMagnitude = 1;
    double mStepRatio = 0.1;
    unsigned int mNumberOfSteps = 10;

    [[nodiscard]] std::vector<double> steps() const;
};
}  // namespace plato::process_manager::extension

#endif
