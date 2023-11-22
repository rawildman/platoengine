#ifndef PLATO_FUNCTIONAL_GEOMETRYREGISTRATION
#define PLATO_FUNCTIONAL_GEOMETRYREGISTRATION

#include <ROL_StdVector.hpp>
#include <memory>
#include <string_view>

#include "FactoryRegistration.hpp"
#include "Function.hpp"
#include "detail/GeometryInputBuilder.hpp"

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
    using Compute = Function<MeshProxy, JacobianMultiplier, const ROL::StdVector<double>&>;
    using InitialGuess = std::unique_ptr<ROL::StdVector<double>>;
    using Bounds = std::pair<std::vector<double>, std::vector<double>>;
    using Output = std::function<void(const ROL::StdVector<double>&)>;

    Compute mCompute;
    InitialGuess mInitialGuess;
    Bounds mBounds;
    Output mOutput;
};

/// A `std::variant` with alternatives corresponding to input blocks
/// created using the PLATO_GEOMETRY_INPUT_BLOCK_STRUCT macro.
using GeometryInput = Detail::GeometryInputVariant<Plato::PlatoInput>;
using GeometryRegistration = Registration<FactoryTypes, GeometryInput>;

bool is_geometry_function_registered(const std::string_view aFunctionName);

}  // namespace Plato::Functional::GeometryFactory

#endif
