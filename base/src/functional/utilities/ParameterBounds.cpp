#include "ParameterBounds.hpp"

namespace plato::functional::utilities
{
auto unit_bounded() -> ParameterBounds<double> { return ParameterBounds{Inclusive{0.0}, Inclusive{1.0}}; }

}  // namespace plato::functional::utilities