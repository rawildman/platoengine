#ifndef PLATO_FUNCTIONAL_AFFIRMUTILITIES
#define PLATO_FUNCTIONAL_AFFIRMUTILITIES

#include <string>

#include "Exception.hpp"
#include "Plato_InputBlocks.hpp"

namespace Plato::Functional::Affirmations
{
/// @throw Exception if @a aParameter does not contain a value.
template <typename T>
void affirm_parameter_exists(const boost::optional<T>& aParameter, const std::string_view aEntryName);

/// @brief Checks if the objective or constraint given by @a aParameter should be included in the optimization problem.
/// @tparam Must have a public field `active` that is a `boost` or `std::optional`.
template <typename Parameter>
bool is_active(const Parameter& aParameter);

template <typename T>
void affirm_parameter_exists(const boost::optional<T>& aParameter, const std::string_view aEntryName)
{
    if (!aParameter)
    {
        throw Plato::Functional::Exception("Objective block missing required entry \"" + std::string{aEntryName} +
                                           "\"");
    }
}

template <typename Parameter>
bool is_active(const Parameter& aParameter)
{
    return !aParameter.active.has_value() || aParameter.active.value();
}
}  // namespace Plato::Functional::Affirmations

#endif