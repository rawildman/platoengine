#include "GeometryFactory.hpp"

#include "BrickShapeGeometry.hpp"
#include "DensityTopology.hpp"
#include "Exception.hpp"
#include "GeometryRegistration.hpp"
#include "Plato_InputBlocks.hpp"

namespace Plato::Functional::GeometryFactory
{

namespace
{

GeometryFactory::GeometryInput to_geometry_input(const Plato::PlatoInput& aInput)
{
    if (aInput.mBrickShapeGeometry)
    {
        return aInput.mBrickShapeGeometry.value();
    }
    else if (aInput.mDensityTopology)
    {
        return aInput.mDensityTopology.value();
    }
    throw Exception("No geometry block was defined.");
}

std::string block_name(const GeometryFactory::GeometryInput& aInput)
{
    return std::visit(
        [](const auto& aObj) -> std::string
        {
            using InputType = std::decay_t<decltype(aObj)>;
            return Plato::block_name<InputType>();
        },
        aInput);
    throw Exception("No geometry block was defined.");
}

}  // namespace

GeometryFunction make_geometry_function(const Plato::PlatoInput& aInput)
{
    const GeometryInput tGeometryInput = to_geometry_input(aInput);

    if (const auto tIter =
            detail::registered_functions<GeometryFunction, GeometryInput>().find(block_name(tGeometryInput));
        tIter != detail::registered_functions<GeometryFunction, GeometryInput>().end())
    {
        return tIter->second(tGeometryInput);
    }
    else
    {
        throw Plato::Functional::Exception{"Unknown geometry"};
    }
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
