#include "FilterRegistration.hpp"

#include <optional>

#include "FilterInterface.hpp"
#include "FilterJacobian.hpp"
#include "MeshProxy.hpp"
#include "Plato_InputBlocks.hpp"
#include "SharedLibraryUtilities.hpp"

namespace Plato::Functional::FilterFactory
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
    return make_function([tFilterAsShared](const MeshProxy& aMeshProxy) { return tFilterAsShared->filter(aMeshProxy); },
                         [tFilterAsShared](const MeshProxy& aMeshProxy) {
                             return FilterJacobian{tFilterAsShared, aMeshProxy};
                         });
}

std::unique_ptr<FilterInterface> load_filter(const Plato::density_topology& aInput,
                                             const std::filesystem::path& aSharedLibraryPath)
{
    using CreateFilterFunction = std::add_pointer_t<std::unique_ptr<FilterInterface>(const FilterParameters&)>;
    void* const tSharedLibInterface = Utilities::load_shared_library(aSharedLibraryPath);
    const auto tCreateFilterFunction = Utilities::load_function<CreateFilterFunction>(
        tSharedLibInterface, kCreateFilterFunctionName, aSharedLibraryPath);

    return tCreateFilterFunction(to_filter_parameters(aInput));
}

namespace detail
{
auto registered_functions() -> std::unordered_map<std::string, FactoryFunction>&
{
    static auto tFunctions = std::unordered_map<std::string, FactoryFunction>{};
    return tFunctions;
}
}  // namespace detail
}  // namespace Plato::Functional::FilterFactory
