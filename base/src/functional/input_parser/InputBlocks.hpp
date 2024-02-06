#ifndef PLATO_FUNCTIONAL_INPUT_PARSER_INPUTBLOCKS
#define PLATO_FUNCTIONAL_INPUT_PARSER_INPUTBLOCKS

#include <string>
#include <vector>

#include "input_parser/FileList.hpp"
#include "input_parser/InputBlockStruct.hpp"
#include "input_parser/InputEnumTypes.hpp"

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
/// must also be added to the main ParsedInput struct.
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
    (plato)(functional)(input_parser), optimization_parameters,
    (plato::functional::input_parser::FileName, input_file_name)
    (unsigned int, max_iterations)
    (double, step_tolerance)
    (double, gradient_tolerance)
)

PLATO_NAMED_INPUT_BLOCK_STRUCT(
    (plato)(functional)(input_parser), objective,
    (bool, active)
    (plato::functional::input_parser::CodeOptions, app) 
    (plato::functional::input_parser::FileName, shared_library_path)
    (unsigned int, number_of_processors)
    (plato::functional::input_parser::FileList, input_files)
    (double, aggregation_weight)
    (plato::functional::input_parser::ObjectiveTypes, objective_type)
)

PLATO_NAMED_INPUT_BLOCK_STRUCT(
    (plato)(functional)(input_parser), constraint,
    (bool, active)
    (plato::functional::input_parser::CodeOptions, app) 
    (plato::functional::input_parser::FileName, shared_library_path)
    (unsigned int, number_of_processors)
    (plato::functional::input_parser::FileList, input_files)
    (double, equal_to)
    (bool, is_linear)
)

PLATO_GEOMETRY_INPUT_BLOCK_STRUCT(
    (plato)(functional)(input_parser), brick_shape_geometry,
    (plato::functional::input_parser::FileName, mesh_name)
)

PLATO_GEOMETRY_INPUT_BLOCK_STRUCT(
    (plato)(functional)(input_parser), density_topology,
    (plato::functional::input_parser::FileName, mesh_name)
    (plato::functional::input_parser::FileName, output_name)
    (plato::functional::input_parser::FilterTypes, filter_type)
    (double, filter_radius)
    (double, boundary_sticking_penalty)
)

/// ParsedInput is the in-memory representation of a parsed input deck.
/// To add new blocks, use PLATO_INPUT_BLOCK_STRUCT or PLATO_NAMED_INPUT_BLOCK_STRUCT
/// macros. The `NAMED` version is for blocks that can have multiple instantiations
/// identified with a name. Those must be added here wrapped in `std::vector`.
BOOST_FUSION_DEFINE_STRUCT(
    (plato)(functional)(input_parser), ParsedInput,
    (std::vector<plato::functional::input_parser::objective>, mObjectives)
    (std::vector<plato::functional::input_parser::constraint>, mConstraints)
    (boost::optional<plato::functional::input_parser::brick_shape_geometry>, mBrickShapeGeometry)
    (boost::optional<plato::functional::input_parser::density_topology>, mDensityTopology)
    (plato::functional::input_parser::optimization_parameters, mOptimizationParameters)
)
// clang-format on
namespace plato::functional::input_parser
{
template <typename BlockStruct>
std::string block_name()
{
    return InputTypeName<BlockStruct>::name;
}
}  // namespace plato::functional::input_parser

#endif
