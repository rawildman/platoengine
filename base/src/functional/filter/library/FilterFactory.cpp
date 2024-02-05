#include "FilterFactory.hpp"

#include <type_traits>

#include "Exception.hpp"
#include "FilterRegistration.hpp"
#include "InputBlocks.hpp"

namespace plato::functional::filter::library
{
FilterFunction make_filter_function(const Plato::density_topology& aInput)
{
    if (const auto tIter = Plato::Functional::detail::registered_functions<FilterFunction, FilterInput>().find(
            Plato::kFilterTypesTable.toString(aInput.filter_type.value()).value());
        tIter != Plato::Functional::detail::registered_functions<FilterFunction, FilterInput>().end())
    {
        return tIter->second(aInput);
    }
    else
    {
        throw Plato::Functional::Exception{"Unknown filter_type"};
    }
}
}  // namespace plato::functional::filter::library
