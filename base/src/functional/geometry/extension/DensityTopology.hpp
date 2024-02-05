#ifndef PLATO_FUNCTIONAL_DENSITYTOPOLOGY
#define PLATO_FUNCTIONAL_DENSITYTOPOLOGY

#include <filesystem>
#include <optional>

#include "DynamicVector.hpp"
#include "FilterFactory.hpp"
#include "Function.hpp"
#include "InputBlocks.hpp"
#include "JacobianMultiplier.hpp"
#include "MeshProxy.hpp"
#include "ValidationRegistration.hpp"

namespace Plato
{
struct density_topology;
}

namespace plato::functional::geometry::extension
{
/// @brief Density-based topology representation of a geometry.
///
/// Implementation for density-based topology optimization. The design
/// variable used by this class is a nodal density field in `[0, 1]` representing
/// the presence or absence of material. This class may use a Filter to smooth
/// the density field and reduce mesh dependency in the solution.
class DensityTopology
{
   public:
    explicit DensityTopology(const Plato::density_topology& aInput);

    [[nodiscard]] Plato::Functional::MeshProxy generateMesh(
        const linear_algebra::DynamicVector<double>& aDesignParameter) const;

    [[nodiscard]] linear_algebra::JacobianMultiplier jacobian(
        const linear_algebra::DynamicVector<double>& aDesignParameter) const;

    [[nodiscard]] static linear_algebra::DynamicVector<double> initialGuess(
        const std::filesystem::path& aMeshFileName);

    [[nodiscard]] static std::pair<std::vector<double>, std::vector<double>> bounds(
        const std::filesystem::path& aMeshFileName);

    static void output(const std::filesystem::path& aInputMeshName,
                       const linear_algebra::DynamicVector<double>& aSolution,
                       const std::filesystem::path& aOutputMeshName);

   private:
    std::filesystem::path mFileName;
    unsigned int mNumDesignParameters = 0;
    plato::functional::filter::library::FilterFunction mFilter;
};

/// @brief Generate a geometry function, that can be composed with an objective function.
[[nodiscard]] auto make_topology_geometry(const DensityTopology& aDensityTopology)
    -> Plato::Functional::Function<Plato::Functional::MeshProxy,
                                   linear_algebra::JacobianMultiplier,
                                   const linear_algebra::DynamicVector<double>&>;

namespace detail
{
[[nodiscard]] std::optional<std::string> validate_output_name(const Plato::density_topology& aInput);
}  // namespace detail

}  // namespace plato::functional::geometry::extension

#endif
