#include <gtest/gtest.h>

#include <boost/mpi/communicator.hpp>

#include "ObjectiveValidation.hpp"
#include "Plato_InputBlocks.hpp"

namespace
{
constexpr auto kNumRanks = int{3};
}

TEST(ParallelObjectiveValidation, MPISize)
{
    auto tComm = boost::mpi::communicator{};
    EXPECT_EQ(tComm.size(), kNumRanks);
}

TEST(ParallelObjectiveValidation, ValidateMPIRanksVsNumberOfObjectives)
{
    namespace pfcd = Plato::Functional::Criteria::detail;

    // One objective and three ranks
    const Plato::objective tObjective;
    EXPECT_TRUE(pfcd::validate_number_of_ranks_vs_objectives({tObjective}).has_value());

    // Three objectives and three ranks
    EXPECT_FALSE(pfcd::validate_number_of_ranks_vs_objectives({tObjective, tObjective, tObjective}).has_value());

    // Four objectives and three ranks
    EXPECT_FALSE(pfcd::validate_number_of_ranks_vs_objectives({tObjective, tObjective, tObjective, tObjective}).has_value());
}
