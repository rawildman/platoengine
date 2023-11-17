#ifndef PLATO_FUNCTIONAL_OPTIMIZER_FACTORY
#define PLATO_FUNCTIONAL_OPTIMIZER_FACTORY

#include <ROL_Algorithm.hpp>

#include "Plato_InputBlocks.hpp"

namespace Plato::Functional
{

///@brief Generate a ROL::ParameterList. Either from input deck or file.
/// First attempt is to load from a file, but if the file input is not specified the defaults
/// or values specified in the input block will be used
///@param aOptimizationParameters
///@return ROL::ParameterList
[[nodiscard]] ROL::ParameterList rol_parameter_list(const Plato::optimization_parameters& aOptimizationParameters);

}  // namespace Plato::Functional

#endif
