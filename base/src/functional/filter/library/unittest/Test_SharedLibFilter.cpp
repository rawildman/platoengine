#include <gtest/gtest.h>

#include "FilterFactory.hpp"
#include "FilterInterface.hpp"
#include "FilterRegistration.hpp"
#include "MeshProxy.hpp"
#include "InputBlocks.hpp"

namespace
{
const std::filesystem::path kSharedLibPath = "libPlatoIdentityFilter.so";
constexpr std::string_view kMeshName = "the-mesh-is-a-lie.exo";
const auto kRho = std::vector{-1.0, 0.0, 1.0};
const auto kMeshArgument = Plato::Functional::MeshProxy{kMeshName, kRho};
}  // namespace

namespace plato::functional::filter::library
{
TEST(SharedLibFilter, LoadAndValue)
{
    const std::unique_ptr<const FilterInterface> tFilter =
        load_filter(Plato::density_topology{}, kSharedLibPath);
    EXPECT_EQ(tFilter->filter(kMeshArgument).mNodalDensities, kRho);
}
}
