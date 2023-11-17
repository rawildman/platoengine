#include "FilterFactory.hpp"

#include <dlfcn.h>

#include <type_traits>

#include "Exception.hpp"
#include "FilterInterface.hpp"
#include "IdentityFilter.hpp"
#include "Plato_InputBlocks.hpp"
#include "SharedLibraryUtilities.hpp"

namespace Plato::Functional::FilterFactory
{
namespace
{
const auto kHelmholtzFilterLibName = std::filesystem::path{"libAnalyzeFunctionalInterface.so"};

[[nodiscard]] auto make_filter_function(std::unique_ptr<FilterInterface> aFilter)
    -> Function<MeshProxy, FilterJacobian, const MeshProxy&>
{
    auto tFilterAsShared = std::shared_ptr<FilterInterface>(std::move(aFilter));
    return make_function([tFilterAsShared](const MeshProxy& aMeshProxy) { return tFilterAsShared->filter(aMeshProxy); },
                         [tFilterAsShared](const MeshProxy& aMeshProxy) {
                             return FilterJacobian{tFilterAsShared, aMeshProxy};
                         });
}

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

FilterFunction make_filter_function(const Plato::density_topology& aInput)
{
    if (aInput.filter_type == Plato::FilterTypes::kKernel)
    {
        throw Plato::Functional::Exception("Kernel filter is not yet implemented.");
    }
    else if (aInput.filter_type == Plato::FilterTypes::kHelmholtz)
    {
        return make_filter_function(detail::load_filter(to_filter_parameters(aInput), kHelmholtzFilterLibName));
    }
    else  // Default to identity filter
    {
        return make_identity_filter_function();
    }
}

namespace detail
{
std::unique_ptr<FilterInterface> load_filter(const FilterParameters& aInput,
                                             const std::filesystem::path& aSharedLibraryPath)
{
    using CreateFilterFunction = std::add_pointer_t<std::unique_ptr<FilterInterface>(const FilterParameters&)>;
    void* const tSharedLibInterface = Utilities::load_shared_library(aSharedLibraryPath);
    const auto tCreateFilterFunction = Utilities::load_function<CreateFilterFunction>(
        tSharedLibInterface, kCreateFilterFunctionName, aSharedLibraryPath);

    return tCreateFilterFunction(aInput);
}
}  // namespace detail
}  // namespace Plato::Functional::FilterFactory
