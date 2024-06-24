#ifndef PLATO_UTILITIES_COORDINATETESTUTILITIES
#define PLATO_UTILITIES_COORDINATETESTUTILITIES

#include "plato/test_utilities/TestContext.hpp"
#include "plato/utilities/Vector3.hpp"

namespace plato::utilities
{

template <typename Container3>
void test_double_equality_of_components(const Container3& aResult,
                                        const Container3& aGold,
                                        const test_utilities::TestContext& aTestContext)
{
    EXPECT_DOUBLE_EQ(aResult.x, aGold.x) << aTestContext;
    EXPECT_DOUBLE_EQ(aResult.y, aGold.y) << aTestContext;
    EXPECT_DOUBLE_EQ(aResult.z, aGold.z) << aTestContext;
}

}  // namespace plato::utilities

#endif
