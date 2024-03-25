#ifndef PLATO_UTILITIES_STRINGUTILTIES
#define PLATO_UTILITIES_STRINGUTILTIES

#include <string>
#include <string_view>
#include <vector>

namespace plato::utilities
{
/// @brief Concatenates all messages in @a aStrings with a delimeter @a aDelimiter in between each string.
std::string concatenate_vector(const std::vector<std::string>& aStrings, std::string_view aDelimiter = " ");
}
#endif