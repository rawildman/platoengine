#ifndef PLATO_FUNCTIONAL_GEOMETRYREGISTRATION
#define PLATO_FUNCTIONAL_GEOMETRYREGISTRATION

#include <memory>
#include <string_view>

#include "DynamicVector.hpp"
#include "FactoryRegistration.hpp"
#include "Function.hpp"
#include "GeometryInputBuilder.hpp"

namespace Plato
{
namespace Functional
{
struct JacobianMultiplier;
struct MeshProxy;
}  // namespace Functional
}  // namespace Plato

namespace Plato::Functional::GeometryFactory
{
struct FactoryTypes
{
    using Compute = Function<MeshProxy, JacobianMultiplier, const Core::DynamicVector<double>&>;
    using InitialGuess = Core::DynamicVector<double>;
    using Bounds = std::pair<std::vector<double>, std::vector<double>>;
    using Output = std::function<void(const Core::DynamicVector<double>&)>;

    Compute mCompute;
    InitialGuess mInitialGuess;
    Bounds mBounds;
    Output mOutput;
};

/// A `std::variant` with alternatives corresponding to input blocks
/// created using the PLATO_GEOMETRY_INPUT_BLOCK_STRUCT macro.
using GeometryInput = Detail::GeometryInputVariant<Plato::PlatoInput>;
using ValidatedGeometryInput =
    Core::ValidatedInputTypeWrapper<Detail::ValidatedGeometryInputVariant<Plato::PlatoInput>>;
using GeometryRegistration = Registration<FactoryTypes, ValidatedGeometryInput>;

bool is_geometry_function_registered(const std::string_view aFunctionName);

/// @brief Helper to get the raw input from a validated geometry variant.
template <typename Geometry>
[[nodiscard]] const Geometry& geometry_raw_input(const ValidatedGeometryInput& aValidatedInput)
{
    return std::get<Core::ValidatedInputTypeWrapper<Geometry>>(aValidatedInput.rawInput()).rawInput();
}
}  // namespace Plato::Functional::GeometryFactory

#endif
