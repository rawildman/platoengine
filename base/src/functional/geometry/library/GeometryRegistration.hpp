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

namespace plato::functional::geometry::library
{
struct FactoryTypes
{
    using Compute = Plato::Functional::Function<Plato::Functional::MeshProxy,
                                                Plato::Functional::JacobianMultiplier,
                                                const Plato::Functional::Core::DynamicVector<double>&>;
    using InitialGuess = Plato::Functional::Core::DynamicVector<double>;
    using Bounds = std::pair<std::vector<double>, std::vector<double>>;
    using Output = std::function<void(const Plato::Functional::Core::DynamicVector<double>&)>;

    Compute mCompute;
    InitialGuess mInitialGuess;
    Bounds mBounds;
    Output mOutput;
};

/// A `std::variant` with alternatives corresponding to input blocks
/// created using the PLATO_GEOMETRY_INPUT_BLOCK_STRUCT macro.
using GeometryInput = detail::GeometryInputVariant<Plato::PlatoInput>;
using ValidatedGeometryInput =
    Plato::Functional::Core::ValidatedInputTypeWrapper<detail::ValidatedGeometryInputVariant<Plato::PlatoInput>>;
using GeometryRegistration = Plato::Functional::Registration<FactoryTypes, ValidatedGeometryInput>;

/// @return A GeometryInput variant, which is the first non-empty geometry input block found in @a aInput.
/// @throw Exception If no geometry block was defined in @a aInput.
[[nodiscard]] library::GeometryInput first_geometry_input(const Plato::PlatoInput& aInput);

bool is_geometry_function_registered(const std::string_view aFunctionName);

/// @brief Helper to get the raw input from a validated geometry variant.
template <typename Geometry>
[[nodiscard]] const Geometry& geometry_raw_input(const ValidatedGeometryInput& aValidatedInput)
{
    return std::get<Plato::Functional::Core::ValidatedInputTypeWrapper<Geometry>>(aValidatedInput.rawInput())
        .rawInput();
}
}  // namespace plato::functional::geometry::library

#endif
