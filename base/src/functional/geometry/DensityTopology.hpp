#ifndef PLATO_FUNCTIONAL_DENSITYTOPOLOGY
#define PLATO_FUNCTIONAL_DENSITYTOPOLOGY

#include <ROL_StdVector.hpp>
#include <filesystem>

#include "FilterFactory.hpp"
#include "Function.hpp"
#include "JacobianMultiplier.hpp"
#include "MeshProxy.hpp"

namespace Plato
{
struct density_topology;
}

namespace Plato::Functional
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
    explicit DensityTopology(const density_topology& aInput);

    [[nodiscard]] MeshProxy generateMesh(const ROL::StdVector<double>& aDesignParameter) const;

    [[nodiscard]] JacobianMultiplier jacobian(const ROL::StdVector<double>& aDesignParameter) const;

    [[nodiscard]] static std::unique_ptr<ROL::StdVector<double>> initialGuess(
        const std::filesystem::path& aMeshFileName);

    [[nodiscard]] static std::pair<std::vector<double>, std::vector<double>> bounds(
        const std::filesystem::path& aMeshFileName);

    static void output(const std::filesystem::path& aInputMeshName,
                       const ROL::StdVector<double>& aSolution,
                       const std::filesystem::path& aOutputMeshName);

   private:
    std::filesystem::path mFileName;
    unsigned int mNumDesignParameters = 0;
    FilterFactory::FilterFunction mFilter;
};

/// @brief Generate a geometry function, that can be composed with an objective function.
[[nodiscard]] auto make_topology_geometry(const DensityTopology& aDensityTopology)
    -> Function<MeshProxy, JacobianMultiplier, const ROL::StdVector<double>&>;

namespace detail
{
[[nodiscard]] std::optional<std::string> validate_mesh_name(const Plato::density_topology& aInput);
}

}  // namespace Plato::Functional

#endif
