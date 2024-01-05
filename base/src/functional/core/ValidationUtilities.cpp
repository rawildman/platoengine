#include "ValidationUtilities.hpp"

#include <numeric>

namespace Plato::Functional::Validation
{
std::string all_messages(const std::vector<std::string>& aMessages)
{
    return std::accumulate(aMessages.cbegin(), aMessages.cend(), std::string{},
                           [](std::string aCurrentMessages, std::string aMessage)
                           { return std::move(aCurrentMessages) + std::move(aMessage) + "\n"; });
}
}  // namespace Plato::Functional::Validation
