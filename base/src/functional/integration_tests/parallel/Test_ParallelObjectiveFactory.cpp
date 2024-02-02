#include <gtest/gtest.h>

#include "Exception.hpp"
#include "ObjectiveFactory.hpp"
#include "ValidatedInput.hpp"
#include "InputGeneration.hpp"

namespace
{
constexpr auto kNumRanks = int{2};

Plato::Functional::Validation::ValidatedInput create_one_objective_test_input()
{
    namespace pfv = Plato::Functional::Validation;
    namespace pftu = Plato::Functional::TestUtilities;

    // Input for the actual test
    const std::string tObjectiveInput =
        R"(
          begin objective test1
            app nodal_sum
            aggregation_weight 42.0
          end
       )";
    // Other inputs to make sure we have valid input
    const std::string tGeometryInput = pftu::create_valid_density_topology_geometry_string();
    const std::string tOptimizerInput = pftu::create_valid_example_optimization_parameters_string();

    return pfv::parse_and_validate(tObjectiveInput + tGeometryInput + tOptimizerInput);
}
}

TEST(ObjectiveFactory, MPISize)
{
    auto tComm = boost::mpi::communicator{};
    EXPECT_EQ(tComm.size(), kNumRanks);
}

TEST(ObjectiveFactory, InvalidParallelAggregate)
{
    namespace pfv = Plato::Functional::Validation;

    EXPECT_THROW(const pfv::ValidatedInput tData = create_one_objective_test_input(), Plato::Functional::Exception);
}
