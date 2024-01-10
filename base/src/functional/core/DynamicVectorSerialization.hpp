#ifndef PLATO_FUNCTIONAL_CORE_DYNAMICVECTORSERIALIZATION
#define PLATO_FUNCTIONAL_CORE_DYNAMICVECTORSERIALIZATION

#include <boost/serialization/valarray.hpp>

#include "DynamicVector.hpp"

namespace Plato::Functional::Core
{
template <typename Archive, typename U>
void serialize(Archive& aArchive, DynamicVector<U>& aVector, const unsigned int /*aVersion*/)
{
    aArchive& aVector.mVector;
}

}  // namespace Plato::Functional::Core

#endif
