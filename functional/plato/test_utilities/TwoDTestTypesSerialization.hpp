#ifndef PLATO_FUNCTIONAL_TEST_UTILITIES_TWODTYPESSERIALIZATION
#define PLATO_FUNCTIONAL_TEST_UTILITIES_TWODTYPESSERIALIZATION

#include <boost/serialization/array.hpp>

#include "test_utilities/TwoDTestTypes.hpp"

namespace plato::functional::test_utilities
{
template <typename Archive>
void serialize(Archive& aArchive, TwoDVector& aVector, const unsigned int /*version*/)
{
    aArchive& aVector.mData;
}
}  // namespace plato::functional::test_utilities

#endif
