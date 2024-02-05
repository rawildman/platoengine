#include <gtest/gtest.h>

#include "Exception.hpp"
#include "InputGeneration.hpp"
#include "ObjectiveFactory.hpp"
#include "ValidatedInput.hpp"

namespace plato::functional::integration_tests::parallel
{
namespace
{
constexpr auto kNumRanks = int{4};

Plato::Functional::Validation::ValidatedInput create_one_objective_test_input()
{
    namespace pfv = Plato::Functional::Validation;
    namespace pftu = plato::functional::test_utilities;

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
}  // namespace

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
}  // namespace plato::functional::integration_tests::parallel
