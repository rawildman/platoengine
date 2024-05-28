#ifndef PLATO_SERVICES_APPCONFIGURATION
#define PLATO_SERVICES_APPCONFIGURATION

#include <string>

namespace plato::services
{
/// @brief Configuration data for defining an app's interface with plato.
///
/// A file, written using the serialize function is expected to be associated with each
/// shared library containing an app.
struct AppConfiguration
{
    std::string mName;
    std::string mLibraryFileName;
    bool mHasParallelImplementation = false;
    bool mHasSerialImplementation = false;
};

/// @todo Use `operator==() = default` in c++20
[[nodiscard]] bool operator==(const AppConfiguration& aAppConfigurationLeft,
                              const AppConfiguration& aAppConfigurationRight);

template <class Archive>
void serialize(Archive& aArchive, AppConfiguration& aAppConfiguration, const unsigned int /*version*/)
{
    aArchive& aAppConfiguration.mName;
    aArchive& aAppConfiguration.mLibraryFileName;
    aArchive& aAppConfiguration.mHasParallelImplementation;
    aArchive& aAppConfiguration.mHasSerialImplementation;
}

}  // namespace plato::services

#endif
