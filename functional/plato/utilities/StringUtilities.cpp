#include "plato/utilities/StringUtilities.hpp"

#include <numeric>

namespace plato::utilities
{
std::string concatenate_vector(const std::vector<std::string>& aStrings, const std::string_view aDelimiter)
{
    if (aStrings.empty())
    {
        return {};
    }
    return std::accumulate(std::next(aStrings.cbegin()), aStrings.cend(), aStrings.front(),
                           [aDelimiter](std::string aAllNames, std::string aCurrentName)
                           { return std::move(aAllNames) + std::string{aDelimiter} + std::move(aCurrentName); });
}
}  // namespace plato::utilities
