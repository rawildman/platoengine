#include <gtest/gtest.h>

#include "plato/criteria/library/ObjectiveFactory.hpp"
#include "plato/integration_tests/utilities/CheckProcessorsMatchObjectives.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"
#include "plato/test_utilities/InputGeneration.hpp"
#include "plato/utilities/Exception.hpp"

namespace plato::integration_tests::parallel
{
namespace
{
constexpr auto kNumRanks = int{4};

process_manager::library::ValidatedInput create_one_objective_test_input()
{
    namespace pftu = plato::test_utilities;

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
    const std::string tOptimizerInput = pftu::create_valid_example_rol_optimization_string();

    return process_manager::library::parse_and_validate(tObjectiveInput + tGeometryInput + tOptimizerInput);
}
}  // namespace

TEST(ObjectiveFactory, MPISize)
{
    auto tComm = boost::mpi::communicator{};
    EXPECT_EQ(tComm.size(), kNumRanks);
}

TEST(ObjectiveFactory, InvalidParallelAggregate)
{
    EXPECT_THROW(const process_manager::library::ValidatedInput tData = create_one_objective_test_input(),
                 plato::utilities::Exception);
}

TEST(ObjectiveFactory, NumberOfProcessors)
{
    namespace pitu = plato::integration_tests::utilities;
    auto tInput = test_utilities::create_valid_example_input();
    {
        // Set number_of_processors to 4
        tInput.mObjectives.front().number_of_processors = 4u;
        auto tValidInput = process_manager::library::make_validated_input(tInput);
        pitu::check_processors_match_objectives(
            criteria::library::number_of_processors_per_objective(tValidInput.objectives()), tValidInput.objectives());
    }
    {
        // Add another objective with 1 processor
        tInput.mObjectives.front().number_of_processors = 3u;
        tInput.mObjectives.push_back(test_utilities::create_valid_example_objective());
        auto tValidInput = process_manager::library::make_validated_input(tInput);
        pitu::check_processors_match_objectives(
            criteria::library::number_of_processors_per_objective(tValidInput.objectives()), tValidInput.objectives());
    }
}

}  // namespace plato::integration_tests::parallel
