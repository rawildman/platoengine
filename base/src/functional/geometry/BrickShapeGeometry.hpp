#ifndef PLATO_BRICK_SHAPE_GEOMETRY
#define PLATO_BRICK_SHAPE_GEOMETRY

#include <ROL_StdVector.hpp>
#include <filesystem>
#include <optional>

#include "Function.hpp"
#include "JacobianColumnEvaluator.hpp"
#include "JacobianMultiplier.hpp"
#include "MeshProxy.hpp"

namespace stk::mesh
{
class BulkData;
}

namespace Plato::Functional
{
/// @brief Design parameters for BrickShapeGeometry
struct BrickDesign
{
    double center_x = 0.0;
    double center_y = 0.0;
    double center_z = 0.0;
    double dimension_x = 1.0;
    double dimension_y = 1.0;
    double dimension_z = 1.0;
};

/// @brief Prototype example of a geometry function that maps a center location and dimensions to a brick.
///
/// The purpose of this class is to demonstrate a geometry function. It uses six design parameters,
/// center coordinates and dimensions, and maps to a hex mesh. The goal is to demonstrate a shape-optimization-like
/// capability similar to ESP for testing purposes.
class BrickShapeGeometry
{
   public:
    /// @param aFileName The name of the file to write the mesh generated in generateMesh
    /// @param aDiscretizationSize When non-empty, this gives the approximate discretization size of the resulting mesh.
    /// I.e., the
    ///  number of elements is `ceil(dimension / discretizationSize)`.
    explicit BrickShapeGeometry(std::filesystem::path aFileName,
                                std::optional<double> aDiscretizationSize = std::nullopt);

    [[nodiscard]] MeshProxy generateMesh(const BrickDesign& aDesignParameters) const;

    [[nodiscard]] JacobianColumnEvaluator jacobian(const BrickDesign& aDesignParameters) const;

    [[nodiscard]] static std::unique_ptr<ROL::StdVector<double>> initialGuess();

    [[nodiscard]] static std::pair<std::vector<double>, std::vector<double>> bounds();

    static void output(const ROL::StdVector<double>& aSolution);

   private:
    std::filesystem::path mFileName;
    std::optional<double> mDiscretizationSize;
};

/// @brief Generate a geometry function, that can be composed with an objective function.
[[nodiscard]] auto make_brick_shape_geometry(const BrickShapeGeometry& aBrickShapeGeometry)
    -> Function<MeshProxy, JacobianMultiplier, const ROL::StdVector<double>&>;

namespace detail
{
[[nodiscard]] BrickDesign to_design_parameters(const ROL::StdVector<double>& aDesignParameter);

[[nodiscard]] std::shared_ptr<stk::mesh::BulkData> create_mesh(
    const BrickDesign& aDesign, std::optional<double> aDiscretizationSize = std::nullopt);

[[nodiscard]] std::vector<double> sensitivities(unsigned int aParameterIndex);

[[nodiscard]] ROL::StdVector<double> to_rol_std_vector(const BrickDesign& aDesignParameters);

[[nodiscard]] std::unique_ptr<ROL::StdVector<double>> to_rol_std_vector_ptr(const BrickDesign& aDesignParameters);
}  // namespace detail
}  // namespace Plato::Functional
#endif
