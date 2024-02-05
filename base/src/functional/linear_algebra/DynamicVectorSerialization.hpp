#ifndef PLATO_FUNCTIONAL_CORE_DYNAMICVECTORSERIALIZATION
#define PLATO_FUNCTIONAL_CORE_DYNAMICVECTORSERIALIZATION

#include <boost/serialization/vector.hpp>

#include "DynamicVector.hpp"

namespace plato::functional::linear_algebra
{
template <typename Archive, typename U>
void serialize(Archive& aArchive, DynamicVector<U>& aVector, const unsigned int /*aVersion*/)
{
    aArchive& aVector.mVector;
}

}  // namespace plato::functional::linear_algebra

#endif
