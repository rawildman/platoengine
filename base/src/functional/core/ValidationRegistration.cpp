#include "ValidationRegistration.hpp"

namespace Plato::Functional::Validation
{

void print_messages(const std::vector<std::string>& aMessages)
{
    for (auto tMessage : aMessages)
    {
        std::cout << tMessage << std::endl;
    }
}

}  // namespace Plato::Functional::Validation
