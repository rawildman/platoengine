#include "plato/utilities/SharedLibraryUtilities.hpp"

namespace plato::utilities
{
namespace
{
constexpr auto kUsingSanitizer = static_cast<bool>(BUILD_WITH_SANITIZER_FLAGS);
}
void* load_shared_library(const std::filesystem::path& aSharedLibPath)
{
    // RTLD_DEEPBIND is not compatible with building with sanitizer flags.
    // RTLD_DEEPBIND is required for external shared libraries to be opened correctly.
    // This is intended to enable sanitizer checks in the unit test suite during CI.
    constexpr auto tOptions = kUsingSanitizer ? RTLD_LAZY : (RTLD_LAZY | RTLD_DEEPBIND);
    void* const tSharedLibInterface = dlopen(aSharedLibPath.c_str(), tOptions);
    if (tSharedLibInterface == nullptr)
    {
        char* const tErrorMessage = dlerror();
        throw Exception{"Couldn't load shared lib at " + aSharedLibPath.string() +
                        ". Error: " + std::string{tErrorMessage}};
    }
    return tSharedLibInterface;
}
}  // namespace plato::utilities
