#include "FilterFactory.hpp"

#include <type_traits>

#include "Exception.hpp"
#include "FilterRegistration.hpp"
#include "InputBlocks.hpp"

namespace plato::functional::filter::library
{
FilterFunction make_filter_function(const Plato::density_topology& aInput)
{
    const std::string tFilterName = Plato::kFilterTypesTable.toString(aInput.filter_type.value()).value();
    const std::optional<FilterFunction> tFilter =
        core::create_object_from_factory<FilterFunction, FilterInput>(tFilterName, aInput);

    if (tFilter)
    {
        return std::move(tFilter).value();
    }
    else
    {
        throw plato::functional::utilities::Exception{"Unknown filter_type"};
    }
}
}  // namespace plato::functional::filter::library
