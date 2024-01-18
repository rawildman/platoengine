#ifndef PLATO_FUNCTIONAL_NAMEDTYPE
#define PLATO_FUNCTIONAL_NAMEDTYPE

#include <utility>

namespace Plato::Functional::Core
{

template <typename T, typename NameTag>
struct NamedType
{
    constexpr explicit NamedType(const T& value) : mValue(value) {}
    constexpr explicit NamedType(T&& value) : mValue(std::move(value)) {}

    T mValue;
};

}  // namespace Plato::Functional::Core
#endif