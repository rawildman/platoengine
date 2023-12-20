#include <gtest/gtest.h>

#include "Exception.hpp"
#include "GeometryFactory.hpp"
#include "InputGeneration.hpp"
#include "InputParser.hpp"
#include "STKUtilities.hpp"

/* Have to figure out what to do with this test.
Is it even possible that an unknown geometry could get to the factory?
Should the factory no longer have that throw case?
TEST(GeometryFactory, UnknownGeometry)
{
    namespace pf = Plato::Functional;

    const auto tInput = Plato::PlatoInput{};
    EXPECT_THROW(auto tData = pf::GeometryFactory::make_geometry_data(tInput), pf::Exception);
}*/
