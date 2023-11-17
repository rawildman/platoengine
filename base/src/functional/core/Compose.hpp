#ifndef PLATO_FUNCTIONAL_COMPOSE
#define PLATO_FUNCTIONAL_COMPOSE

#include <type_traits>
#include <utility>

#include "Function.hpp"

namespace Plato::Functional
{
/// @brief Generates a new Function that is the composition of @a f and @a g, i.e. \f$f(g(x))\f$
template <typename fR, typename dFR, typename fArg, typename gR, typename dGR, typename gArg>
[[nodiscard]] auto compose(Plato::Functional::Function<fR, dFR, fArg> aF, Plato::Functional::Function<gR, dGR, gArg> aG)
{
    static_assert(std::is_convertible_v<gR, fArg>,
                  "The return type of aG must be convertible to the argument type of aF.");

    auto tFOfG = [aF, aG](const gArg& aX) { return aF.f(aG.f(aX)); };

    auto tDfOfG = [tF = std::move(aF), tG = std::move(aG)](const gArg& tX) { return tF.df(tG.f(tX)) * tG.df(tX); };
    using dfOfGR = std::invoke_result_t<decltype(tDfOfG), gArg>;
    return Plato::Functional::Function<fR, dfOfGR, gArg>{std::move(tFOfG), std::move(tDfOfG)};
}

}  // namespace Plato::Functional

#endif
