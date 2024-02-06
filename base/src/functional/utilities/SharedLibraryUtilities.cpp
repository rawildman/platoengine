#include "utilities/SharedLibraryUtilities.hpp"

namespace plato::functional::utilities
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
}  // namespace plato::functional::utilities