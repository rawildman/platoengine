#include "BrickShapeGeometry.hpp"

#include <filesystem>
#include <iomanip>
#include <sstream>

#include "Exception.hpp"
#include "GeometryRegistration.hpp"
#include "GeometryValidation.hpp"
#include "JacobianColumnEvaluator.hpp"
#include "Plato_InputBlocks.hpp"
#include "STKUtilities.hpp"
#include "ValidationRegistration.hpp"

namespace Plato::Functional
{
namespace
{
constexpr int kNumDims = 3;
constexpr int kNumDesignParameters = 6;
const std::vector<double> kLowerBounds = {-10.0, -10.0, -10.0, 1e-2, 1e-2, 1e-2};  // Arbitrary
const std::vector<double> kUpperBounds = {10.0, 10.0, 10.0, 1e2, 1e2, 1e2};        // Arbitrary

[[nodiscard]] std::filesystem::path mesh_path(const GeometryFactory::GeometryInput& aGeometryInput)
{
    if (!std::get<Plato::brick_shape_geometry>(aGeometryInput).mesh_name)
    {
        throw Plato::Functional::Exception{"A brick mesh must have a mesh_name."};
    }
    return std::get<Plato::brick_shape_geometry>(aGeometryInput).mesh_name.value().mName;
}

[[nodiscard]] std::function<void(const ROL::StdVector<double>&)> make_output()
{
    return [](const ROL::StdVector<double>& aSolution) { return BrickShapeGeometry::output(aSolution); };
}

[[maybe_unused]] static auto kBrickShapeGeometryRegistration = Plato::Functional::GeometryFactory::GeometryRegistration{
    Plato::block_name<Plato::brick_shape_geometry>(), [](const GeometryFactory::GeometryInput& aGeometryInput)
    {
        return GeometryFactory::FactoryTypes{make_brick_shape_geometry(BrickShapeGeometry{mesh_path(aGeometryInput)}),
                                             BrickShapeGeometry::initialGuess(), BrickShapeGeometry::bounds(),
                                             make_output()};
    }};

[[maybe_unused]] static auto kBrickShapeValidationRegistration = Validation::Registration<Plato::brick_shape_geometry>{
    [](const Plato::brick_shape_geometry& aInput) { return Geometry::detail::validate_mesh_name(aInput); }};
}  // namespace

BrickShapeGeometry::BrickShapeGeometry(std::filesystem::path aFileName, const std::optional<double> aDiscretizationSize)
    : mFileName(std::move(aFileName)), mDiscretizationSize(aDiscretizationSize)
{
}

MeshProxy BrickShapeGeometry::generateMesh(const BrickDesign& aDesignParameters) const
{
    std::shared_ptr<stk::mesh::BulkData> tMesh = detail::create_mesh(aDesignParameters, mDiscretizationSize);
    write_mesh(mFileName, tMesh);
    return MeshProxy{mFileName, {}};
}

JacobianColumnEvaluator BrickShapeGeometry::jacobian(const BrickDesign& aDesignParameters) const
{
    return JacobianColumnEvaluator{
        /*.mColumns=*/kNumDesignParameters,
        /*.mX=*/detail::to_rol_std_vector(aDesignParameters),
        /*.mColumnFunction=*/[](unsigned int i, const ROL::StdVector<double>&) {
            return ROL::StdVector<double>(ROL::makePtr<std::vector<double>>(detail::sensitivities(i)));
        }};
}

std::unique_ptr<ROL::StdVector<double>> BrickShapeGeometry::initialGuess()
{
    return detail::to_rol_std_vector_ptr(BrickDesign{});
}

std::pair<std::vector<double>, std::vector<double>> BrickShapeGeometry::bounds()
{
    return {kLowerBounds, kUpperBounds};
}

void BrickShapeGeometry::output(const ROL::StdVector<double>& aSolution)
{
    std::cout << "centers: " << aSolution[0] << " " << aSolution[1] << " " << aSolution[2] << std::endl;
    std::cout << "dimensions: " << aSolution[3] << " " << aSolution[4] << " " << aSolution[5] << std::endl;
}

auto make_brick_shape_geometry(const BrickShapeGeometry& aBrickShapeGeometry)
    -> Function<MeshProxy, JacobianMultiplier, const ROL::StdVector<double>&>
{
    return make_function(
        [tBrickShapeGeometry = aBrickShapeGeometry](const ROL::StdVector<double>& x)
        { return tBrickShapeGeometry.generateMesh(detail::to_design_parameters(x)); },
        [tBrickShapeGeometry = aBrickShapeGeometry](const ROL::StdVector<double>& x)
        { return to_jacobian_multiplier(tBrickShapeGeometry.jacobian(detail::to_design_parameters(x))); });
}

namespace detail
{
std::shared_ptr<stk::mesh::BulkData> create_mesh(const BrickDesign& aDesign,
                                                 const std::optional<double> aDiscretizationSize)
{
    std::stringstream generationCommand;
    generationCommand << std::setprecision(16);
    generationCommand << "generated:";
    if (aDiscretizationSize)
    {
        const auto tNx = static_cast<int>(std::ceil(aDesign.dimension_x / aDiscretizationSize.value()));
        const auto tNy = static_cast<int>(std::ceil(aDesign.dimension_y / aDiscretizationSize.value()));
        const auto tNz = static_cast<int>(std::ceil(aDesign.dimension_z / aDiscretizationSize.value()));
        generationCommand << tNx << "x" << tNy << "x" << tNz;
    }
    else
    {
        generationCommand << "1x1x1";
    }
    generationCommand << "|bbox:";
    const double xmin = aDesign.center_x - aDesign.dimension_x / 2.0;
    const double ymin = aDesign.center_y - aDesign.dimension_y / 2.0;
    const double zmin = aDesign.center_z - aDesign.dimension_z / 2.0;
    generationCommand << xmin << "," << ymin << "," << zmin << ",";
    const double xmax = aDesign.center_x + aDesign.dimension_x / 2.0;
    const double ymax = aDesign.center_y + aDesign.dimension_y / 2.0;
    const double zmax = aDesign.center_z + aDesign.dimension_z / 2.0;
    generationCommand << xmax << "," << ymax << "," << zmax;
    generationCommand << "|sideset:Z|nodeset:Y";
    std::cout << generationCommand.str() << std::endl;
    return Plato::Functional::create_mesh(generationCommand.str());
}

std::vector<double> sensitivities(const unsigned int aParameterIndex)
{
    // design parameters are center (x,y,z), dimension (x,y,z)
    // All nodes or just corners? ESP is based on surface nodeset not all interior nodes, settle for corners right now
    // Assuming nodes are min(x),min(y),min(z) -> increasing x, increasing y, increasing z
    std::vector<double> base = {1, 1, 1, 1, 1, 1, 1, 1};
    const std::vector<double> xdim = {-0.5, 0.5, -0.5, 0.5, -0.5, 0.5, -0.5, 0.5};
    const std::vector<double> ydim = {-0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5};
    const std::vector<double> zdim = {-0.5, -0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0.5};

    switch (aParameterIndex)
    {
        case 3:
            base = xdim;
            break;
        case 4:
            base = ydim;
            break;
        case 5:
            base = zdim;
            break;
        default:
            break;
    }

    std::vector<double> sensitive(base.size() * kNumDims, 0);
    const unsigned int shift = aParameterIndex % 3;
    for (unsigned int b = 0; b < base.size(); ++b)
    {
        sensitive[kNumDims * b + shift] = base[b];
    }
    return sensitive;
}

ROL::StdVector<double> to_rol_std_vector(const BrickDesign& aDesignParameters)
{
    return ROL::StdVector<double>{aDesignParameters.center_x,    aDesignParameters.center_y,
                                  aDesignParameters.center_z,    aDesignParameters.dimension_x,
                                  aDesignParameters.dimension_y, aDesignParameters.dimension_z};
}

std::unique_ptr<ROL::StdVector<double>> to_rol_std_vector_ptr(const BrickDesign& aDesignParameters)
{
    ROL::StdVector<double> tROLVector = to_rol_std_vector(aDesignParameters);
    return std::make_unique<ROL::StdVector<double>>(tROLVector.getVector());
}

BrickDesign to_design_parameters(const ROL::StdVector<double>& aDesignParameter)
{
    assert(aDesignParameter.dimension() == kNumDesignParameters);
    return BrickDesign{/*.center_x=*/aDesignParameter[0],
                       /*.center_y=*/aDesignParameter[1],
                       /*.center_z=*/aDesignParameter[2],
                       /*.dimension_x=*/aDesignParameter[3],
                       /*.dimension_y=*/aDesignParameter[4],
                       /*.dimension_z=*/aDesignParameter[5]};
}
}  // namespace detail

}  // namespace Plato::Functional
