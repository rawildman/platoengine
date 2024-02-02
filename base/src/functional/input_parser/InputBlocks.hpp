// clang-format off
#ifndef PLATO_INPUTBLOCKS_HPP
#define PLATO_INPUTBLOCKS_HPP

#include "InputBlockStruct.hpp"
#include "FileList.hpp"
#include "InputEnumTypes.hpp"

#include <string>
#include <vector>

/// @file Input block declarations.
///  Each PLATO_INPUT_BLOCK_STRUCT represents a parsable struct of key-value pairs.
///  For example, the `objective` block will be parsed as
/// @code
/// begin objective
///   active true
///   app sd_mass_app
///   ...
/// end objective
/// @endcode
/// and the parsed object has fields corresponding to the input keys.
///
/// Each declaration starts with the namespace the struct is declared in.
/// The second argument is the struct name, and the third argument is a
/// sequence of type/name pairs for each struct field. Any new input blocks
/// must also be added to the main PlatoInput struct.
///
/// Importantly, each field type is wrapped in boost::optional, which is used
/// to indicate if the field was actually present and parsed in the input deck.
///
/// @note `std::string` cannot be used as an argument directly, instead use
/// the helper types FileName and FileList.
/// @note Enumerations may be used, but must be declared with DECLARE_ENUM_SYMBOL_TABLE and
///  defined with DEFINE_ENUM_SYMBOL_TABLE.
// clang-format off
PLATO_INPUT_BLOCK_STRUCT(
    (Plato), optimization_parameters,
    (Plato::FileName, input_file_name)
    (unsigned int, max_iterations)
    (double, step_tolerance)
    (double, gradient_tolerance)
)

PLATO_NAMED_INPUT_BLOCK_STRUCT(
    (Plato), objective,
    (bool, active)
    (Plato::CodeOptions, app) 
    (Plato::FileName, shared_library_path)
    (unsigned int, number_of_processors)
    (Plato::FileList, input_files)
    (double, aggregation_weight)
    (Plato::ObjectiveTypes, objective_type)
)

PLATO_NAMED_INPUT_BLOCK_STRUCT(
    (Plato), constraint,
    (bool, active)
    (Plato::CodeOptions, app) 
    (Plato::FileName, shared_library_path)
    (unsigned int, number_of_processors)
    (Plato::FileList, input_files)
    (double, equal_to)
    (bool, is_linear)
)

PLATO_GEOMETRY_INPUT_BLOCK_STRUCT(
    (Plato), brick_shape_geometry,
    (Plato::FileName, mesh_name)
)

PLATO_GEOMETRY_INPUT_BLOCK_STRUCT(
    (Plato), density_topology,
    (Plato::FileName, mesh_name)
    (Plato::FileName, output_name)
    (Plato::FilterTypes, filter_type)
    (double, filter_radius)
    (double, boundary_sticking_penalty)
)

/// PlatoInput is the in-memory representation of a parsed input deck.
/// To add new blocks, use PLATO_INPUT_BLOCK_STRUCT or PLATO_NAMED_INPUT_BLOCK_STRUCT
/// macros. The `NAMED` version is for blocks that can have multiple instantiations
/// identified with a name. Those must be added here wrapped in `std::vector`.
BOOST_FUSION_DEFINE_STRUCT(
    (Plato), PlatoInput,
    (std::vector<Plato::objective>, mObjectives)
    (std::vector<Plato::constraint>, mConstraints)
    (boost::optional<Plato::brick_shape_geometry>, mBrickShapeGeometry)
    (boost::optional<Plato::density_topology>, mDensityTopology)
    (Plato::optimization_parameters, mOptimizationParameters)
)
// clang-format on
namespace Plato
{
template <typename BlockStruct>
std::string block_name()
{
    return Input::InputTypeName<BlockStruct>::name;
}
}  // namespace Plato

#endif
// clang-format on