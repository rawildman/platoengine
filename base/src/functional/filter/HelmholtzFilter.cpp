#include <filesystem>

#include "FilterInterface.hpp"
#include "FilterRegistration.hpp"
#include "Plato_InputEnumTypes.hpp"

namespace Plato::Functional
{
namespace
{
const auto kHelmholtzFilterLibName = std::filesystem::path{"libAnalyzeFunctionalInterface.so"};

[[maybe_unused]] static auto kHelmholtzFilterRegistration = Plato::Functional::FilterFactory::FilterRegistration{
    Plato::kFilterTypesTable.toString(Plato::FilterTypes::kHelmholtz).value(), [](const Plato::density_topology& aInput)
    {
        return FilterFactory::make_filter_function_from_interface(
            FilterFactory::load_filter(aInput, kHelmholtzFilterLibName));
    }};

}  // namespace
}  // namespace Plato::Functional
