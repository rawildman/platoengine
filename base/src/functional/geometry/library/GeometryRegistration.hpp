#ifndef PLATO_FUNCTIONAL_GEOMETRYREGISTRATION
#define PLATO_FUNCTIONAL_GEOMETRYREGISTRATION

#include <memory>
#include <string_view>

#include "DynamicVector.hpp"
#include "FactoryRegistration.hpp"
#include "Function.hpp"
#include "GeometryInputBuilder.hpp"

namespace plato::functional::core
{
struct MeshProxy;
}

namespace plato::functional::linear_algebra
{
struct JacobianMultiplier;
}

namespace plato::functional::geometry::library
{
struct FactoryTypes
{
    using Compute = core::
        Function<core::MeshProxy, linear_algebra::JacobianMultiplier, const linear_algebra::DynamicVector<double>&>;
    using InitialGuess = linear_algebra::DynamicVector<double>;
    using Bounds = std::pair<std::vector<double>, std::vector<double>>;
    using Output = std::function<void(const linear_algebra::DynamicVector<double>&)>;

    Compute mCompute;
    InitialGuess mInitialGuess;
    Bounds mBounds;
    Output mOutput;
};

/// A `std::variant` with alternatives corresponding to input blocks
/// created using the PLATO_GEOMETRY_INPUT_BLOCK_STRUCT macro.
using GeometryInput = detail::GeometryInputVariant<Plato::PlatoInput>;
using ValidatedGeometryInput =
    core::ValidatedInputTypeWrapper<detail::ValidatedGeometryInputVariant<Plato::PlatoInput>>;
using GeometryRegistration = core::FactoryRegistration<FactoryTypes, ValidatedGeometryInput>;

/// @return A GeometryInput variant, which is the first non-empty geometry input block found in @a aInput.
/// @throw Exception If no geometry block was defined in @a aInput.
[[nodiscard]] library::GeometryInput first_geometry_input(const Plato::PlatoInput& aInput);

bool is_geometry_function_registered(const std::string_view aFunctionName);

/// @brief Helper to get the raw input from a validated geometry variant.
template <typename Geometry>
[[nodiscard]] const Geometry& geometry_raw_input(const ValidatedGeometryInput& aValidatedInput)
{
    return std::get<core::ValidatedInputTypeWrapper<Geometry>>(aValidatedInput.rawInput()).rawInput();
}
}  // namespace plato::functional::geometry::library

#endif
