#include "plato/integration_tests/utilities/CheckProcessorsMatchObjectives.hpp"

#include <gtest/gtest.h>

#include "plato/utilities/Zip.hpp"

namespace plato::integration_tests::utilities
{
void check_processors_match_objectives(const std::vector<unsigned int>& aNumberOfProcessors,
                                       const criteria::library::ValidatedObjectives& aObjectives)
{
    for (const auto [tNumberOfProcessors, tObjective] :
         plato::utilities::Zip{aNumberOfProcessors, aObjectives.rawInput()})
    {
        if (tObjective.rawInput().active.value_or(true))
        {
            EXPECT_EQ(tNumberOfProcessors, tObjective.rawInput().number_of_processors.value_or(1u));
        }
    }
};
}  // namespace plato::integration_tests::utilities
