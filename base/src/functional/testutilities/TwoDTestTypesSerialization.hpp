#ifndef PLATO_FUNCTIONAL_TEST_TWODTYPESSERIALIZATION
#define PLATO_FUNCTIONAL_TEST_TWODTYPESSERIALIZATION

#include <boost/serialization/array.hpp>

#include "TwoDTestTypes.hpp"

namespace Plato::Functional::Test
{
template <typename Archive>
void serialize(Archive& aArchive, TwoDVector& aVector, const unsigned int /*version*/)
{
    aArchive & aVector.mData;
}
}

#endif
