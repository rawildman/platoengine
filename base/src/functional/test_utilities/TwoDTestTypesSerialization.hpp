#ifndef PLATO_FUNCTIONAL_TEST_TWODTYPESSERIALIZATION
#define PLATO_FUNCTIONAL_TEST_TWODTYPESSERIALIZATION

#include <boost/serialization/array.hpp>

#include "TwoDTestTypes.hpp"

namespace plato::functional::test_utilities
{
template <typename Archive>
void serialize(Archive& aArchive, TwoDVector& aVector, const unsigned int /*version*/)
{
    aArchive& aVector.mData;
}
}  // namespace plato::functional::test_utilities

#endif
