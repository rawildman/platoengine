#ifndef PLATO_SERVICES_APPSHAREDLIBRARY
#define PLATO_SERVICES_APPSHAREDLIBRARY

#include "plato/services/SharedLibrarySetupTeardown.hpp"

namespace plato::services
{
/// @brief The purpose of this class is to provide an interface between AppConfiguration and SharedLibrarySetupTeardown.
class [[nodiscard]] AppSharedLibrary
{
   public:
    explicit AppSharedLibrary(AppConfigurationWithDirectory aAppConfiguration);

    /// @brief Calls function with name @a aFunction name and arguments @a aArgs from the shared library loaded on
    /// construction.
    /// @param aFunctionName The name must be one of the criteria names given in the AppConfiguration set on
    /// construction.
    /// @throw plato::utilities::Exception On error loading the shared library function.
    template <typename R, typename... Args>
    R call(std::string_view aFunctionName, Args&&... aArgs);

    AppSharedLibrary(const AppSharedLibrary&) = delete;
    AppSharedLibrary& operator=(const AppSharedLibrary&) = delete;
    AppSharedLibrary(AppSharedLibrary&&) = default;
    AppSharedLibrary& operator=(AppSharedLibrary&&) = default;

   private:
    AppConfigurationWithDirectory mAppConfiguration;
    SharedLibrarySetupTeardown mSharedLibrary;
};

template <typename R, typename... Args>
R AppSharedLibrary::call(const std::string_view /*aFunctionName*/, Args&&... /*aArgs*/)
{
    return R{};
}

}  // namespace plato::services

#endif
