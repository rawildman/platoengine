#include "AffirmUtilities.hpp"

namespace Plato::Functional::Affirmations
{
void print_messages(const std::vector<std::string>& aMessages)
{
    for (auto tMessage : aMessages)
    {
        std::cout << tMessage << std::endl;
    }
}
}  // namespace Plato::Functional::Affirmations