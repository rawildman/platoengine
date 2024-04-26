#include "plato/integration_tests/utilities/CheckProcessorsMatchObjectives.hpp"

#include <gtest/gtest.h>

#include "plato/utilities/Zip.hpp"

namespace plato::integration_tests::utilities
{
void check_processors_match_objectives(const std::vector<unsigned int>& aNumberOfProcessors,
                                       const criteria::library::ValidatedObjectives& aObjectives)
{
    ASSERT_EQ(aNumberOfProcessors.size(), aObjectives.rawInput().size());
    for (const auto [tNumberOfProcessors, tObjective] :
         plato::utilities::Zip{aNumberOfProcessors, aObjectives.rawInput()})
    {
        EXPECT_EQ(tNumberOfProcessors, tObjective.rawInput().number_of_processors.value_or(1u));
    }
};
}  // namespace plato::integration_tests::utilities
