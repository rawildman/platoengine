#include "SharedLibraryUtilities.hpp"

namespace Plato::Functional::Utilities
{
void* load_shared_library(const std::filesystem::path& aSharedLibPath)
{
    void* tSharedLibInterface = dlopen(aSharedLibPath.c_str(), RTLD_LAZY);
    if (tSharedLibInterface == nullptr)
    {
        throw Exception{"Couldn't load shared lib at " + aSharedLibPath.string()};
    }
    return tSharedLibInterface;
}
}  // namespace Plato::Functional::Utilities