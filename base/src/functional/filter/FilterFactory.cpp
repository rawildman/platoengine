#include "FilterFactory.hpp"

#include <type_traits>

#include "Exception.hpp"
#include "FilterRegistration.hpp"
#include "Plato_InputBlocks.hpp"

namespace Plato::Functional::FilterFactory
{
FilterFunction make_filter_function(const Plato::density_topology& aInput)
{
    if (const auto tIter = detail::registered_functions<FilterFunction, FilterInput>().find(
            Plato::kFilterTypesTable.toString(aInput.filter_type.value()).value());
        tIter != detail::registered_functions<FilterFunction, FilterInput>().end())
    {
        return tIter->second(aInput);
    }
    else
    {
        throw Plato::Functional::Exception{"Unknown filter_type"};
    }
}
}  // namespace Plato::Functional::FilterFactory
