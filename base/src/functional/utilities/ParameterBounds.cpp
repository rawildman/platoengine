#include "ParameterBounds.hpp"

namespace Plato::Functional::Core
{
auto unit_bounded() -> ParameterBounds<double>
{
    return ParameterBounds{Inclusive{0.0}, Inclusive{1.0}};
}

}