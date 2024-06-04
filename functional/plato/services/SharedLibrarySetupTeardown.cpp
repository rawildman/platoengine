#include "plato/services/SharedLibrarySetupTeardown.hpp"

#include "AppConfigurationUtilities.hpp"
#include "plato/services/AppConfiguration.hpp"

namespace plato::services
{
namespace
{
constexpr auto kUsingSanitizer = static_cast<bool>(BUILD_WITH_SANITIZER_FLAGS);

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
        throw utilities::Exception{"Couldn't load shared lib at " + aSharedLibPath.string() +
                                   ". Error: " + std::string{tErrorMessage}};
    }
    return tSharedLibInterface;
}
}  // namespace

SharedLibrarySetupTeardown::SharedLibrarySetupTeardown(std::filesystem::path aSharedLibraryPath)
    : mSharedLibraryPath{std::move(aSharedLibraryPath)}, mSharedLibrary{load_shared_library(mSharedLibraryPath)}
{
}

SharedLibrarySetupTeardown::~SharedLibrarySetupTeardown()
{
    // todo: call dlclose?
    // If any function pointers pointing to functions within this lib are used after dlclose is called, that will result
    // in a crash. We need a way to tie the lifetime of the function pointers to the lifetime of this object. Maybe
    // wrapping the function pointers in weak_ptr?
}

}  // namespace plato::services
