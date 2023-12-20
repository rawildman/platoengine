#include <gtest/gtest.h>

#include <ROL_Algorithm.hpp>
#include <ROL_Solver.hpp>

#include "BrickShapeGeometry.hpp"
#include "Exception.hpp"
#include "GeometryFactory.hpp"
#include "InputGeneration.hpp"
#include "InputParser.hpp"
#include "MeshProxy.hpp"
#include "ObjectiveFactory.hpp"
#include "OptimizerFactory.hpp"
#include "PlatoProblem.hpp"
#include "ValidatedInput.hpp"

TEST(PlatoProblem, ParsePlatoProblemEvaluateObjective)
{
    namespace pf = Plato::Functional;
    const std::string tInput = pf::TestUtilities::create_valid_brick_shape_geometry_string() +
                               pf::TestUtilities::create_valid_example_objective_string() +
                               pf::TestUtilities::create_valid_example_optimization_parameters_string();

    const pf::Validation::ValidatedInput tData{pf::parse_and_validate(tInput)};

    const pf::PlatoProblem tProblem = pf::make_plato_problem(tData);
    const auto tGeometry = pf::GeometryFactory::make_geometry_data(tData.geometry());

    // Test Geometry
    const ROL::StdVector<double> tBoundingBox{0, 0, 0, 1, 1, 1};
    const pf::MeshProxy tGeomProxy = tGeometry.mCompute.f(tBoundingBox);
    const pf::MeshProxy tPlatoProblemGeomProxy = tProblem.mGeometry.mCompute.f(tBoundingBox);
    EXPECT_EQ(tGeomProxy.mFileName, tPlatoProblemGeomProxy.mFileName);

    // Test Objective
    const auto tObjective = pf::ObjectiveFactory::make_aggregate_objective_function(tData.objectives());
    EXPECT_EQ(tObjective.f(tGeomProxy), tProblem.mObjective.f(tGeomProxy));

    std::filesystem::remove(tGeomProxy.mFileName);
}

TEST(PlatoProblem, InputFileToROLObjective)
{
    namespace pf = Plato::Functional;
    constexpr double tWeight = 42.0;

    const std::string tInput = pf::TestUtilities::create_valid_brick_shape_geometry_string() +
                               " begin objective test"
                               " active true"
                               " app nodal_sum"
                               " number_of_processors 4"
                               " input_files test-input.inp"
                               " aggregation_weight " +
                               std::to_string(tWeight) + " objective_type minimize" + " end" +
                               pf::TestUtilities::create_valid_example_optimization_parameters_string();

    const pf::Validation::ValidatedInput tData{pf::parse_and_validate(tInput)};

    pf::PlatoProblem tProblem = pf::make_plato_problem(tData);
    std::unique_ptr<pf::ROLObjectiveFunction> tObjectiveFunction = pf::make_rol_objective(tProblem);
    const ROL::StdVector<double> tBoundingBox{0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
    double tTolerance = 1e-8;

    constexpr double tNodalSum = 12.0;
    EXPECT_DOUBLE_EQ(tObjectiveFunction->value(tBoundingBox, tTolerance), tWeight * tNodalSum);

    pf::BrickDesign tDesign;
    EXPECT_EQ(*tProblem.mGeometry.mInitialGuess->getVector(), *pf::detail::to_rol_std_vector(tDesign).getVector());

    std::filesystem::remove("my_mesh.exo");
}

TEST(PlatoProblem, InputFileToROLConstraint)
{
    namespace pf = Plato::Functional;
    const std::string tInput = pf::TestUtilities::create_valid_brick_shape_geometry_string() +
                               pf::TestUtilities::create_valid_example_objective_string() +
                               R"(
                                begin constraint test
                                  active true
                                  app nodal_sum
                                  equal_to 2
                                end
                              )" +
                               pf::TestUtilities::create_valid_example_optimization_parameters_string();

    const pf::Validation::ValidatedInput tData{pf::parse_and_validate(tInput)};

    pf::PlatoProblem tProblem = pf::make_plato_problem(tData);
    const auto tConstraints = pf::make_rol_constraints(tProblem);
    ASSERT_EQ(tConstraints.size(), 1);

    const ROL::StdVector<double> tBoundingBox{0, 0, 0, 1, 1, 1};
    ROL::StdVector<double> tResult{0};
    const auto tGold = std::vector{-2.0};
    double tTolerance = 1e-8;

    ASSERT_NE(tConstraints.front(), nullptr);
    tConstraints.front()->value(tResult, tBoundingBox, tTolerance);

    EXPECT_EQ(*tResult.getVector(), tGold);
    std::filesystem::remove("my_mesh.exo");
}

TEST(PlatoProblem, InputFileToROLSolver)
{
    namespace pf = Plato::Functional;

    const std::string tInput = pf::TestUtilities::create_valid_brick_shape_geometry_string() +
                               pf::TestUtilities::create_valid_example_objective_string() +
                               R"(
                                begin constraint test
                                  active true
                                  app nodal_sum
                                  equal_to 2
                                end
                              )" +
                               pf::TestUtilities::create_valid_example_optimization_parameters_string();

    const pf::Validation::ValidatedInput tData{pf::parse_and_validate(tInput)};
    const pf::PlatoProblem tPlatoProblem = pf::make_plato_problem(tData);
    Teuchos::ParameterList tROLOptions = tPlatoProblem.mROLOptions;
    const auto tROLProblem = Teuchos::RCP{pf::make_rol_problem(tPlatoProblem).release()};
    const ROL::Solver<double> tSolver = pf::make_rol_solver(tROLOptions, std::move(tROLProblem));

    EXPECT_EQ(tSolver.getAlgorithmState()->iter, 0);
}

TEST(PlatoProblem, ParseAndValidateInvalidInput)
{
    namespace pf = Plato::Functional;
    const std::string tInput;
    EXPECT_THROW(const pf::Validation::ValidatedInput tData = pf::parse_and_validate(""), pf::Exception);
}