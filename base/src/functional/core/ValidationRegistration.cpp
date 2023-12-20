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

namespace detail
{
bool is_within_bounds(const double aValue, const std::optional<double> aLowerBound, const std::optional<double> aUpperBound)
{
    return is_within_lower_bounds(aValue, aLowerBound) && is_within_upper_bounds(aValue, aUpperBound);
}

bool is_within_lower_bounds(const double aValue, const std::optional<double> aLowerBound)
{
    if (!aLowerBound)
    {
        return true;
    }
    else
    {
        return aValue >= aLowerBound.value();
    }
}
bool is_within_upper_bounds(const double aValue, const std::optional<double> aUpperBound)
{
    if (!aUpperBound)
    {
        return true;
    }
    else
    {
        return aValue <= aUpperBound.value();
    }
}
}  // namespace detail

}  // namespace Plato::Functional::Validation
