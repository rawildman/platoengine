#ifndef PLATO_FUNCTIONAL_INPUTPARSER
#define PLATO_FUNCTIONAL_INPUTPARSER

#include <filesystem>
#include <string_view>

#include "Plato_InputBlocks.hpp"

namespace Plato::Functional
{
/// @brief Parses all content of @a aInput as if it were an input deck.
[[nodiscard]] Plato::PlatoInput parse_input(std::string_view aInput);

/// @brief Parses all content of the file @a aFileName.
[[nodiscard]] Plato::PlatoInput parse_input_from_file(const std::filesystem::path& aFileName);
}  // namespace Plato::Functional

#endif
