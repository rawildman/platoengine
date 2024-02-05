#include <gtest/gtest.h>

#include "GeometryFactory.hpp"
#include "InputGeneration.hpp"
#include "STKUtilities.hpp"
#include "ValidatedInput.hpp"

namespace plato::functional::main::library::unittest
{
TEST(GeometryFactory, ValidBrickShapeGeometry)
{
    namespace pf = Plato::Functional;
    Plato::PlatoInput tInput;
    tInput.mBrickShapeGeometry = plato::functional::test_utilities::create_valid_brick_shape_geometry();
    tInput.mObjectives = {plato::functional::test_utilities::create_valid_example_objective()};
    tInput.mOptimizationParameters = plato::functional::test_utilities::create_valid_example_optimization_parameters();

    const ValidatedInput tData = make_validated_input(tInput);
    EXPECT_NO_THROW(auto tUnused = plato::functional::geometry::library::make_geometry_data(tData.geometry()));
}

TEST(GeometryFactory, ValidTopology)
{
    namespace pf = Plato::Functional;

    Plato::PlatoInput tInput;
    tInput.mDensityTopology = plato::functional::test_utilities::create_valid_density_topology_geometry();
    tInput.mObjectives = {plato::functional::test_utilities::create_valid_example_objective()};
    tInput.mOptimizationParameters = plato::functional::test_utilities::create_valid_example_optimization_parameters();

    const std::filesystem::path tMeshFileName{tInput.mDensityTopology.value().mesh_name.value().mName};
    pf::write_mesh(tMeshFileName, pf::create_mesh("generated:3x3x4|bbox:-1,-2,-1,2,1,2"));

    const ValidatedInput tData = make_validated_input(tInput);

    EXPECT_NO_THROW(auto tUnused = plato::functional::geometry::library::make_geometry_data(tData.geometry()));

    std::filesystem::remove(tMeshFileName);
}
}
