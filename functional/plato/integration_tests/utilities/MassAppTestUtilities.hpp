#ifndef PLATO_INTEGRATION_TESTS_UTILITIES
#define PLATO_INTEGRATION_TESTS_UTILITIES

#include <boost/mpi/communicator.hpp>
#include <string>
#include <string_view>
#include <utility>

#include "plato/linear_algebra/DynamicVector.hpp"
#include "plato/process_manager/library/ValidatedInput.hpp"
#include "plato/services/AppConfigurationUtilities.hpp"

namespace plato::integration_tests::utilities
{
/// @brief Registers a plugin for the MassApp test shared library and returns the registered name and the number of apps
/// successfully registered.
/// @note This assumes that the actual shared library is in the current working directory, which
///  will be the case for any unit tests run from the build directory.
[[nodiscard]] services::ConfigurationDirectorySetupTeardown register_test_mass_app(
    std::string_view aAppName, const boost::mpi::communicator& aComm = boost::mpi::communicator{});

/// @brief Creates test input that has a brick shape geometry, ROL optimization, and an objective
/// with name @a aMassAppName and number of processors @a aNumProcessors.
[[nodiscard]] process_manager::library::ValidatedInput create_valid_brick_input(std::string_view aMassAppName,
                                                                                const unsigned int aNumProcessors);

/// @brief Creates arbitrary test controls with the associated total volume for a BrickShapeGeometry.
[[nodiscard]] std::pair<linear_algebra::DynamicVector<double>, double> brick_shape_geometry_controls_with_volume();

}  // namespace plato::integration_tests::utilities

#endif
