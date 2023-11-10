#include <type_traits>

#include "FilterFactory.hpp"
#include "Exception.hpp"
#include "FilterRegistration.hpp"
#include "Plato_InputBlocks.hpp"

namespace Plato::Functional::FilterFactory
{
FilterFunction make_filter_function(const Plato::density_topology& aInput)
{
    std::cout << "Num filters registered: " << detail::registered_functions().size() << std::endl;
    if (const auto tIter =
            detail::registered_functions().find(Plato::kFilterTypesTable.toString(aInput.filter_type.value()).value());
        tIter != detail::registered_functions().end())
    {
        return tIter->second(aInput);
    }
    else
    {
        throw Plato::Functional::Exception{"Unknown filter_type"};
    }
}
}  // namespace Plato::Functional::FilterFactory
