#include <gtest/gtest.h>

#include "ConstraintFactory.hpp"
#include "ConstraintValidation.hpp"
#include "Exception.hpp"
#include "InputGeneration.hpp"
#include "ValidatedInput.hpp"

TEST(ConstraintFactory, ValidConstraint)
{
    namespace pfv = Plato::Functional::Validation;
    namespace pftu = Plato::Functional::TestUtilities;

    const std::string tConstraintInput = pftu::create_valid_example_constraint_string();
    const std::string tGeometryInput = pftu::create_valid_density_topology_geometry_string();
    const std::string tOptimizerInput = pftu::create_valid_example_optimization_parameters_string();
    const std::string tObjectiveInput = pftu::create_valid_example_objective_string();
    const pfv::ValidatedInput tData =
        pfv::parse_and_validate(tConstraintInput + tGeometryInput + tOptimizerInput + tObjectiveInput);

    ASSERT_EQ(tData.constraints().rawInput().size(), 1);
    const auto tConstraint = Plato::Functional::ConstraintFactory::detail::make_constraint(tData.constraints().rawInput().front());
    EXPECT_TRUE(tConstraint.mLinear);
    EXPECT_EQ(tConstraint.mConstraintTarget, 13.0);
}
