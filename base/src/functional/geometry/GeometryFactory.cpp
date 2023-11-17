#include "GeometryFactory.hpp"

#include "BrickShapeGeometry.hpp"
#include "DensityTopology.hpp"
#include "Exception.hpp"
#include "Plato_InputBlocks.hpp"

namespace Plato::Functional::GeometryFactory
{

GeometryFunction make_geometry_function(const Plato::PlatoInput& aInput)
{
    if (aInput.mBrickShapeGeometry)
    {
        if (aInput.mBrickShapeGeometry->mesh_name)
        {
            return make_brick_shape_geometry(BrickShapeGeometry{aInput.mBrickShapeGeometry->mesh_name->mName});
        }
        else
        {
            throw Exception("brick_shape_geometry requires a mesh_name.");
        }
    }
    else if (aInput.mDensityTopology)
    {
        if (aInput.mDensityTopology->mesh_name)
        {
            return make_topology_geometry(DensityTopology{aInput.mDensityTopology.value()});
        }
        else
        {
            throw Exception("density_topology requires a mesh_name.");
        }
    }
    throw Exception("No geometry block was defined.");
}

std::unique_ptr<ROL::StdVector<double>> make_initial_guess(const Plato::PlatoInput& aInput)
{
    if (aInput.mBrickShapeGeometry)
    {
        return BrickShapeGeometry::initialGuess();
    }
    else if (aInput.mDensityTopology)
    {
        return DensityTopology::initialGuess(aInput.mDensityTopology->mesh_name->mName);
    }
    throw Exception("No geometry block was defined.");
}

ROL::StdBoundConstraint<double> make_bound_constraint(const Plato::PlatoInput& aInput)
{
    if (aInput.mBrickShapeGeometry)
    {
        auto [lowerBound, upperBound] = BrickShapeGeometry::bounds();
        return ROL::StdBoundConstraint<double>{lowerBound, upperBound};
    }
    else if (aInput.mDensityTopology)
    {
        auto [lowerBound, upperBound] = DensityTopology::bounds(aInput.mDensityTopology->mesh_name->mName);
        return ROL::StdBoundConstraint<double>{lowerBound, upperBound};
    }
    throw Exception("No geometry block was defined.");
}

std::function<void(const ROL::StdVector<double>&)> make_output_function(const Plato::PlatoInput& aInput)
{
    if (aInput.mBrickShapeGeometry)
    {
        return [](const ROL::StdVector<double>& x) { BrickShapeGeometry::output(x); };
    }
    else if (aInput.mDensityTopology)
    {
        if (!aInput.mDensityTopology->output_name)
        {
            throw Exception("density_topology requires an output_name.");
        }
        return [tInputMeshName = aInput.mDensityTopology->mesh_name->mName,
                tOutputMeshName = aInput.mDensityTopology->output_name->mName](const ROL::StdVector<double>& x)
        { DensityTopology::output(tInputMeshName, x, tOutputMeshName); };
    }
    throw Exception("No geometry block was defined.");
}
}  // namespace Plato::Functional::GeometryFactory
