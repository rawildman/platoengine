#ifndef PLATO_FUNCTIONAL_DENSITYTOPOLOGY
#define PLATO_FUNCTIONAL_DENSITYTOPOLOGY

#include <ROL_StdVector.hpp>
#include <filesystem>
#include <optional>

#include "FilterFactory.hpp"
#include "Function.hpp"
#include "JacobianMultiplier.hpp"
#include "MeshProxy.hpp"
#include "Plato_InputBlocks.hpp"
#include "ValidationRegistration.hpp"

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

namespace Validation::DensityTopology::detail
{

template <Plato::FilterTypes X>
[[nodiscard]] constexpr typename std::enable_if<X == Plato::FilterTypes::kIdentity, std::optional<std::string>>::type
check_filter_values(const Plato::density_topology& aInput)
{
    if (aInput.boundary_sticking_penalty.has_value() || aInput.filter_radius.has_value())
    {
        return Plato::block_name<Plato::density_topology>() + R"( identity filter cannot have "filter_radius" or "boundary_sticking_penalty" defined.)";
    }
    else
    {
        return std::nullopt;
    }
}

template <Plato::FilterTypes X>
[[nodiscard]] constexpr typename std::enable_if<X == Plato::FilterTypes::kHelmholtz, std::optional<std::string>>::type
check_filter_values(const Plato::density_topology& aInput)
{
    const std::optional<std::string> tErrorMsgForStickingMethod =
        Plato::Functional::Validation::error_message_for_parameter_out_of_bounds(Plato::block_name<Plato::density_topology>(),
                                                                                 aInput.boundary_sticking_penalty,
                                                                                 "boundary_sticking_penalty",
                                                                                 0,
                                                                                 std::nullopt);

    const std::optional<std::string> tErrorMsgForFilterRadius =
        Plato::Functional::Validation::error_message_for_parameter_out_of_bounds(Plato::block_name<Plato::density_topology>(),
                                                                                 aInput.filter_radius,
                                                                                 "filter_radius",
                                                                                 1e-16,
                                                                                 std::nullopt);
    std::string tErrorMsg;
    if (tErrorMsgForFilterRadius)
    {
        tErrorMsg = tErrorMsgForFilterRadius.value();
    }
    if (tErrorMsgForStickingMethod)
    {
        tErrorMsg += tErrorMsgForStickingMethod.value();
    }

    return tErrorMsg.length() == 0 ? std::nullopt : std::optional<std::string>{tErrorMsg};
}

[[nodiscard]] std::optional<std::string> validate_output_name(const Plato::density_topology& aInput);
[[nodiscard]] std::optional<std::string> validate_filter(const Plato::density_topology& aInput);

}  // namespace Validation::DensityTopology::detail

}  // namespace Plato::Functional

#endif
