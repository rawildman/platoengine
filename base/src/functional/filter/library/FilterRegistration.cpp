#include "FilterRegistration.hpp"

#include <optional>

#include "FilterInterface.hpp"
#include "FilterJacobian.hpp"
#include "InputBlocks.hpp"
#include "MeshProxy.hpp"
#include "SharedLibraryUtilities.hpp"

namespace plato::functional::filter::library
{
namespace
{
template <typename T>
std::optional<T> to_std_optional(const boost::optional<T>& aT)
{
    if (aT)
    {
        return aT.value();
    }
    else
    {
        return std::nullopt;
    }
}

FilterParameters to_filter_parameters(const Plato::density_topology& aInput)
{
    return FilterParameters{
        /*.mFilterRadius=*/aInput.filter_radius.value_or(1.0),  // FIX-ME When adding validation, should be required
        /*.mBoundaryStickingPenalty=*/to_std_optional(aInput.boundary_sticking_penalty)};
}
}  // namespace

auto make_filter_function_from_interface(std::unique_ptr<FilterInterface> aFilter) -> FilterFunction
{
    auto tFilterAsShared = std::shared_ptr<FilterInterface>(std::move(aFilter));
    return core::make_function([tFilterAsShared](const core::MeshProxy& aMeshProxy)
                               { return tFilterAsShared->filter(aMeshProxy); },
                               [tFilterAsShared](const core::MeshProxy& aMeshProxy) {
                                   return FilterJacobian{tFilterAsShared, aMeshProxy};
                               });
}

std::unique_ptr<FilterInterface> load_filter(const Plato::density_topology& aInput,
                                             const std::filesystem::path& aSharedLibraryPath)
{
    using CreateFilterFunction = std::add_pointer_t<std::unique_ptr<FilterInterface>(const FilterParameters&)>;
    void* const tSharedLibInterface = plato::functional::utilities::load_shared_library(aSharedLibraryPath);
    const auto tCreateFilterFunction = plato::functional::utilities::load_function<CreateFilterFunction>(
        tSharedLibInterface, kCreateFilterFunctionName, aSharedLibraryPath);

    return tCreateFilterFunction(to_filter_parameters(aInput));
}

bool is_filter_function_registered(const std::string_view aFunctionName)
{
    return core::is_factory_function_registered<FilterFunction, FilterInput>(aFunctionName);
}

}  // namespace plato::functional::filter::library
